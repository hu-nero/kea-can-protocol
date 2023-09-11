/*
 * task_wdog.c
 *
 *  Created on: 2023Äê9ÔÂ11ÈÕ
 *      Author: xinlei.hu
 */
#include "task_wdog.h"
#include "Hal.h"


void
wdog_task_init(void)
{
	//wdog init
	hal_wdog_init(HAL_DEV_WDOG0);
}

uint16_t
wdog_task(void)
{
    hal_wdog_feed(HAL_DEV_WDOG0);
    return 0;
}








