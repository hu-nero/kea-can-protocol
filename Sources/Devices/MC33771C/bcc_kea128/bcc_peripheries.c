/*
 * Copyright 2016 - 2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * File: bcc_peripheries.c
 *
 * This file implements functions for LPSPI and GPIO operations required by BCC
 * driver. It is closely related to this demo example.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "Cpu.h"
#include "bcc_peripheries.h"
#include "Hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* LPIT channel used for timeout implementation. */
#define LPIT_CHANNEL              3U

/* Number of bytes what 48bits needs to be aligned in S32K144 SDK LPSPI driver
 * to. */
#define LPSPI_ALIGNMENT           6

/* Timeout for sending one 48b frame via SPI TX. */
#define BCC_TX_COM_TIMEOUT_MS     1

/* Timeout for SPI TX communication in milliseconds. Note that the maximal
 * transfer (receiving of 127 registers) takes 4 ms. Another 0.95 us
 * (t_port_delay) is introduced by each repeater in 33771. */
#define BCC_RX_COM_TIMEOUT_MS     10

/* EN - PTB0 (TPL1 interface) */
#define BCC_EN1_INSTANCE          HAL_GPIOB_PTG
#define BCC_EN1_INDEX             3

/* EN - PTD4 (TPL2 interface) */
#define BCC_EN2_INSTANCE          HAL_GPIOA_PTD
#define BCC_EN2_INDEX             4

/* INTB - PTB1 (TPL1 interface) */
#define BCC_INTB1_INSTANCE        HAL_GPIOB_PTE
#define BCC_INTB1_INDEX           3

/* INTB - PTB12 (TPL2 interface) */
#define BCC_INTB2_INSTANCE        HAL_GPIOB_PTE
#define BCC_INTB2_INDEX           12

/* CSB_TX1 - PTA6/LPSPI1_PCS1 (TPL1 interface) */
#define BCC_TX1_LPSPI_PCS         HAL_GPIOB_PTG
#define BCC_TX1_LPSPI_PCS_PIN     2

/* CSB_TX2 - PTA16/LPSPI1_PCS2 (TPL2 interface) */
#define BCC_TX2_LPSPI_PCS         HAL_GPIOB_PTG
#define BCC_TX2_LPSPI_PCS_PIN     2

/* CLK_OE - PTG1/MC33664_CLK_OE (TPL1 interface) */
#define BCC_TPL1_CLK_OE_PORT      HAL_GPIOB_PTG
#define BCC_TPL1_CLK_OE_PIN       1

/* CLK_OE - PTG1/MC33664_CLK_OE (TPL2 interface) */
#define BCC_TPL2_CLK_OE_PORT      HAL_GPIOB_PTG
#define BCC_TPL2_CLK_OE_PIN       1

#define _L_RESET_ON hal_gpio_write_pin(HAL_GPIOA_PTC, 6, 0);
#define _L_RESET_OFF hal_gpio_write_pin(HAL_GPIOA_PTC, 6, 1);
#define _M_RESET_ON hal_gpio_write_pin(HAL_GPIOB_PTE, 4, 0);
#define _M_RESET_OFF hal_gpio_write_pin(HAL_GPIOB_PTE, 4, 1);
#define _H_RESET_ON hal_gpio_write_pin(HAL_GPIOB_PTF, 3, 0);
#define _H_RESET_OFF hal_gpio_write_pin(HAL_GPIOB_PTF, 3, 1);

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : BCC_MCU_StartTimeout
 * Description   : Starts a non-blocking timeout mechanism. After expiration of
 *                 the time passed as a parameter, function
 *                 BCC_MCU_TimeoutExpired should signalize an expired timeout.
 *
 *END**************************************************************************/
bcc_status_t BCC_MCU_StartTimeout(const uint32_t timeoutUs)
{
    uint16_t status;

    /* Initialize LPIT channel 3 and configure it as a periodic counter
     * which is used to generate an interrupt. */
    status = hal_pit_timeus_set(HAL_DEV_PIT0, timeoutUs);
    return status;

}

/*FUNCTION**********************************************************************
 *
 * Function Name : BCC_MCU_TimeoutExpired
 * Description   : This function returns state of the timeout mechanism started
 *                 by the function BCC_MCU_StartTimeout.
 *
 *END**************************************************************************/
bool BCC_MCU_TimeoutExpired(void)
{
    return hal_pit_expiredFlag_get();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BCC_MCU_TransferSpi
 * Description   : This function is required by the BCC driver. As the SPI mode
 *                 is not targeted, this function is never called and it does
 *                 nothing.
 *
 *END**************************************************************************/
bcc_status_t BCC_MCU_TransferSpi(const uint8_t drvInstance, uint8_t txBuf[],
    uint8_t rxBuf[])
{
    return BCC_STATUS_SPI_FAIL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BCC_MCU_TransferTpl
 * Description   : This function sends and receives data via TX and RX SPI buses.
 *
 *END**************************************************************************/
bcc_status_t BCC_MCU_TxTpl(const uint8_t drvInstance, uint8_t transBuf[])
{
    uint32_t lpspiTxInstance;
    uint16_t u16Err;
    uint8_t recvBuf[LPSPI_ALIGNMENT];

    DEV_ASSERT(transBuf != NULL);

    lpspiTxInstance = (drvInstance == 0) ? HAL_DEV_SPI1 : HAL_DEV_SPI1;
    /* Transmissions at RX and TX SPI occur almost at the same time. Start
     * reading (response) at RX SPI first. */
    //(0)switch to MCU drive SCL
    BCC_MCU_WriteOePin(drvInstance, 1);
    u16Err = hal_spi_master_transfer_data_blocking(lpspiTxInstance, transBuf, recvBuf, LPSPI_ALIGNMENT, 10);
    if (u16Err != 0)
    {
        return BCC_STATUS_SPI_FAIL;
    }
    return BCC_STATUS_SUCCESS;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : BCC_MCU_TransferTpl
 * Description   : This function sends and receives data via TX and RX SPI buses.
 *
 *END**************************************************************************/
bcc_status_t BCC_MCU_TransferTpl(const uint8_t drvInstance, uint8_t transBuf[],
    uint8_t recvBuf[], const uint16_t recvTrCnt)
{
    uint32_t lpspiRxInstance;
    uint16_t timerCount;
    uint8_t index;
    SPI_MemMapPtr lpspiBaseInstance = (drvInstance == 0) ? SPI1_BASE_PTR : SPI1_BASE_PTR;
    lpspiRxInstance = (drvInstance == 0) ? HAL_DEV_SPI1 : HAL_DEV_SPI1;

    DEV_ASSERT(transBuf != NULL);
    DEV_ASSERT(recvBuf != NULL);
    DEV_ASSERT(recvTrCnt > 0);

    BCC_MCU_WriteOePin(drvInstance, 1);
    EnterCritical();
    //(1)set cs
    BCC_MCU_WriteCsbPin(lpspiRxInstance, 0);
    if (SPI_PDD_ReadStatusReg(lpspiBaseInstance) & SPI_PDD_RX_BUFFER_FULL)
    {
        SPI_PDD_ReadData8bit(lpspiBaseInstance);
    }
    index = 0;
    while (index < LPSPI_ALIGNMENT)
    {
        while (!(SPI_PDD_ReadStatusReg(lpspiBaseInstance) & SPI_PDD_TX_BUFFER_EMPTYG));//绛夊緟鍙戦�缂撳啿鍖虹┖
        SPI_PDD_WriteData8Bit(lpspiBaseInstance, transBuf[index]);
        index ++;
    }
    if (SPI_PDD_ReadStatusReg(lpspiBaseInstance) & SPI_PDD_RX_BUFFER_FULL)
    {
        SPI_PDD_ReadData8bit(lpspiBaseInstance);
    }

    BCC_MCU_WaitUs(8);
    //(2)configure slave spi mode
    SM1_Deinit(NULL);
    SS1_Init(NULL);
    //(3)switch to MC33664 drive SCL
    BCC_MCU_WriteOePin(lpspiRxInstance, 0);
    BCC_MCU_WaitUs(2);
    //(4)pull up cs
    BCC_MCU_WriteCsbPin(lpspiRxInstance,1);
    //(5)recv data
    index = 0;
    timerCount = 0;

    while ((index<(recvTrCnt*LPSPI_ALIGNMENT)) && (timerCount < 0xFFFF))
    {
        timerCount ++;
        if (SPI_PDD_ReadStatusReg(lpspiBaseInstance) & SPI_PDD_RX_BUFFER_FULL)
        {
            //绛夊緟鎺ユ敹缂撳啿鍖烘湁鏁版嵁
            recvBuf[index] = SPI_PDD_ReadData8bit(lpspiBaseInstance);
            index ++;
            timerCount = 0;
        }
    }

    ExitCritical();
    SS1_Deinit(NULL);
    SM1_Init(NULL);

    BCC_MCU_WriteOePin(lpspiRxInstance, 1);
    BCC_MCU_WaitUs(2);

    if (index != (recvTrCnt*LPSPI_ALIGNMENT))
    {
        return 1;
    }
    if (timerCount != 0)
    {
        return 2;
    }
    return BCC_STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BCC_MCU_Assert
 * Description   : User implementation of assert.
 *
 *END**************************************************************************/
void BCC_MCU_Assert(const bool x)
{
    DEV_ASSERT(x);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BCC_MCU_WriteCsbPin
 * Description   : Writes logic 0 or 1 to the CSB_TX pin.
 *
 *END**************************************************************************/
void BCC_MCU_WriteCsbPin(const uint8_t drvInstance, const uint8_t value)
{
    if (drvInstance == 0)
    {
        hal_gpio_write_pin(BCC_TX1_LPSPI_PCS, BCC_TX1_LPSPI_PCS_PIN, value);
    }
    else
    {
        hal_gpio_write_pin(BCC_TX2_LPSPI_PCS, BCC_TX2_LPSPI_PCS_PIN, value);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BCC_MCU_WriteRstPin
 * Description   : This function is required by the BCC driver. As RST pin
 *                 is not used in this example, this function does nothing.
 *
 *END**************************************************************************/
void BCC_MCU_WriteRstPin(const uint8_t drvInstance, const uint8_t value)
{
    BCC_MCU_WriteOePin(drvInstance, 1);
    BCC_MCU_WaitUs(10);
    _L_RESET_ON;
    _M_RESET_ON;
    _H_RESET_ON;
    BCC_MCU_WaitUs(200);
    _L_RESET_OFF;
    _M_RESET_OFF;
    _H_RESET_OFF;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BCC_MCU_WriteEnPin
 * Description   : Writes logic 0 or 1 to the EN pin of MC33664.
 *
 *END**************************************************************************/
void BCC_MCU_WriteEnPin(const uint8_t drvInstance, const uint8_t value)
{
    if (drvInstance == 0)
    {
        hal_gpio_write_pin(BCC_EN1_INSTANCE, BCC_EN1_INDEX, value);
    }
    else
    {
        hal_gpio_write_pin(BCC_EN1_INSTANCE, BCC_EN1_INDEX, value);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BCC_MCU_WriteOePin
 * Description   : Writes logic 0 or 1 to the OE pin of MC33664.
 *
 *END**************************************************************************/
void BCC_MCU_WriteOePin(const uint8_t drvInstance, const uint8_t value)
{
    if (drvInstance == 0)
    {
        hal_gpio_write_pin(BCC_TPL1_CLK_OE_PORT, BCC_TPL1_CLK_OE_PIN, value);
    }
    else
    {
        hal_gpio_write_pin(BCC_TPL2_CLK_OE_PORT, BCC_TPL2_CLK_OE_PIN, value);
    }
}
/*FUNCTION**********************************************************************
 *
 * Function Name : BCC_MCU_WriteEnPin
 * Description   : Reads logic value of INTB pin of MC33664.
 *
 *END**************************************************************************/
uint32_t BCC_MCU_ReadIntbPin(const uint8_t drvInstance)
{
    uint32_t value;
    if (drvInstance == 0)
    {
        hal_gpio_read_pin(BCC_INTB1_INSTANCE, BCC_INTB1_INDEX, &value);
    }
    else
    {
        hal_gpio_read_pin(BCC_INTB2_INSTANCE, BCC_INTB2_INDEX, &value);
    }
    return value;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
