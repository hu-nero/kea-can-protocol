/*
 * board_pin_config.h
 *
 *  Created on: 2023Äê2ÔÂ8ÈÕ
 *      Author: xinlei.hu
 */

#ifndef BOARD_BOARD_PIN_CONFIG_H_
#define BOARD_BOARD_PIN_CONFIG_H_

#include "Hal.h"

#define MCU_LED_G_PORT                             HAL_GPIOA_PTC
#define MCU_LED_G_PIN                              5u

#define MCU_RP_CTRL_PORT                           HAL_GPIOB_PTH
#define MCU_RP_CTRL_PIN                            0u

#define MCU_RN_CTRL_PORT                           HAL_GPIOB_PTE
#define MCU_RN_CTRL_PIN                            6u

#endif /* BOARD_BOARD_PIN_CONFIG_H_ */
