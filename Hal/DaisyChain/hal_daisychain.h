/*
 * hal_daisychain.h
 *
 *  Created on: 2023Äê2ÔÂ23ÈÕ
 *      Author: xinlei.hu
 */

#ifndef HAL_DAISYCHAIN_HAL_DAISYCHAIN_H_
#define HAL_DAISYCHAIN_HAL_DAISYCHAIN_H_

#include <stdint.h>
#include "hal_gpio.h"

#define MC33664_EN_PORT        HAL_GPIOB_PTG
#define MC33664_EN_PIN         HAL_GPIO_PIN3
#define MC33664_CLK_OE_PORT    HAL_GPIOB_PTG
#define MC33664_CLK_OE_PIN     HAL_GPIO_PIN1
#define MC33664_CS_TX_PORT     HAL_GPIOB_PTG
#define MC33664_CS_TX_PIN      HAL_GPIO_PIN2

#define ENABLE_MC33664_EN      hal_gpio_write_pin(MC33664_EN_PORT, MC33664_EN_PIN, 1);
#define DISABLE_MC33664_EN     hal_gpio_write_pin(MC33664_EN_PORT, MC33664_EN_PIN, 0);

#define ENABLE_MC33664_CLK_OE  hal_gpio_write_pin(MC33664_CLK_OE_PORT, MC33664_CLK_OE_PIN, 0);
#define DISABLE_MC33664_CLK_OE hal_gpio_write_pin(MC33664_CLK_OE_PORT, MC33664_CLK_OE_PIN, 1);

#define ENABLE_MC33664_CS_TX   hal_gpio_write_pin(MC33664_CS_TX_PORT, MC33664_CS_TX_PIN, 0);
#define DISABLE_MC33664_CS_TX  hal_gpio_write_pin(MC33664_CS_TX_PORT, MC33664_CS_TX_PIN, 1);

typedef enum
{
    HAL_DACH_SPI0 = 0,
    HAL_DACH_SPI1,
    HAL_DACH_SPI_MAX_NUM
} HAL_DACH_SPI_Id;

extern uint16_t hal_spi_daisychain_init(HAL_DACH_SPI_Id Id);
extern uint16_t hal_spi_daisychain_recv_data_blocking(HAL_DACH_SPI_Id Id, uint8_t *SendBuf, uint8_t *RecvBuf, uint16_t Len, uint32_t TimeoutMs);
extern uint16_t hal_spi_daisychain_transfer_data_blocking(HAL_DACH_SPI_Id Id, uint8_t *SendBuf, uint8_t *RecvBuf, uint16_t Len, uint32_t TimeoutMs);
extern void hal_daisychain_tx_callback(void);
extern void hal_daisychain_rx_callback(void);


#endif /* HAL_DAISYCHAIN_HAL_DAISYCHAIN_H_ */
