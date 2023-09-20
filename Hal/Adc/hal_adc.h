/*
 * hal_adc.h
 *
 *  Created on: 2023Äê3ÔÂ3ÈÕ
 *      Author: xinlei.hu
 */

#ifndef HAL_ADC_HAL_ADC_H_
#define HAL_ADC_HAL_ADC_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    HAL_DEV_ADC0 = 0,
    HAL_DEV_ADC_MAX_NUM
} HAL_DEV_ADC_Id;

typedef enum
{
    HAL_ADC_GROUP_0= 0,
	HAL_ADC_GROUP_1,
	HAL_ADC_GROUP_2,
    HAL_ADC_GROUP_MAX_NUM
} HAL_ADC_GROUP_Id;

/**
 * @brief :init adc
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_adc_init(HAL_DEV_ADC_Id Id);

/**
 * @brief :deinit adc
 *
 * @param Id :device id
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_adc_deinit(HAL_DEV_ADC_Id Id);

extern uint16_t hal_adc_operation_mode_set(void);
/**
 * @brief :start a group conversion
 *
 * @param Id :device id
 * @param Group :group index
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_adc_start_single_group_measurement(HAL_DEV_ADC_Id Id, uint16_t Group);

/**
 * @brief :get measure success flag
 *
 * @param Id :device id
 *
 * @return  :true, measurement finished;false, measurement is not finished
 */
extern bool hal_adc_get_measurement_flag(HAL_DEV_ADC_Id Id);

/**
 * @brief :get adc measurement value
 *
 * @param Id :device id
 * @param Buffer :storage value of measurement
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_adc_get_measurement_value(HAL_DEV_ADC_Id Id, uint16_t *Buffer);








#endif /* HAL_ADC_HAL_ADC_H_ */
