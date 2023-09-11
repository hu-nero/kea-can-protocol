/*
 * task_adc_measure.h
 *
 *  Created on: 2023Äê9ÔÂ5ÈÕ
 *      Author: xinlei.hu
 */

#ifndef TASK_ADC_MEASURE_TASK_ADC_MEASURE_H_
#define TASK_ADC_MEASURE_TASK_ADC_MEASURE_H_
#include <stdint.h>
#include "Hal.h"


#define ADC_VDDA_VOL                3300u
#define ADC_BIT_ACCURACY            4096u

#define ADC_MEASURE_GROUP_SIZE      3u
#define ADC_MEASURE_MAX_CH          4u

#define ADC_MEASURE_MAX_GROUP0      1u
#define ADC_MEASURE_MAX_GROUP1      4u
#define ADC_MEASURE_MAX_GROUP2      4u


extern void adc_measure_task_init(void);

/**
 * @brief :Collect a set of ad values for each run
 *
 * @return :0
 */
extern uint16_t adc_measure_task(void);

extern uint16_t* adc_measure_task_vol_get(HAL_ADC_GROUP_Id Id);
extern void adc_measure_task_vol_set(uint16_t ** ValuePtr);

#endif /* TASK_ADC_MEASURE_TASK_ADC_MEASURE_H_ */
