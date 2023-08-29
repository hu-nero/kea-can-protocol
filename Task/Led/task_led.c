/*
 * task_led.c
 *
 *  Created on: 2023Äê8ÔÂ29ÈÕ
 *      Author: xinlei.hu
 */
#include "task_led.h"
#include "Hal.h"


void
led_task_init(void)
{
  //LED Init
  hal_gpio_set_pin_dir(MCU_LED_G_PORT, MCU_LED_G_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
}

uint16_t
led_task(void)
{
//    while(1)
//    {
      hal_gpio_toggle_pin(MCU_LED_G_PORT, MCU_LED_G_PIN);
//            hal_systick_delay_ms(500);
//    }
}




