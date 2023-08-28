/*
 * hal_spi.c
 *
 *  Created on: 2023Äê2ÔÂ21ÈÕ
 *      Author: xinlei.hu
 *       brief: use loop without interrupt
 */
#include "hal_spi.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hal_systick.h"
#include "hal_gpio.h"
#include "CRC15.h"


static LDD_TDeviceData *halSpi0DevicePtr = NULL;
static LDD_TDeviceData *halSpi1DevicePtr = NULL;
static HAL_SPI_CLIENT_Struct Client;

static volatile uint8_t gsu8HalSpiTxCallBackFlag = 0;
static volatile uint8_t gsu8HalSpiRxCallBackFlag = 0;

uint8_t gsu8HalSpiRxBuf[280] = {0};
uint8_t gsu8HalSpiTxBuf[280] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
		  		  	  	  	  	  	 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 ,0x11, 0x22, 1, 2};



typedef union
{
	uint16_t u16Crc;
	uint8_t u8Crc[2];
} CrcUnion;

/**
 * @brief :init spi
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_spi_init(uint8_t Id, HAL_GPIO_PORT_Type CsPort, HAL_GPIO_PIN_Type CsPin)
{
    //init spi device
    Client.devSpiId = Id;
    Client.gpioCsPort = CsPort;
    Client.gpioCsPin = CsPin;
    switch(Id)
    {
        case HAL_DEV_SPI0:
            {
                halSpi0DevicePtr = SS0_Init(NULL);
                if(NULL == halSpi0DevicePtr)
                    return 1;
                //RDY signal
                hal_gpio_set_pin_dir(HAL_GPIOA_PTD, 7, HAL_GPIO_OUTPUT_DIRECTION, 1);
            }
            break;
        case HAL_DEV_SPI1:
            {
                //init CS port
                hal_gpio_set_pin_dir(Client.gpioCsPort, Client.gpioCsPin, HAL_GPIO_OUTPUT_DIRECTION, 0);
                halSpi1DevicePtr = SM1_Init(NULL);
                if(NULL == halSpi1DevicePtr)
                    return 1;
            }
            break;
        default:break;
    }
    return 0;
}

uint16_t
hal_spi_int_init(void)
{
    gsu8HalSpiTxBuf[1] = rand()%0xFF;
    halSpi0DevicePtr = SS0_Init(NULL);
    if(NULL == halSpi0DevicePtr)
        return 1;
    SS0_ReceiveBlock(halSpi0DevicePtr, gsu8HalSpiRxBuf, 280);
    SS0_SendBlock(halSpi0DevicePtr, gsu8HalSpiTxBuf, 280);
    gsu8HalSpiTxBuf[278] = 0xaa;
    gsu8HalSpiTxBuf[279] = 0xBB;
    return 0;
}

/**
 * @brief :deinit spi
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_spi_deinit(uint8_t Id)
{
    //init spi device
    switch(Id)
    {
        case HAL_DEV_SPI0:
            SS0_Deinit(NULL);
            break;
        case HAL_DEV_SPI1:
            SM1_Deinit(NULL);
            break;
        default:break;
    }
    return 0;
}

/**
 * @brief :spi master send and recv blocking func
 *
 * @param Client :device struct
 * @param SendBuf :buffer to write
 * @param RecvBuf :storage value of read data
 * @param Len :length
 * @param TimeoutMs :blocking time
 *
 * @return 
 */
uint16_t
hal_spi_master_transfer_data_blocking(uint8_t Id,
                                      const uint8_t *SendBuf,
									  uint8_t *RecvBuf,
                                      uint16_t Len,
                                      uint32_t Timeout)
{
    uint32_t u32TimeOut = 0;
    uint16_t index = 0;
    uint16_t u16Err = 0;

    switch(Id)
    {
        case HAL_DEV_SPI0:
            {

            }
            break;
        case HAL_DEV_SPI1:
           {
        	    EnterCritical();
                if (SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL)
                {
                    SPI_PDD_ReadData8bit(SPI1_BASE_PTR);
                }
                //set cs
                hal_gpio_write_pin(Client.gpioCsPort, Client.gpioCsPin, 0);
                while (index < Len)
                {
                    u32TimeOut = 0;
                    //wait for send buf to be empty
                    while (!(SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_TX_BUFFER_EMPTYG) && (u32TimeOut < Timeout))
                    {
                        u32TimeOut ++;
                    }
                    if(u32TimeOut == Timeout)
                    {
                    	u16Err = 1;
                        break;
                    }
                    //send
                    SPI_PDD_WriteData8Bit(SPI1_BASE_PTR, SendBuf[index]);
                    u32TimeOut = 0;
                    //wait for recv buf to be full
                    while (!(SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL) && (u32TimeOut < Timeout))
                    {
                        u32TimeOut ++;
                    }
                    if(u32TimeOut == Timeout)
                    {
                    	u16Err = 2;
                        break;
                    }
                    //recv
                    RecvBuf[index] = SPI_PDD_ReadData8bit(SPI1_BASE_PTR);
                    index ++;
                }
                //unset cs
                hal_gpio_write_pin(Client.gpioCsPort, Client.gpioCsPin, 1);
                ExitCritical();
            }
            break;
        default:u16Err = 3;
    }
    return u16Err;
}

/**
 * @brief :spi slave send and recv blocking func
 *
 * @param Client :device struct
 * @param SendBuf :buffer to write
 * @param RecvBuf :storage value of read data
 * @param Len :length
 * @param TimeoutMs :blocking time
 *
 * @return 
 */
uint16_t
hal_spi_slave_transfer_data_blocking(uint8_t Id,
								   	 uint8_t *SendBuf,
									 uint8_t *RecvBuf,
                                     uint16_t Len,
                                     uint32_t Timeout)
{
    uint32_t u32TimeOut = 0;
    uint16_t index = 0;
    uint16_t u16Err = 0;

    switch(Id)
    {
        case HAL_DEV_SPI0:
            {
                while (index < Len)
                {
                    u32TimeOut = 0;
                    //wait for send buf to be empty
                    while (!(SPI_PDD_ReadStatusReg(SPI0_BASE_PTR) & SPI_PDD_TX_BUFFER_EMPTYG) && (u32TimeOut < Timeout))
                    {
                        u32TimeOut ++;
                    }
                    if(u32TimeOut >= Timeout)
                    {
                        uint8_t u8Tmp = SPI_PDD_ReadData8bit(SPI0_BASE_PTR);
                    	u8Tmp = u8Tmp;
                    	u16Err = 1;
                    	break;
                    }
                    //send
                    SPI_PDD_WriteData8Bit(SPI0_BASE_PTR, SendBuf[index]);
                    u32TimeOut = 0;
                    //wait for recv buf to be full
                    while (!(SPI_PDD_ReadStatusReg(SPI0_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL) && (u32TimeOut < Timeout))
                    {
                        u32TimeOut ++;
                    }
                    if(u32TimeOut >= Timeout)
                    {
                    	uint8_t u8Tmp = SPI_PDD_ReadData8bit(SPI0_BASE_PTR);
                    	u8Tmp = u8Tmp;
                        u16Err = 2;
                        break;
                    }
                    //recv
                    RecvBuf[index] = SPI_PDD_ReadData8bit(SPI0_BASE_PTR);
                    index ++;
                }
            }
            break;
        case HAL_DEV_SPI1:
            {
                if (SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL)
                {
                    SPI_PDD_ReadData8bit(SPI1_BASE_PTR);
                }
                while (index < Len)
                {
                    //send
                    u32TimeOut = 0;
                    while (((SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_TX_BUFFER_EMPTYG) == 0U) && (u32TimeOut < Timeout)) /* Is HW Tx buffer empty? */
                    {
                        u32TimeOut ++;
                    }
                    if(u32TimeOut == Timeout)
                    {
                        u16Err = 1;
                        break;
                    }
                    SPI_PDD_WriteData8Bit(SPI1_BASE_PTR, SendBuf[index]);

                    //recv
                    u32TimeOut = 0;
                    while (((SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL) == 0U) && (u32TimeOut < Timeout)) /* Is any char in HW Rx buffer? */
                    {
                        u32TimeOut ++;
                    }
                    if(u32TimeOut == Timeout)
                    {
                    	u16Err = 2;
                        break;
                    }
                    RecvBuf[index] = SPI_PDD_ReadData8bit(SPI1_BASE_PTR);
                    index ++;
                }
            }
            break;
        default:u16Err = 3;
    }
    return u16Err;
}

/**
 * @brief :spi master tx callback
 */
void
hal_spi_master_tx_callback(void)
{
}

/**
 * @brief :spi master rx callback
 */
void
hal_spi_master_rx_callback(void)
{
}


void
hal_spi_slave_tx_callback(void)
{
    gsu8HalSpiTxCallBackFlag = 1;
}

/**
 * @brief :spi slave rx callback
 */

void
hal_spi_slave_rx_callback(void)
{
//    gsu8HalSpiRxCallBackFlag = 1;

    SS0_ReceiveBlock(halSpi0DevicePtr, gsu8HalSpiRxBuf, 280);
    SS0_SendBlock(halSpi0DevicePtr, gsu8HalSpiTxBuf, 280);
}
/**
 * @brief :spi slave rx callback
 */
void
hal_spi_slave_cs_callback(void)
{
    uint16_t index = 0;
    uint16_t len = 280;
    uint16_t u16Timeout = 0;
    uint8_t u8Err = 0;


    {
        DMA_Data_CMD_Handle(gsu8HalSpiTxBuf, 280);
        if (SPI_PDD_ReadStatusReg(SPI0_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL)
        {
            SPI_PDD_ReadData8bit(SPI0_BASE_PTR);
        }
        //TODO:pull pin
        hal_gpio_write_pin(HAL_GPIOA_PTD, 7, 0);
        while (index < len)
        {
            //send
            u16Timeout = 0;
            while (((SPI_PDD_ReadStatusReg(SPI0_BASE_PTR) & SPI_PDD_TX_BUFFER_EMPTYG) == 0U) && (u16Timeout<0xFFFF)) /* Is HW Tx buffer empty? */
            {
                u16Timeout ++;
            }
            if(u16Timeout == 0xFFFF)
            {
                u8Err = 1;
                break;
            }
            SPI_PDD_WriteData8Bit(SPI0_BASE_PTR, gsu8HalSpiTxBuf[index]);

            //recv
            u16Timeout = 0;
            while (((SPI_PDD_ReadStatusReg(SPI0_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL) == 0U) && (u16Timeout<0xFFFF)) /* Is any char in HW Rx buffer? */
            {
                u16Timeout ++;
            }
            if(u16Timeout == 0xFFFF)
            {
                u8Err = 2;
                break;
            }
            gsu8HalSpiRxBuf[index] = SPI_PDD_ReadData8bit(SPI0_BASE_PTR);
            index ++;
        }
        if(u8Err != 0 )
        {
            SS0_Deinit(halSpi0DevicePtr);
            halSpi0DevicePtr = SS0_Init(NULL);
            SPI_PDD_ReadStatusReg(SPI0_BASE_PTR);
            SPI_PDD_ReadData8bit(SPI0_BASE_PTR);
        }

    }
    hal_gpio_write_pin(HAL_GPIOA_PTD, 7, 1);
    //clear wdg
    //WDog1_Clear(NULL);
}

uint8_t 
DMA_Data_CMD_Handle(uint8_t *Data, uint16_t Len)
{
    volatile CrcUnion u16RxCrc;
	//u16RxCrc.u16Crc = PEC15(Data, 2);
	//if( (u16RxCrc.u8Crc[0] != Data[3]) || (u16RxCrc.u8Crc[1] != Data[2]))
	//{
	//	return 2;
	//}
    if(Len > 280)
        return 1;
	gsu8HalSpiTxBuf[2] = rand()%0xff;
	gsu8HalSpiTxBuf[60] = rand()%0xff;
	gsu8HalSpiTxBuf[100] = rand()%0xff;
	gsu8HalSpiTxBuf[160] = rand()%0xff;
	gsu8HalSpiTxBuf[200] = rand()%0xff;
	gsu8HalSpiTxBuf[260] = rand()%0xff;

	//crc1
	u16RxCrc.u16Crc = PEC15(gsu8HalSpiTxBuf, 18);
	gsu8HalSpiTxBuf[18] = u16RxCrc.u8Crc[1];
	gsu8HalSpiTxBuf[19] = u16RxCrc.u8Crc[0];
	//crc2
	u16RxCrc.u16Crc = PEC15(&gsu8HalSpiTxBuf[20], 96);
	gsu8HalSpiTxBuf[116] = u16RxCrc.u8Crc[1];
	gsu8HalSpiTxBuf[117] = u16RxCrc.u8Crc[0];
	//crc3
	u16RxCrc.u16Crc = PEC15(&gsu8HalSpiTxBuf[118], 35);
	gsu8HalSpiTxBuf[153] = u16RxCrc.u8Crc[1];
	gsu8HalSpiTxBuf[154] = u16RxCrc.u8Crc[0];
	//crc4
	u16RxCrc.u16Crc = PEC15(&gsu8HalSpiTxBuf[155], 38);
	gsu8HalSpiTxBuf[193] = u16RxCrc.u8Crc[1];
	gsu8HalSpiTxBuf[194] = u16RxCrc.u8Crc[0];
	//crc5
	u16RxCrc.u16Crc = PEC15(&gsu8HalSpiTxBuf[195], 48);
	gsu8HalSpiTxBuf[243] = u16RxCrc.u8Crc[1];
	gsu8HalSpiTxBuf[244] = u16RxCrc.u8Crc[0];

	//crc6
	u16RxCrc.u16Crc = PEC15(&gsu8HalSpiTxBuf[245], 33);
	gsu8HalSpiTxBuf[278] = u16RxCrc.u8Crc[1];
	gsu8HalSpiTxBuf[279] = u16RxCrc.u8Crc[0];
    return 0;
}











