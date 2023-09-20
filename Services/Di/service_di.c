/*
 * service_di.c
 *
 *  Created on: 2023Äê9ÔÂ20ÈÕ
 *      Author: xinlei.hu
 */
#include "service_di.h"
#include "board_config.h"
#include "board_pin_config.h"
#include "Hal.h"

void
di_service_input_io_init(void)
{
    //switch input io init
    hal_gpio_set_pin_dir(SIG_DI_0_PORT, SIG_DI_0_PIN, HAL_GPIO_INPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_DI_1_PORT, SIG_DI_1_PIN, HAL_GPIO_INPUT_DIRECTION, 0);
}


uint16_t
di_service_input_io_read(uint32_t *DI_0, uint32_t *DI_1)
{
    uint16_t res = 0;
    res = hal_gpio_read_pin(SIG_DI_0_PORT, SIG_DI_0_PIN, DI_0);
    res |= hal_gpio_read_pin(SIG_DI_1_PORT, SIG_DI_1_PIN, DI_1);
    return res;
}


