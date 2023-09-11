/*
 * task_led.c
 *
 *  Created on: 2023Äê8ÔÂ29ÈÕ
 *      Author: xinlei.hu
 */
#include "task_led.h"
#include "board_pin_config.h"
#include "Hal.h"


void
led_task_init(void)
{
    //LED0 LED1 init
    hal_gpio_set_pin_dir(MCU_LED0_PORT, MCU_LED0_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(MCU_LED1_PORT, MCU_LED1_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
}

uint16_t
led_task(void)
{
    hal_gpio_toggle_pin(MCU_LED0_PORT, MCU_LED0_PIN);
    hal_gpio_toggle_pin(MCU_LED1_PORT, MCU_LED1_PIN);
    return 0;
}




