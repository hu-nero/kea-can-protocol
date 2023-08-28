/*
 * hal_daisychain.c
 *
 *  Created on: 2023Äê2ÔÂ23ÈÕ
 *      Author: xinlei.hu
 */
#include "hal_daisychain.h"
#include <stdbool.h>
#include "hal_systick.h"
#include "SM1.h"
#include "SS1.h"

static LDD_TDeviceData *halDaChSpi1DevicePtr = NULL;




uint16_t
hal_spi_daisychain_init(HAL_DACH_SPI_Id Id)
{
    uint16_t u16Err = 0;
    //init spi device
    switch(Id)
    {
        case HAL_DACH_SPI0:
            {
            }
            break;
        case HAL_DACH_SPI1:
            {
                halDaChSpi1DevicePtr = SM1_Init(NULL);
                if(NULL == halDaChSpi1DevicePtr)
                    return 1;
                //MC33664_EN
                u16Err |= hal_gpio_set_pin_dir(MC33664_EN_PORT, MC33664_EN_PIN, HAL_GPIO_OUTPUT_DIRECTION, 1);
                //input MC33664_INT
                //MC33664_CLK_OE
                u16Err |= hal_gpio_set_pin_dir(MC33664_CLK_OE_PORT, MC33664_CLK_OE_PIN, HAL_GPIO_OUTPUT_DIRECTION, 1);
                //MC33664_CS_TX
                u16Err |= hal_gpio_set_pin_dir(MC33664_CS_TX_PORT, MC33664_CS_TX_PIN, HAL_GPIO_OUTPUT_DIRECTION, 1);
            }
            break;
        default:break;
    }
    return u16Err;
}


uint16_t
hal_spi_daisychain_recv_data_blocking(HAL_DACH_SPI_Id Id,
                                          uint8_t *SendBuf,
                                          uint8_t *RecvBuf,
                                          uint16_t Len,
                                          uint32_t TimeoutMs)
{
    uint32_t u32TimeOut = 0;
    uint16_t index = 0;

    switch(Id)
    {
        case HAL_DACH_SPI0:
            {
            }
            break;
        case HAL_DACH_SPI1:
            {
                //sck drive by mcu
                ENABLE_MC33664_CLK_OE;

                //select CS
                u32TimeOut = 0;
                ENABLE_MC33664_CS_TX;
                //(1)master spi send one frame
				while (index < Len)
				{
					u32TimeOut = 0;
					//wait for send buf to be empty
					while (!(SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_TX_BUFFER_EMPTYG) && (u32TimeOut < TimeoutMs))
					{
						u32TimeOut ++;
						hal_systick_delay_ms(1);
					}
					if(u32TimeOut >= TimeoutMs)
					{
						//unset cs
						DISABLE_MC33664_CS_TX;
						SM1_CancelBlockReception(halDaChSpi1DevicePtr);
						return 3;
					}
					//send
					SPI_PDD_WriteData8Bit(SPI1_BASE_PTR, SendBuf[index]);
					index ++;
				}
				//clean recv reg
				if ((SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL)){
					RecvBuf[0] = SPI_PDD_ReadData8bit(SPI1_BASE_PTR);
				}
                //(2)set slave mode
                SM1_Deinit(NULL);
                halDaChSpi1DevicePtr = SS1_Init(NULL);
                if(NULL == halDaChSpi1DevicePtr)
                    return 4;
                //TODO:test cs input mode
                //(3)sck drive by 33664
                DISABLE_MC33664_CLK_OE;
                //(4)unset cs
                DISABLE_MC33664_CS_TX;
                //(5)slave spi recv data
                index = 0;
                while (index < Len)
				{
					u32TimeOut = 0;
					//wait for recv buf to be full
					while (!(SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL) && (u32TimeOut < TimeoutMs))
					{
						u32TimeOut ++;
						hal_systick_delay_ms(1);
					}
					if(u32TimeOut >= TimeoutMs)
					{
						SS1_CancelBlockTransmission(halDaChSpi1DevicePtr);
						return 3;
					}
					//recv
					RecvBuf[index] = SPI_PDD_ReadData8bit(SPI1_BASE_PTR);
					index ++;
				}
                //recover master mode
                SS1_Deinit(NULL);
                halDaChSpi1DevicePtr = SM1_Init(NULL);
            }
            break;
        default:return 8;
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
hal_spi_daisychain_transfer_data_blocking(HAL_DACH_SPI_Id Id,
                                      uint8_t *SendBuf,
									  uint8_t *RecvBuf,
                                      uint16_t Len,
                                      uint32_t TimeoutMs)
{
    uint32_t u32TimeOut = 0;
    uint16_t index = 0;

    switch(Id)
    {
        case HAL_DACH_SPI0:
            {
            }
            break;
        case HAL_DACH_SPI1:
           {
                //set cs
                hal_gpio_write_pin(MC33664_CS_TX_PORT, MC33664_CS_TX_PIN, 0);
                while (index < Len)
                {
                    u32TimeOut = 0;
                    //wait for send buf to be empty
                    while (!(SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_TX_BUFFER_EMPTYG) && (u32TimeOut < TimeoutMs))
                    {
                        u32TimeOut ++;
                        hal_systick_delay_ms(1);
                    }
                    if(u32TimeOut >= TimeoutMs)
                    {
                        //unset cs
                        hal_gpio_write_pin(MC33664_CS_TX_PORT, MC33664_CS_TX_PIN, 1);
                        SM1_CancelBlockReception(halDaChSpi1DevicePtr);
                        return 3;
                    }
                    //send
                    SPI_PDD_WriteData8Bit(SPI1_BASE_PTR, SendBuf[index]);
                    u32TimeOut = 0;
                    //wait for recv buf to be full
                    while (!(SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL) && (u32TimeOut < TimeoutMs))
                    {
                        u32TimeOut ++;
                        hal_systick_delay_ms(1);
                    }
                    if(u32TimeOut >= TimeoutMs)
                    {
                        //unset cs
                        hal_gpio_write_pin(MC33664_CS_TX_PORT, MC33664_CS_TX_PIN, 1);
                        SM1_CancelBlockTransmission(halDaChSpi1DevicePtr);
                        return 3;
                    }
                    //recv
                    RecvBuf[index] = SPI_PDD_ReadData8bit(SPI1_BASE_PTR);
                    index ++;
                }
                //set cs
                hal_gpio_write_pin(MC33664_CS_TX_PORT, MC33664_CS_TX_PIN, 1);
            }
            break;
        default:return 4;
    }
    return 0;
}

/**
 * @brief :tx callback
 */
void
hal_daisychain_tx_callback(void)
{
}

/**
 * @brief :rx callback
 * Before use, ensure that the pin is in output mode
 */
void
hal_daisychain_rx_callback(void)
{
}

