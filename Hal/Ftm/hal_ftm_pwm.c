/*
 * hal_ftm_pwm.c
 *
 *  Created on: 2023Äê9ÔÂ11ÈÕ
 *      Author: xinlei.hu
 */
#include "hal_ftm_pwm.h"
#include "CP_TIM.h"
#include "CP_PWM.h"

static LDD_TDeviceData *halTIMDevicePtr = NULL;
static LDD_TDeviceData *halPWMDevicePtr = NULL;

/**
 * @brief :FTM timer init
 *
 * @param Id :FTM dev id
 * @return :0 is success;other is error
 */
uint16_t
hal_ftm_pwm_init(uint8_t Id)
{
    uint16_t res = 0;
    //init FTM timer device
    switch(Id)
    {
        case HAL_DEV_FTM0:
            {
                res = 1;
            }
            break;
        case HAL_DEV_FTM1:
            {
                res = 2;
            }
            break;
        case HAL_DEV_FTM2:
            {
                halTIMDevicePtr = CP_TIM_Init(NULL);
                if(NULL == halTIMDevicePtr)
                    res = 3;
                halPWMDevicePtr = CP_PWM_Init(NULL);
                if(NULL == halPWMDevicePtr)
                    res = 4;
            }
            break;
        default:
            {
                res = 5;
            }
            break;
    }
    return res;
}


/**
 * @brief :deinit FTM
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_ftm_pwm_deinit(uint8_t Id)
{
    uint16_t res = 0;
    //init FTM timer device
    switch(Id)
    {
        case HAL_DEV_FTM0:
            {
                res = 1;
            }
            break;
        case HAL_DEV_FTM1:
            {
                res = 2;
            }
            break;
        case HAL_DEV_FTM2:
            {
                CP_TIM_Deinit(NULL);
                CP_PWM_Init(NULL);
            }
            break;
        default:
            {
                res = 3;
            }
            break;
    }
    return res;
}

/**
 * @brief :enable FTM
 *
 * @param Id :FTM id
 *
 * @return :0 is success
 */
uint16_t
hal_ftm_pwm_enable(uint8_t Id)
{
    uint16_t res = 0;
    switch(Id)
    {
        case HAL_DEV_FTM0:
            {
                res = 1;
            }
            break;
        case HAL_DEV_FTM1:
            {
                res = 2;
            }
            break;
        case HAL_DEV_FTM2:
            {
                CP_TIM_Enable(halTIMDevicePtr);
                CP_PWM_Enable(halPWMDevicePtr);
            }
            break;
        default:
            {
                res = 3;
            }
            break;
    }
    return res;
}

/**
 * @brief :disable FTM
 *
 * @param Id :FTM id
 *
 * @return :0 is success
 */
uint16_t
hal_ftm_pwm_disable(uint8_t Id)
{
    uint16_t res = 0;
    switch(Id)
    {
        case HAL_DEV_FTM0:
            {
                res = 1;
            }
            break;
        case HAL_DEV_FTM1:
            {
                res = 2;
            }
            break;
        case HAL_DEV_FTM2:
            {
                CP_TIM_Disable(halTIMDevicePtr);
                CP_PWM_Disable(halPWMDevicePtr);
            }
            break;
        default:
            {
                res = 3;
            }
            break;
    }
    return res;
}

/**
 * @brief :get pwm Period
 *
 * @param Id:device id
 * @param PeriodUs:pointer of period,in union of us
 *
 * @return :0 is success
 */
uint16_t
hal_ftm_pwm_period_get(uint8_t Id, uint16_t *PeriodUs)
{
    uint16_t res = 0;
    uint16_t u16PeriodTick = 0;
    res = CP_TIM_GetPeriodTicks(halTIMDevicePtr, &u16PeriodTick);
    *PeriodUs = u16PeriodTick * HAL_FTM_CLK_CYCLE;
    return res;
}

/**
 * @brief :set pwm Period
 *
 * @param Id:device id
 * @param PeriodUs:period,in union of us
 *
 * @return :0 is success
 */
uint16_t
hal_ftm_pwm_period_set(uint8_t Id, uint16_t PeriodUs)
{
    uint16_t u16PeriodTick = 0;
    u16PeriodTick = PeriodUs / HAL_FTM_CLK_CYCLE;
    return CP_TIM_SetPeriodTicks(halTIMDevicePtr, u16PeriodTick);
}

/**
 * @brief :set pwm duty
 *
 * @param Id:device id
 * @param Duty:duty(0~100)
 *
 * @return :0 is success
 */
uint16_t
hal_ftm_pwm_duty_set(uint8_t Id, uint16_t Duty)
{
    if (Duty > 100)
    {
        return 1;
    }
    return CP_PWM_SetRatio16(halPWMDevicePtr, Duty * 0xFFFFu / 100);
}











