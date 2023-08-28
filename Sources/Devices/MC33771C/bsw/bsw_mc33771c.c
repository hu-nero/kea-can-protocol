/*
 * MC33771C.c
 *
 *  Created on: 2023Äê6ÔÂ2ÈÕ
 *      Author: xinlei.hu
 */

#include "bsw_mc33771c.h"
#include "MC33771C/bcc_kea128/bcc_peripheries.h"
#include "Hal.h"

volatile int exit_code = 0;


/*******************************************************************************
 * Definitions (depends on the utilized HW setup)
 ******************************************************************************/

/* Used BCC device */
#define MC33771C    /* Define MC33771C if RD33771CDSTEVB's are used. */
//#define MC33772C    /* Define MC33772C if KIT33772CTPLEVB's are used. */

#if !defined(MC33771C) && !defined(MC33772C)
    #error "Select used BCC device by defining MC33771C or MC33772C."
#endif

#if defined(MC33771C) && defined(MC33772C)
    #error "Select only one type of BCC device."
#endif

/*******************************************************************************
 * Global variables
 ******************************************************************************/

bcc_drv_config_t bccConfig1;     /* BCC driver configuration (first chain). */
bcc_drv_config_t bccConfig2;     /* BCC driver configuration (second chain). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Red on-board LED. */
#define RED_LED_PORT   PTD
#define RED_LED_PIN    15U

/* LPSPI_TX configuration. */
#define BCC_TX_LPSPI_DELAY_PCS_TO_SCLK         3U  /* 3us (f >= 1.75us) */
#define BCC_TX_LPSPI_DELAY_SCLK_TO_PCS         1U  /* 1us (g >= 0.60us) */
#define BCC_TX_LPSPI_DELAY_BETWEEN_TRANSFERS   5U  /* 5us (t_MCU_RES >= 4us) */

/*******************************************************************************
 * Initial BCC configuration
 ******************************************************************************/

/*! @brief  Number of MC33771 registers configured in the initialization with
 * user values. */
#define BCC_INIT_CONF_REG_CNT     5U

#define MC33771C_SYS_CFG1_VALUE ( \
    MC33771C_SYS_CFG1_CYCLIC_TIMER(MC33771C_SYS_CFG1_CYCLIC_TIMER_DISABLED_ENUM_VAL) | \
    MC33771C_SYS_CFG1_DIAG_TIMEOUT(MC33771C_SYS_CFG1_DIAG_TIMEOUT_1S_ENUM_VAL) | \
    MC33771C_SYS_CFG1_I_MEAS_EN(MC33771C_SYS_CFG1_I_MEAS_EN_ENABLED_ENUM_VAL) | \
    MC33771C_SYS_CFG1_CB_DRVEN(MC33771C_SYS_CFG1_CB_DRVEN_DISABLED_ENUM_VAL) | \
    MC33771C_SYS_CFG1_GO2DIAG(MC33771C_SYS_CFG1_GO2DIAG_EXIT_ENUM_VAL) | \
    MC33771C_SYS_CFG1_CB_MANUAL_PAUSE(MC33771C_SYS_CFG1_CB_MANUAL_PAUSE_DISABLED_ENUM_VAL) | \
    MC33771C_SYS_CFG1_SOFT_RST(MC33771C_SYS_CFG1_SOFT_RST_DISABLED_ENUM_VAL) | \
    MC33771C_SYS_CFG1_FAULT_WAVE(MC33771C_SYS_CFG1_FAULT_WAVE_DISABLED_ENUM_VAL) | \
    MC33771C_SYS_CFG1_WAVE_DC_BITX(MC33771C_SYS_CFG1_WAVE_DC_BITX_500US_ENUM_VAL) \
)

/* Initial value of SYS_CFG2 register. */
#define MC33771C_SYS_CFG2_VALUE ( \
    MC33771C_SYS_CFG2_FLT_RST_CFG(MC33771C_SYS_CFG2_FLT_RST_CFG_OSC_MON_RESET_ENUM_VAL) | \
    MC33771C_SYS_CFG2_TIMEOUT_COMM(MC33771C_SYS_CFG2_TIMEOUT_COMM_256MS_ENUM_VAL) | \
    MC33771C_SYS_CFG2_NUMB_ODD(MC33771C_SYS_CFG2_NUMB_ODD_EVEN_ENUM_VAL) | \
    MC33771C_SYS_CFG2_HAMM_ENCOD(MC33771C_SYS_CFG2_HAMM_ENCOD_DECODE_ENUM_VAL) \
)

/* Initial value of ADC_CFG register. */
#define MC33771C_ADC_CFG_VALUE ( \
    MC33771C_ADC_CFG_AVG(MC33771C_ADC_CFG_AVG_NO_AVERAGING_ENUM_VAL) | \
    MC33771C_ADC_CFG_PGA_GAIN(MC33771C_ADC_CFG_PGA_GAIN_AUTO_ENUM_VAL) | \
    MC33771C_ADC_CFG_ADC1_A_DEF(MC33771C_ADC_CFG_ADC1_A_DEF_16_BIT_ENUM_VAL) | \
    MC33771C_ADC_CFG_ADC1_B_DEF(MC33771C_ADC_CFG_ADC1_B_DEF_16_BIT_ENUM_VAL) | \
    MC33771C_ADC_CFG_ADC2_DEF(MC33771C_ADC_CFG_ADC2_DEF_16_BIT_ENUM_VAL) \
)

/* Initial value of ADC2_OFFSET_COMP register. */
#define MC33771C_ADC2_OFFSET_COMP_VALUE (\
    MC33771C_ADC2_OFFSET_COMP_CC_RST_CFG(MC33771C_ADC2_OFFSET_COMP_CC_RST_CFG_CC_RESET_ENUM_VAL) | \
    MC33771C_ADC2_OFFSET_COMP_FREE_CNT(MC33771C_ADC2_OFFSET_COMP_FREE_CNT_CLAMP_ENUM_VAL) | \
    MC33771C_ADC2_OFFSET_COMP_ALLCBOFFONSHORT(MC33771C_ADC2_OFFSET_COMP_ALLCBOFFONSHORT_SHORTED_ENUM_VAL) | \
    MC33771C_ADC2_OFFSET_COMP_ADC2_OFFSET_COMP(BCC_GET_ADC2_OFFSET(0)) \
)

/* Initial value of OV_UV_EN register.
 * Note: MC33772C does not have OV_UV_EN[CTx_OVUV], x = {7 .. 14}.
 *       However, they are set to '0' in the macro below --> The macro
 *       can be used also for MC33772C. */
#define MC33771C_OV_UV_EN_VALUE ( \
    MC33771C_OV_UV_EN_COMMON_UV_TH(MC33771C_OV_UV_EN_COMMON_UV_TH_COMMON_ENUM_VAL) | \
    MC33771C_OV_UV_EN_COMMON_OV_TH(MC33771C_OV_UV_EN_COMMON_OV_TH_COMMON_ENUM_VAL) | \
    /* Disable ADC data to be compared with thresholds for OV/UV. */ \
    MC33771C_OV_UV_EN_CT14_OVUV_EN(MC33771C_OV_UV_EN_CT14_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT13_OVUV_EN(MC33771C_OV_UV_EN_CT13_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT12_OVUV_EN(MC33771C_OV_UV_EN_CT12_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT11_OVUV_EN(MC33771C_OV_UV_EN_CT11_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT10_OVUV_EN(MC33771C_OV_UV_EN_CT10_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT9_OVUV_EN(MC33771C_OV_UV_EN_CT9_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT8_OVUV_EN(MC33771C_OV_UV_EN_CT8_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT7_OVUV_EN(MC33771C_OV_UV_EN_CT7_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT6_OVUV_EN(MC33771C_OV_UV_EN_CT6_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT5_OVUV_EN(MC33771C_OV_UV_EN_CT5_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT4_OVUV_EN(MC33771C_OV_UV_EN_CT4_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT3_OVUV_EN(MC33771C_OV_UV_EN_CT3_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT2_OVUV_EN(MC33771C_OV_UV_EN_CT2_OVUV_EN_DISABLED_ENUM_VAL) | \
    MC33771C_OV_UV_EN_CT1_OVUV_EN(MC33771C_OV_UV_EN_CT1_OVUV_EN_DISABLED_ENUM_VAL) \
)

/* Structure containing a register name and its address. */
typedef struct
{
    const uint8_t address;
    const uint16_t defaultVal;
    const uint16_t value;
} bcc_init_reg_t;

/* Initial register configuration for MC33771C/MC33772C for this example. */
static const bcc_init_reg_t s_initRegs[BCC_INIT_CONF_REG_CNT] = {
    {MC33771C_OV_UV_EN_OFFSET, MC33771C_OV_UV_EN_POR_VAL, MC33771C_OV_UV_EN_VALUE},
    {MC33771C_SYS_CFG1_OFFSET, MC33771C_SYS_CFG1_POR_VAL, MC33771C_SYS_CFG1_VALUE},
    {MC33771C_SYS_CFG2_OFFSET, MC33771C_SYS_CFG2_POR_VAL, MC33771C_SYS_CFG2_VALUE},
    {MC33771C_ADC_CFG_OFFSET, MC33771C_ADC_CFG_POR_VAL, MC33771C_ADC_CFG_VALUE},
    {MC33771C_ADC2_OFFSET_COMP_OFFSET, MC33771C_ADC2_OFFSET_COMP_POR_VAL, MC33771C_ADC2_OFFSET_COMP_VALUE},
};

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/

static bcc_status_t initRegisters(bcc_drv_config_t* const drvConfig);
static bcc_status_t clearFaultRegs(bcc_drv_config_t* const drvConfig);
static bcc_status_t getVoltMeasurements(bcc_drv_config_t* const drvConfig,
		 const bcc_cid_t cid, uint32_t measurements[]);
static bcc_status_t doVoltMeasurements();
static bcc_status_t doGetIcTemperature();
static bcc_status_t doGetIsenseVoltage();
static bcc_status_t doGetCoulombCounter();
static bcc_status_t doStartBalance();
/*******************************************************************************
 * Functions
 ******************************************************************************/

/*!
 * @brief Initializes BCC device registers according to BCC_INIT_CONF.
 * Registers having the wanted content already after POR are not rewritten.
 */
static bcc_status_t initRegisters(bcc_drv_config_t* const drvConfig)
{
    bcc_status_t status;

    for(uint8_t cid=0;cid<drvConfig->devicesCnt;cid++)
    {
        for (uint8_t regId = 0; regId < BCC_INIT_CONF_REG_CNT; regId++)
        {
            if (s_initRegs[regId].value != s_initRegs[regId].defaultVal)
            {
                status = BCC_Reg_Write(drvConfig, drvConfig->deviceId[cid],
                        s_initRegs[regId].address, s_initRegs[regId].value);
                if (status != BCC_STATUS_SUCCESS)
                {
                    return status;
                }
            }
        }
    }

    return BCC_STATUS_SUCCESS;
}

/*!
 * @brief Clears all fault registers of BCC devices.
 */
static bcc_status_t clearFaultRegs(bcc_drv_config_t* const drvConfig)
{
    bcc_status_t status;

    status = BCC_Fault_ClearStatus(drvConfig, BCC_CID_DEV1, BCC_FS_CELL_OV);
    if (status != BCC_STATUS_SUCCESS)
    {
        return status;
    }

    status = BCC_Fault_ClearStatus(drvConfig, BCC_CID_DEV1, BCC_FS_CELL_UV);
    if (status != BCC_STATUS_SUCCESS)
    {
        return status;
    }

    status = BCC_Fault_ClearStatus(drvConfig, BCC_CID_DEV1, BCC_FS_CB_OPEN);
    if (status != BCC_STATUS_SUCCESS)
    {
        return status;
    }

    status = BCC_Fault_ClearStatus(drvConfig, BCC_CID_DEV1, BCC_FS_CB_SHORT);
    if (status != BCC_STATUS_SUCCESS)
    {
        return status;
    }

    status = BCC_Fault_ClearStatus(drvConfig, BCC_CID_DEV1, BCC_FS_GPIO_STATUS);
    if (status != BCC_STATUS_SUCCESS)
    {
        return status;
    }

    status = BCC_Fault_ClearStatus(drvConfig, BCC_CID_DEV1, BCC_FS_AN_OT_UT);
    if (status != BCC_STATUS_SUCCESS)
    {
        return status;
    }

    status = BCC_Fault_ClearStatus(drvConfig, BCC_CID_DEV1, BCC_FS_GPIO_SHORT);
    if (status != BCC_STATUS_SUCCESS)
    {
        return status;
    }

    status = BCC_Fault_ClearStatus(drvConfig, BCC_CID_DEV1, BCC_FS_COMM);
    if (status != BCC_STATUS_SUCCESS)
    {
        return status;
    }

    status = BCC_Fault_ClearStatus(drvConfig, BCC_CID_DEV1, BCC_FS_FAULT1);
    if (status != BCC_STATUS_SUCCESS)
    {
        return status;
    }

    status = BCC_Fault_ClearStatus(drvConfig, BCC_CID_DEV1, BCC_FS_FAULT2);
    if (status != BCC_STATUS_SUCCESS)
    {
        return status;
    }

    return BCC_Fault_ClearStatus(drvConfig, BCC_CID_DEV1, BCC_FS_FAULT3);
}

/*!
 * @brief MCU and BCC initialization.
 */
bcc_status_t
initDemo(void)
{
	bcc_status_t  bccError = BCC_STATUS_SUCCESS;
	//init gpio
    //MC33771 reset pin
	hal_gpio_set_pin_dir(HAL_GPIOA_PTC, 6, HAL_GPIO_OUTPUT_DIRECTION, 1);
	hal_gpio_set_pin_dir(HAL_GPIOB_PTE, 4, HAL_GPIO_OUTPUT_DIRECTION, 1);
	hal_gpio_set_pin_dir(HAL_GPIOB_PTF, 3, HAL_GPIO_OUTPUT_DIRECTION, 1);
	//MC33771 SPI enable pin
	hal_gpio_set_pin_dir(HAL_GPIOB_PTG, 1, HAL_GPIO_OUTPUT_DIRECTION, 1);
	hal_gpio_set_pin_dir(HAL_GPIOB_PTG, 2, HAL_GPIO_OUTPUT_DIRECTION, 1);
	hal_gpio_set_pin_dir(HAL_GPIOB_PTG, 3, HAL_GPIO_OUTPUT_DIRECTION, 0);
	hal_gpio_set_pin_dir(HAL_GPIOB_PTE, 3, HAL_GPIO_INPUT_DIRECTION, 0);
	//init spi
    hal_spi_init(HAL_DEV_SPI1, HAL_GPIOB_PTG, HAL_GPIO_PIN2);
    //init pit
    hal_pit_init(HAL_DEV_PIT0);
	/* Initialize BCC driver configuration structures. */
	bccConfig1.commMode = BCC_MODE_TPL;
	bccConfig1.drvInstance = 0U;
	bccConfig1.deviceIndex = 0U;
	bccConfig1.devicesCnt = 3U;
	bccConfig1.device[0] = BCC_DEVICE_MC33771C;
	bccConfig1.device[1] = BCC_DEVICE_MC33771C;
	bccConfig1.device[2] = BCC_DEVICE_MC33771C;
	bccConfig1.cellCnt[0] = 14U;
	bccConfig1.cellCnt[1] = 14U;
	bccConfig1.cellCnt[2] = 14U;
	bccConfig1.loopBack = false;

	/* Initialize BCC devices. */
	bccError = BCC_Init(&bccConfig1);
	if (bccError != BCC_STATUS_SUCCESS)
	{
		return bccError;
	}

	/* Initialize registers of BCC devices. */
	bccError = initRegisters(&bccConfig1);
	if (bccError != BCC_STATUS_SUCCESS)
	{
		return bccError;
	}

	/* Clear fault registers. */
	bccError = clearFaultRegs(&bccConfig1);
	if (bccError != BCC_STATUS_SUCCESS)
	{
		return bccError;
	}
	return bccError;
}


/*!
 * @brief This function starts on-demand conversion and reads measured
 * stack and cell voltages.
 *
 * @param cid          Cluster Identification Address.
 * @param measurements [0] Stack voltage in [uV].
 *                     [1] Cell 1 voltage in [uV].
 *                     [6/14] Cell 6/14 voltage in [uV].
 *
 * @return bcc_status_t Error code.
 */
static bcc_status_t getVoltMeasurements(bcc_drv_config_t* const drvConfig,
		 const bcc_cid_t cid, uint32_t measurements[])
{
    bcc_status_t error;

    /* Start conversion and wait for the conversion time. */
    error = BCC_Meas_StartAndWait(drvConfig, cid, BCC_AVG_1);
    if (error != BCC_STATUS_SUCCESS)
    {
        return error;
    }

    /* Read measured values. */
    error = BCC_Meas_GetStackVoltage(drvConfig, cid, &measurements[0]);
    if (error != BCC_STATUS_SUCCESS)
    {
        return error;
    }

    return BCC_Meas_GetCellVoltages(drvConfig, cid, &measurements[1]);
}

/*!
 * @brief This function prints measures stack and cell voltages and prints
 * the results to serial console output.
 *
 * @return bcc_status_t Error code.
 */
static bcc_status_t doVoltMeasurements()
{
    uint32_t measChain1[3][15];
    bcc_status_t error;

    for(uint8_t i=1;i<=3;i++)
    {
        if ((error = getVoltMeasurements(&bccConfig1, i, measChain1[i-1])) != BCC_STATUS_SUCCESS)
        {
            return error;
        }
    }

    return BCC_STATUS_SUCCESS;
}

/*!
 * @brief This function prints ic temperature.
 *
 * @return bcc_status_t Error code.
 */
static bcc_status_t doGetIcTemperature()
{
    bcc_status_t error;
    int16_t icTemp[3];
    for(uint8_t i=1;i<=3;i++)
    {
        if ((error = BCC_Meas_GetIcTemperature(&bccConfig1, i, BCC_TEMP_CELSIUS, &icTemp[i-1])) != BCC_STATUS_SUCCESS)
        {
            return error;
        }
    }
    return error;
}

static bcc_status_t doGetIsenseVoltage()
{
    bcc_status_t error;
    int32_t isenseVolt[3] = {0};

    for(uint8_t i=1;i<=3;i++)
    {
        /* Start conversion and wait for the conversion time. */
        error = BCC_Meas_StartAndWait(&bccConfig1, i, BCC_AVG_1);
        if (error != BCC_STATUS_SUCCESS)
        {
            return error;
        }
        if ((error = BCC_Meas_GetIsenseVoltage(&bccConfig1, i, &isenseVolt[i-1])) != BCC_STATUS_SUCCESS)
        {
            return error;
        }
    }
    return error;
}

static bcc_status_t doGetCoulombCounter()
{
    bcc_status_t error;
    bcc_cc_data_t cc[3] = {0};

    for(uint8_t i=1;i<=3;i++)
    {
        if ((error = BCC_Meas_GetCoulombCounter(&bccConfig1, i, &cc[i-1])) != BCC_STATUS_SUCCESS)
        {
            return error;
        }
    }
    return error;
}

//Currently off balance
static bcc_status_t doStartBalance()
{
    bcc_status_t error;
    bool isCbEnable;
    uint16_t u16Tmp[14] = {0};

    for(uint8_t i=1;i<=3;i++)
    {

        //is CellBalance enable
        if ((error = BCC_CB_IsEnable(&bccConfig1, i, &isCbEnable)) != BCC_STATUS_SUCCESS)
        {
            return error;
        }
        if (!isCbEnable)
        {
            if ((error = BCC_CB_Enable(&bccConfig1, i, true)) != BCC_STATUS_SUCCESS)
            {
                return error;
            }
        }

        //start cell balance(timer between 0 and 30s)
        for(uint8_t cellIndex=0;cellIndex<14;cellIndex++)
        {
            if ((error = BCC_CB_SetIndividual(&bccConfig1, i, cellIndex, true, 5)) != BCC_STATUS_SUCCESS)
            {
                return error;
            }
        }

        //read cell balance reg
		if (0 != BCC_Reg_Read(&bccConfig1, i, MC33771C_CB1_CFG_OFFSET, 14, u16Tmp))
		{
			return 1;
		}
		u16Tmp[0] = u16Tmp[0];

        //close the balance main flag
        if ((error = BCC_CB_Pause(&bccConfig1, i, true)) != BCC_STATUS_SUCCESS)
        {
            return error;
        }
    }
    return error;
}
/*!
 * @brief This function periodically wakes-up the BCC devices, does stack and
 * cell voltage measurements, prints the results to the serial console output
 * and moves BCC devices into the sleep mode.
 *
 * @return bcc_status_t Error code.
 */
void startApp(void)
{
    bcc_status_t status;
    uint32_t i;
    if (initDemo() != 0)
    {
    	return;
    }
    /* Do a measurement and print the measured values. */
    while (1)
    {
        status = doVoltMeasurements();
        if (status != BCC_STATUS_SUCCESS)
        {
        	break;
        }
        status = doGetIcTemperature();
		if (status != BCC_STATUS_SUCCESS)
		{
			break;
		}
		status = doGetCoulombCounter();
		if (status != BCC_STATUS_SUCCESS)
		{
			break;
		}

		//if( status != 0)
		{
			status = doGetIsenseVoltage();
			if (status != BCC_STATUS_SUCCESS)
			{
				break;
			}
		}
		status = doStartBalance();
		if (status != BCC_STATUS_SUCCESS)
		{
			break;
				}
        status = BCC_Sleep(&bccConfig1);
        if (status != BCC_STATUS_SUCCESS)
        {
        	break;
        }

        /* Wait a while.. */
        for (i = 0; i < 1000; i++);

        BCC_WakeUp(&bccConfig1);
    }
    status = status;
}


