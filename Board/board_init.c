/*
 * board_init.c
 *
 *  Created on: 2023Äê8ÔÂ29ÈÕ
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
    //timer init
    hal_timer_init(HAL_DEV_TIM0);
    //can init
    hal_can_init(eCanPort_0);

}
