/*
 * hal_adc.c
 *
 *  Created on: 2023Äê3ÔÂ3ÈÕ
 *      Author: xinlei.hu
 */
#include "hal_adc.h"
#include "AD.h"

static LDD_TDeviceData *halAdc0DevicePtr = NULL;

/**
 * @brief :init adc
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_adc_init(HAL_DEV_ADC_Id Id)
{
    //init i2c device
    switch(Id)
    {
        case HAL_DEV_ADC0:
            {
                halAdc0DevicePtr = AD_Init(NULL);
                if(NULL == halAdc0DevicePtr)
                    return 1;
            }
            break;
        default:return 1;
    }
    return 0;
}

/**
 * @brief :deinit adc
 *
 * @param Id :device id
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_adc_deinit(HAL_DEV_ADC_Id Id)
{
    switch(Id)
    {
        case HAL_DEV_ADC0:
                AD_Deinit(NULL);
            break;
        default:return 1;
    }
    return 0;
}

uint16_t
hal_adc_operation_mode_set(void)
{
    AD_SetOperationMode(halAdc0DevicePtr, DOM_RUN, NULL, NULL);
    return 0;
}

/**
 * @brief :start a group conversion
 *
 * @param Id :device id
 * @param Group :group index
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_adc_start_single_group_measurement(HAL_DEV_ADC_Id Id, uint16_t Group)
{
    uint16_t u16Err;
    switch(Id)
    {
        case HAL_DEV_ADC0:
            {
                u16Err = AD_SelectSampleGroup(halAdc0DevicePtr, Group);
                if(u16Err != 0)
                {
                    return 1;
                }
                u16Err = AD_StartSingleMeasurement(halAdc0DevicePtr);
                if(u16Err != 0)
                {
                    return 2;
                }
            }
            break;
        default:return 3;
    }
    return 0;
}

/**
 * @brief :get measure success flag
 *
 * @param Id :device id
 *
 * @return  :true, measurement finished;false, measurement is not finished
 */
bool
hal_adc_get_measurement_flag(HAL_DEV_ADC_Id Id)
{
    switch(Id)
    {
        case HAL_DEV_ADC0:
            {
                return AD_GetMeasurementCompleteStatus(halAdc0DevicePtr);
            }
            break;
        default:break;
    }
    return false;
}

/**
 * @brief :get adc measurement value
 *
 * @param Id :device id
 * @param Buffer :storage value of measurement
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_adc_get_measurement_value(HAL_DEV_ADC_Id Id, uint16_t *Buffer)
{
    uint16_t u16Err = 1;
    switch(Id)
    {
        case HAL_DEV_ADC0:
            {
                u16Err = AD_GetMeasuredValues(halAdc0DevicePtr, Buffer);
            }
            break;
        default:break;
    }
    return u16Err;
}






