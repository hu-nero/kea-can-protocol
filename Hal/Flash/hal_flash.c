/*
 * hal_flash.c
 *
 *  Created on: 2023Äê9ÔÂ22ÈÕ
 *      Author: xinlei.hu
 */
#include "hal_flash.h"
#include <stdbool.h>
#include "FLASH.h"


static LDD_TDeviceData *halFlashDevicePtr = NULL;

/**
 * @brief :flash init
 *
 * @param Id :flash dev id
 * @return :0 is success;other is error
 */
uint16_t
hal_flash_init(uint8_t Id)
{
    //init flash device
    switch(Id)
    {
        case HAL_DEV_FLASH0:
            {
                halFlashDevicePtr = FLASH_Init(NULL);
                if(NULL == halFlashDevicePtr)
                    return 1;
            }
            break;
        case HAL_DEV_FLASH1:
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
hal_flash_deinit(uint8_t Id)
{
    //init flash device
    switch(Id)
    {
        case HAL_DEV_FLASH0:
            FLASH_Deinit(NULL);
            break;
        case HAL_DEV_FLASH1:
            {
                return 1;
            }
            break;
        default:break;
    }
    return 0;
}

/**
 * @brief :read flash
 *
 * @param Id
 * @param FromAddress
 * @param ToPtr
 * @param Size
 *
 * @return 
 */
uint16_t
hal_flash_read(uint8_t Id, uint32_t FromAddress, void *ToPtr, uint32_t Size)
{
    uint16_t res = 0;
    switch(Id)
    {
        case HAL_DEV_FLASH0:
            {
                res = FLASH_Read(halFlashDevicePtr, FromAddress, ToPtr, Size);
            }
            break;
        case HAL_DEV_FLASH1:
            {
                res = 1;
            }
            break;
        default:
            {
                res = 2;
            }
            break;
    }
    return res;
}

/**
 * @brief :write flash
 *
 * @param Id
 * @param FromAddress
 * @param ToPtr
 * @param Size
 *
 * @return 
 */
uint16_t
hal_flash_write(uint8_t Id, void *FromPtr, uint32_t ToAddress, uint32_t Size)
{
    uint16_t res = 0;
    switch(Id)
    {
        case HAL_DEV_FLASH0:
            {
                res = FLASH_Write(halFlashDevicePtr, FromPtr, ToAddress, Size);
            }
            break;
        case HAL_DEV_FLASH1:
            {
                res = 1;
            }
            break;
        default:
            {
                res = 2;
            }
            break;
    }
    return res;
}

/**
 * @brief :erase flash
 *
 * @param Id
 * @param FromAddress
 * @param ToPtr
 * @param Size:The first erased sector is a sector the address specified by the input parameter Address belongs to.
 * The last erased sector is a sector the address  calculated like an addition of the address specified by the input parameter Address
 * and the size specified by the input  parameter Size belongs to.
 *
 * @return 
 */
uint16_t
hal_flash_erase(uint8_t Id, uint32_t ToAddress, uint32_t Size)
{
    uint16_t res = 0;
    switch(Id)
    {
        case HAL_DEV_FLASH0:
            {
                res = FLASH_Erase(halFlashDevicePtr, ToAddress, Size);
            }
            break;
        case HAL_DEV_FLASH1:
            {
                res = 1;
            }
            break;
        default:
            {
                res = 2;
            }
            break;
    }
    return res;
}





