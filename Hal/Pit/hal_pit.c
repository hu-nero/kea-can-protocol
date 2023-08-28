/*
 * hal_pit.c
 *
 *  Created on: 2023Äê6ÔÂ2ÈÕ
 *      Author: xinlei.hu
 */
#include "hal_pit.h"
#include <stdbool.h>
#include "PIT0.h"


static LDD_TDeviceData *halPit0DevicePtr = NULL;
static bool s_timeoutExpired = false;

/**
 * @brief :pit timer init
 *
 * @param Id :pit dev id
 * @return :0 is success;other is error
 */
uint16_t
hal_pit_init(uint8_t Id)
{
    //init pit timer device
    switch(Id)
    {
        case HAL_DEV_PIT0:
            {
                halPit0DevicePtr = PIT0_Init(NULL);
                if(NULL == halPit0DevicePtr)
                    return 1;
            }
            break;
        case HAL_DEV_PIT1:
            {
                    return 2;
            }
            break;
        default:break;
    }
    return 0;
}


/**
 * @brief :deinit pit
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_pit_deinit(uint8_t Id)
{
    //init i2c device
    switch(Id)
    {
        case HAL_DEV_PIT0:
            PIT0_Deinit(NULL);
            break;
        case HAL_DEV_PIT1:
            {
                return 1;
            }
            break;
        default:break;
    }
    return 0;
}

/**
 * @brief :enable pit
 *
 * @param Id :pit id
 *
 * @return :0 is success
 */
uint16_t
hal_pit_enable(uint8_t Id)
{
    switch(Id)
    {
        case HAL_DEV_PIT0:
            {
                PIT0_Enable(halPit0DevicePtr);
            }
            break;
        case HAL_DEV_PIT1:
            {
                return 1;
            }
            break;
        default:break;
    }
    return 0;
}

/**
 * @brief :disable pit
 *
 * @param Id :pit id
 *
 * @return :0 is success
 */
uint16_t
hal_pit_disable(uint8_t Id)
{
    switch(Id)
    {
        case HAL_DEV_PIT0:
            {
                PIT0_Disable(halPit0DevicePtr);
            }
            break;
        case HAL_DEV_PIT1:
            {
                return 1;
            }
            break;
        default:break;
    }
    return 0;
}

/**
 * @brief :set timing time
 *
 * @param Id :pit id
 * @param TimeUs :timeus is timing time
 *
 * @return :0 is set success;other is failed
 */
uint16_t
hal_pit_timeus_set(uint8_t Id, uint16_t TimeUs)
{
    uint32_t u32Tick = 0;
    u32Tick = 0xFFFFFFFF - 20*TimeUs;//20M fre,1 tick=0.05us,20tick=1us
    switch(Id)
    {
        case HAL_DEV_PIT0:
            {
                PIT0_SetPeriodTicks(halPit0DevicePtr, u32Tick);
            }
            break;
        case HAL_DEV_PIT1:
            {
                return 1;
            }
            break;
        default:break;
    }
    return 0;
}

/**
 * @brief :get timing time
 *
 * @param Id :pit id
 * @param TimeUs :timeus is timing time
 *
 * @return :0 is set success;other is failed
 */
uint16_t
hal_pit_timeus_get(uint8_t Id, uint16_t *TimeUs)
{
    uint32_t u32Tick = 0;
    switch(Id)
    {
        case HAL_DEV_PIT0:
            {
                PIT0_GetPeriodTicks(halPit0DevicePtr, &u32Tick);
            }
            break;
        case HAL_DEV_PIT1:
            {
                return 1;
            }
            break;
        default:break;
    }
    *TimeUs = (0xFFFFFFFF - u32Tick)/20;
    return 0;
}

/**
 * @brief :get expired flag
 *
 * @return :expired flag
 */
bool
hal_pit_expiredFlag_get(void)
{
    return s_timeoutExpired;
}

/**
 * @brief :pit0 callback
 */
void
hal_pit0_callback(void)
{
    s_timeoutExpired = true;
    hal_pit_disable(HAL_DEV_PIT0);
}








