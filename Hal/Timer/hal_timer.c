/*
 * hal_timer.c
 *
 *  Created on: 2023Äê8ÔÂ28ÈÕ
 *      Author: xinlei.hu
 */
#include "hal_timer.h"
#include "TaskTimer.h"
#include "task.h"

static LDD_TDeviceData *halTimer0DevicePtr = NULL;
halTimerCallbackFunc ghalTimerTaskTimerCallback = NULL;
static uint32_t gu32TimerCounter = 0;
static uint32_t gu32TimerRunTime = 0;

/**
 * @brief :timer init
 *
 * @param Id :timer dev id
 * @return :0 is success;other is error
 */
uint16_t
hal_timer_init(uint8_t Id)
{
    //init timer device
    switch(Id)
    {
        case HAL_DEV_TIM0:
            {
                halTimer0DevicePtr = TaskTimer_Init(NULL);
                if(NULL == halTimer0DevicePtr)
                    return 1;
            }
            break;
        case HAL_DEV_TIM1:
            {
                return 2;
            }
            break;
        default:return 3;
    }
    return 0;
}


/**
 * @brief :deinit timer
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_timer_deinit(uint8_t Id)
{
    //init timer device
    switch(Id)
    {
        case HAL_DEV_TIM0:
            TaskTimer_Deinit(NULL);
            break;
        case HAL_DEV_TIM1:
            {
                return 1;
            }
            break;
        default:return 2;
    }
    return 0;
}

/**
 * @brief :enable timer
 *
 * @param Id :timer id
 *
 * @return :0 is success
 */
uint16_t
hal_timer_enable(uint8_t Id)
{
    switch(Id)
    {
        case HAL_DEV_TIM0:
            {
                TaskTimer_Enable(halTimer0DevicePtr);
            }
            break;
        case HAL_DEV_TIM1:
            {
                return 1;
            }
            break;
        default:return 2;
    }
    return 0;
}

/**
 * @brief :disable timer
 *
 * @param Id :timer id
 *
 * @return :0 is success
 */
uint16_t
hal_timer_disable(uint8_t Id)
{
    switch(Id)
    {
        case HAL_DEV_TIM0:
            {
                TaskTimer_Disable(halTimer0DevicePtr);
            }
            break;
        case HAL_DEV_TIM1:
            {
                return 1;
            }
            break;
        default:return 2;
    }
    return 0;
}

/**
 * @brief :set timing time
 *
 * @param Id :timer id
 * @param TimeUs :timeus is timing time
 *
 * @return :0 is set success;other is failed
 */
uint16_t
hal_timer_timeus_set(uint8_t Id, uint16_t TimeUs)
{
    uint32_t u32Tick = 0;
    u32Tick = TimeUs/0.05;//20M fre,1 tick=0.05us,20tick=1us
    switch(Id)
    {
        case HAL_DEV_TIM0:
            {
                TaskTimer_SetPeriodTicks(halTimer0DevicePtr, u32Tick);
            }
            break;
        case HAL_DEV_TIM1:
            {
                return 1;
            }
            break;
        default:return 2;
    }
    return 0;
}

/**
 * @brief :get timing time
 *
 * @param Id :timer id
 * @param TimeUs :timeus is timing time
 *
 * @return :0 is set success;other is failed
 */
uint16_t
hal_timer_timeus_get(uint8_t Id, uint16_t *TimeUs)
{
    uint32_t u32Tick = 0;
    switch(Id)
    {
        case HAL_DEV_TIM0:
            {
                TaskTimer_GetPeriodTicks(halTimer0DevicePtr, &u32Tick);
            }
            break;
        case HAL_DEV_TIM1:
            {
                return 1;
            }
            break;
        default:return 2;
    }
    *TimeUs = u32Tick*0.05;
    return 0;
}

/**
 * @brief :get run time
 */
uint32_t
hal_timer_get_time(void)
{
    return gu32TimerRunTime;
}

/**
 * @brief :set timer0 callback
 */
void
hal_timer0_callback_set(halTimerCallbackFunc Func)
{
    ghalTimerTaskTimerCallback = Func;
}
/**
 * @brief :timer0 callback
 */
void
hal_timer0_callback(void)
{
    gu32TimerCounter ++;
    if (gu32TimerCounter == 1000)
    {
        gu32TimerCounter = 0;
        gu32TimerRunTime ++;
    }
    ghalTimerTaskTimerCallback();
}









