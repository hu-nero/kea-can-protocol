/*
 * hal_i2c.h
 *
 *  Created on: 2023Äê2ÔÂ9ÈÕ
 *      Author: xinlei.hu
 */

#ifndef HAL_I2C_HAL_I2C_H_
#define HAL_I2C_HAL_I2C_H_

#include <stdint.h>
#include <stdbool.h>

#define HAL_I2C_SEND_NUM_MAX            (64u)

typedef enum
{
    HAL_DEV_I2C0 = 0,
    HAL_DEV_I2C1,
    HAL_DEV_I2C_MAX_NUM
} HAL_DEV_I2C_Id;


typedef enum
{
  HAL_I2C_ADDRTYPE_7BITS,              /*!< 7 bits address */
  HAL_I2C_ADDRTYPE_10BITS,             /*!< 10 bits address */
  HAL_I2C_ADDRTYPE_GENERAL_CALL        /*!< General call address */
} HAL_I2C_ADDR_Type;

typedef enum
{
  HAL_I2C_NO_SEND_STOP,                /*!< Do not send stop signal */
  HAL_I2C_SEND_STOP                    /*!< Send stop signal */
} HAL_I2C_SendStop;

typedef struct
{
    HAL_DEV_I2C_Id devI2cId;
    uint16_t u16DevAddr;
    bool bIs10bitAddr;
} HAL_I2C_CLIENT_Struct;



/**
 * @brief :init i2c
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_i2c_init(HAL_I2C_CLIENT_Struct *Client);

/**
 * @brief :deinit i2c
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_i2c_deinit(HAL_I2C_CLIENT_Struct *Client);

/**
 * @brief :enable i2c
 *
 * @return :0 is success,other is error
 */
extern uint16_t hal_i2c_enable(HAL_I2C_CLIENT_Struct *Client);

/**
 * @brief :disable i2c
 *
 * @return :0 is success,other is error
 */
extern uint16_t hal_i2c_disable(HAL_I2C_CLIENT_Struct *Client);

/**
 * @brief :set slave device addr
 *
 * @param Id :I2C dev id
 * @param AddrType :7bit or 10 bit
 * @param Addr :dev addr
 *
 * @return :0 is success;other is error
 */
extern uint16_t hal_i2c_select_slave_device(HAL_DEV_I2C_Id Id, HAL_I2C_ADDR_Type AddrType,uint16_t Addr);

/**
 * @brief :i2c single register write
 *
 * @param Client :HAL_I2C_CLIENT_Struct
 * @param RegAddr :reg addr
 * @param Msg :data to write
 * @param Len :data length
 * @param TimeoutMs :blocking time
 *
 * @return :0 is success;other is error
 */
extern uint16_t hal_i2c_write_data_blocking(HAL_I2C_CLIENT_Struct *Client, uint8_t RegAddr, uint8_t *Msg, uint8_t Len, uint32_t TimeoutMs);

/**
 * @brief :i2c single register read
 *
 * @param Client :HAL_I2C_CLIENT_Struct
 * @param RegAddr :reg addr
 * @param Msg :storage value of read data
 * @param Len :data length
 * @param TimeoutMs :blocking time
 *
 * @return :0 is success;other is error
 */
extern uint16_t hal_i2c_read_data_blocking(HAL_I2C_CLIENT_Struct *Client, uint8_t RegAddr, uint8_t *Msg, uint8_t Len, uint32_t TimeoutMs);

/**
 * @brief :tx callback
 */
extern void hal_i2c_tx_callback(HAL_I2C_CLIENT_Struct *Client);
/**
 * @brief :rx callback
 */
extern void hal_i2c_rx_callback(HAL_I2C_CLIENT_Struct *Client);






#endif /* HAL_I2C_HAL_I2C_H_ */
