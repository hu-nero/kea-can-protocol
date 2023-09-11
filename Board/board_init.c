/*
 * board_init.c
 *
 *  Created on: 2023��8��29��
 *      Author: xinlei.hu
 */
#include "board_init.h"
#include "Hal.h"


void
board_init(void)
{
    //systick init
    hal_systick_init();
    //gpio init
    hal_gpio_init();
    //can init
    hal_can_init(eCanPort_0);
    //adc init
    hal_adc_init(HAL_DEV_ADC0);

}
