/*
 * task_led.h
 *
 *  Created on: 2023��8��29��
 *      Author: xinlei.hu
 */

#ifndef LED_TASK_LED_H_
#define LED_TASK_LED_H_
#include <stdint.h>

#define MCU_LED_G_PORT                             HAL_GPIOA_PTC
#define MCU_LED_G_PIN                              5u

#define MCU_RP_CTRL_PORT                           HAL_GPIOB_PTH
#define MCU_RP_CTRL_PIN                            0u

#define MCU_RN_CTRL_PORT                           HAL_GPIOB_PTE
#define MCU_RN_CTRL_PIN                            6u

extern void led_task_init(void);
extern uint16_t led_task(void);

#endif /* LED_TASK_LED_H_ */
