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

#define ADC_MEASURE_MAX_GROUP0      2u
#define ADC_MEASURE_MAX_GROUP1      4u
#define ADC_MEASURE_MAX_GROUP2      4u

#define ADC_CP_GROUP 				HAL_ADC_GROUP_0
#define ADC_HSD_GROUP 				HAL_ADC_GROUP_1
#define ADC_LSD_GROUP 				HAL_ADC_GROUP_2

#define ADC_CP_INDEX			    0u//group0 index0
#define ADC_PWR_INDEX			    1u//group0 index0

#define SERVICE_CP_ADC_R_VALUE          200000UL
#define SERVICE_CP_OUT_R_VALUE          1000000UL

extern void adc_measure_task_init(void);

/**
 * @brief :Collect a set of ad values for each run
 *
 * @return :0
 */
extern uint16_t adc_measure_task(void);
extern uint16_t adc_measure_task_vol_get(HAL_ADC_GROUP_Id Id, uint16_t * Value, uint8_t Len);
extern void adc_measure_task_vol_set(uint16_t ** ValuePtr);
extern uint16_t adc_measure_task_output_vol(uint16_t *Vol);

#endif /* TASK_ADC_MEASURE_TASK_ADC_MEASURE_H_ */
