/*
 * hal_flash.c
 *
 *  Created on: 2023Äê9ÔÂ22ÈÕ
 *      Author: xinlei.hu
 */
#include "hal_flash.h"
#include <stdbool.h>
#include "FlashOperate.h"


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
            	FlashInit();
            	FlashDataInit();
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
                res = FlashWritePageOperate(ToAddress, FromPtr, Size);
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
 * @param Size:block size
 *
 * @return 
 */
uint16_t
hal_flash_erase(uint8_t Id, uint32_t FromAddress)
{
    uint16_t res = 0;
    switch(Id)
    {
        case HAL_DEV_FLASH0:
            {
                res = FlashEraseectorsOperate(FromAddress);
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
 * @brief :erase flash block
 *
 * @param Id
 * @param FromAddress
 * @param ToPtr
 * @param Size
 *
 * @return 
 */
uint16_t
hal_flash_erase_block(uint8_t Id, uint32_t FromAddress)
{
    uint16_t res = 0;
    switch(Id)
    {
        case HAL_DEV_FLASH0:
            {
                res = FlashEraseBlockOperate(FromAddress);
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



