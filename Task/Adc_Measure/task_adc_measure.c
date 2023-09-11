/*
 * task_adc_measure.c
 *
 *  Created on: 2023Äê9ÔÂ5ÈÕ
 *      Author: xinlei.hu
 */
#include "task_adc_measure.h"
#include <string.h>

static uint16_t gu16AdcValue[ADC_MEASURE_GROUP_SIZE][ADC_MEASURE_MAX_CH] = {0};
static uint16_t gu16VolValue[ADC_MEASURE_GROUP_SIZE][ADC_MEASURE_MAX_CH] = {0};

static void adc_measure_task_vol_calculate(uint16_t* ADValue, uint16_t* VolValue, uint8_t Len);

const uint8_t cu8AdcGroupNum[ADC_MEASURE_GROUP_SIZE] = {ADC_MEASURE_MAX_GROUP0, ADC_MEASURE_MAX_GROUP1, ADC_MEASURE_MAX_GROUP2};

void
adc_measure_task_init(void)
{
}

/**
 * @brief :Collect a set of ad values for each run
 *
 * @return :0
 */
uint16_t
adc_measure_task(void)
{
    static uint8_t su8GroupIndex = HAL_ADC_GROUP_0;

    //sample
    hal_adc_start_single_group_measurement(HAL_DEV_ADC0, su8GroupIndex);
    while(!hal_adc_get_measurement_flag(HAL_DEV_ADC0));
    hal_adc_get_measurement_value(HAL_DEV_ADC0, gu16AdcValue[su8GroupIndex]);
    su8GroupIndex = (su8GroupIndex+1)%ADC_MEASURE_GROUP_SIZE;
    //calculate
    adc_measure_task_vol_calculate(gu16AdcValue[su8GroupIndex], gu16VolValue[su8GroupIndex], cu8AdcGroupNum[su8GroupIndex]);

    return 0;
}

uint16_t *
adc_measure_task_vol_get(HAL_ADC_GROUP_Id Id)
{
    static uint16_t u16VolValue[ADC_MEASURE_MAX_CH] = {0};
    if (Id >= HAL_ADC_GROUP_MAX_NUM)
    {
        return NULL;
    }
    memcpy(u16VolValue, gu16VolValue[Id], sizeof(u16VolValue));
    return u16VolValue;
}

void
adc_measure_task_vol_set(uint16_t ** ValuePtr)
{
    memcpy(gu16VolValue, ValuePtr, sizeof(gu16VolValue));
}

static void
adc_measure_task_vol_calculate(uint16_t* ADValue, uint16_t* VolValue, uint8_t Len)
{
    for (uint8_t index=0;index<Len;index++)
    {
        VolValue[index] = ADC_VDDA_VOL * ADValue[index] / ADC_BIT_ACCURACY;
    }
}

