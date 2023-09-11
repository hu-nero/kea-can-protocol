/*
 * hal_wdog.c
 *
 *  Created on: 2023Äê9ÔÂ11ÈÕ
 *      Author: xinlei.hu
 */
#include "hal_wdog.h"
#include <stdbool.h>
#include "WDog.h"


static LDD_TDeviceData *halWDogDevicePtr = NULL;

/**
 * @brief :wdog init
 *
 * @param Id :wdog dev id
 * @return :0 is success;other is error
 */
uint16_t
hal_wdog_init(uint8_t Id)
{
    //init wdog device
    switch(Id)
    {
        case HAL_DEV_WDOG0:
            {
                halWDogDevicePtr = WDog_Init(NULL);
                if(NULL == halWDogDevicePtr)
                    return 1;
            }
            break;
        default:break;
    }
    return 0;
}


/**
 * @brief :deinit wdog
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_wdog_deinit(uint8_t Id)
{
    //init wdog device
    switch(Id)
    {
        case HAL_DEV_WDOG0:
            WDog_Deinit(NULL);
            break;
        default:break;
    }
    return 0;
}

/**
 * @brief :enable wdog
 *
 * @param Id :wdog id
 *
 * @return :0 is success
 */
uint16_t
hal_wdog_enable(uint8_t Id)
{
    switch(Id)
    {
        case HAL_DEV_WDOG0:
            {
                WDog_Enable(halWDogDevicePtr);
            }
            break;
        default:break;
    }
    return 0;
}

/**
 * @brief :disable wdog
 *
 * @param Id :wdog id
 *
 * @return :0 is success
 */
uint16_t
hal_wdog_disable(uint8_t Id)
{
    switch(Id)
    {
        case HAL_DEV_WDOG0:
            {
                WDog_Disable(halWDogDevicePtr);
            }
            break;
        default:break;
    }
    return 0;
}

/**
 * @brief :feed dog
 *
 * @param Id:wdog id
 *
 * @return :0 is success
 */
uint16_t
hal_wdog_feed(uint8_t Id)
{
    return WDog_Clear(halWDogDevicePtr);
}








