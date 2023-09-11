/*
 * task_cp_simulate.h
 *
 *  Created on: 2023Äê9ÔÂ4ÈÕ
 *      Author: xinlei.hu
 */

#ifndef TASK_CP_SIMULATE_TASK_CP_SIMULATE_H_
#define TASK_CP_SIMULATE_TASK_CP_SIMULATE_H_
#include <stdint.h>

extern void cp_simulate_task_init(void);
extern uint16_t cp_simulate_task(void);

extern uint8_t cp_simulate_task_duty_get(void);
extern void cp_simulate_task_duty_set(uint8_t Duty);
extern uint16_t cp_simulate_task_freq_get(void);
extern void cp_simulate_task_freq_set(uint16_t Freq);

extern uint16_t cp_simulate_task_duty_freq_set(uint8_t DutyCycle, uint16_t Freq, uint8_t Mask, uint8_t *RealDuty, uint16_t *RealFreq);
extern uint16_t cp_simulate_task_Vol_get(uint32_t *Vol);

#endif /* TASK_CP_SIMULATE_TASK_CP_SIMULATE_H_ */
