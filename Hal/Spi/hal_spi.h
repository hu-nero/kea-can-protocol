/*
 * hal_spi.h
 *
 *  Created on: 2023Äê2ÔÂ21ÈÕ
 *      Author: xinlei.hu
 */

#ifndef HAL_SPI_HAL_SPI_H_
#define HAL_SPI_HAL_SPI_H_

#include <stdint.h>
#include <stdbool.h>
#include "SM1.h"
#include "SS0.h"
#include "SS1.h"
#include "hal_gpio.h"

typedef enum
{
    HAL_DEV_SPI0 = 0,
    HAL_DEV_SPI1,
    HAL_DEV_SPI_MAX_NUM
} HAL_DEV_SPI_Id;

typedef enum
{
    HAL_SLAVE_SPI_RECV_CMD = 0,
    HAL_SLAVE_SPI_RECV_RCMD,
    HAL_SLAVE_SPI_RECV_TCMD,
    HAL_SLAVE_SPI_RECV_DATA,
    HAL_SLAVE_SPI_SEND_DATA,
    HAL_SLAVE_SPI_UNDEFINED
} HAL_SLAVE_SPI_Enum;

typedef struct
{
    HAL_DEV_SPI_Id devSpiId;
    HAL_GPIO_PORT_Type gpioCsPort;
    HAL_GPIO_PIN_Type gpioCsPin;
} HAL_SPI_CLIENT_Struct;

extern uint8_t gsu8HalSpiRxDataBuf[20];
extern uint8_t hal_spi_slave_spi_recv_data_flag_get(void);
extern void hal_spi_slave_spi_recv_data_flag_set(uint8_t Flag);
/**
 * @brief :init spi
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_spi_init(uint8_t Id, HAL_GPIO_PORT_Type CsPort, HAL_GPIO_PIN_Type CsPin);
/**
 * @brief :init spi
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_spi_int_init(void);
/**
 * @brief :deinit spi
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_spi_deinit(uint8_t Id);
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
extern uint16_t hal_spi_master_transfer_data_blocking(uint8_t Id, const uint8_t *SendBuf, uint8_t *RecvBuf, uint16_t Len, uint32_t Timeout);
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
extern uint16_t hal_spi_slave_transfer_data_blocking(uint8_t Id, uint8_t *SendBuf, uint8_t *RecvBuf, uint16_t Len, uint32_t Timeout);

/**
 * @brief :spi master tx callback
 */
extern void hal_spi_master_tx_callback(void);
/**
 * @brief :spi master rx callback
 */
extern void hal_spi_master_rx_callback(void);
/**
 * @brief :spi slave tx callback
 */
extern void hal_spi_slave_tx_callback(void);
/**
 * @brief :spi slave rx callback
 */
extern void hal_spi_slave_rx_callback(void);
/**
 * @brief :spi slave rx callback
 */
extern void hal_spi_slave_cs_callback(void);

/**
 * @brief :analysis cmd
 *
 * @param Data :cmd data
 * @param Len :cmd len
 * @param TransmitLen :SPI transmit len
 *
 * @return :0 is success;other is failed
 */
extern uint8_t DMA_Data_CMD_Handle(uint8_t *Data, uint16_t Len);
#endif /* HAL_SPI_HAL_SPI_H_ */
