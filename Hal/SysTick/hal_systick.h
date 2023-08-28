/*
 * hal_systick.h
 *
 *  Created on: 2023Äê2ÔÂ3ÈÕ
 *      Author: xinlei.hu
 */

#ifndef HAL_SYSTICK_HAL_SYSTICK_H_
#define HAL_SYSTICK_HAL_SYSTICK_H_

#include <stdint.h>

extern void hal_systick_init(void);
extern uint32_t hal_systick_get_counter(void);
extern void hal_systick_delay_us(unsigned long Nus);
extern void hal_systick_delay_ms(unsigned long Nms);

#endif /* HAL_SYSTICK_HAL_SYSTICK_H_ */
