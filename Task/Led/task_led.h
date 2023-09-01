/*
 * task_led.h
 *
 *  Created on: 2023Äê8ÔÂ29ÈÕ
 *      Author: xinlei.hu
 */

#ifndef TASK_LED_TASK_LED_H_
#define TASK_LED_TASK_LED_H_
#include <stdint.h>

#define MCU_LED_G_PORT                             HAL_GPIOA_PTC
#define MCU_LED_G_PIN                              5u

#define MCU_LED0_PORT                              HAL_GPIOA_PTD
#define MCU_LED0_PIN                               1u

#define MCU_LED1_PORT                              HAL_GPIOA_PTD
#define MCU_LED1_PIN                               0u

extern void led_task_init(void);
extern uint16_t led_task(void);

#endif /* LED_TASK_LED_H_ */
