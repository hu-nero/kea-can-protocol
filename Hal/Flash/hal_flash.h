/*
 * hal_flash.h
 *
 *  Created on: 2023Äê9ÔÂ22ÈÕ
 *      Author: xinlei.hu
 */

#ifndef HAL_FLASH_HAL_FLASH_H_
#define HAL_FLASH_HAL_FLASH_H_
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    HAL_DEV_FLASH0 = 0,
    HAL_DEV_FLASH1,
    HAL_DEV_FLASH_MAX_NUM
} HAL_DEV_FLASH_Enum;


extern uint16_t hal_flash_init(uint8_t Id);


/**
 * @brief :deinit pit
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_flash_deinit(uint8_t Id);

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
extern uint16_t hal_flash_read(uint8_t Id, uint32_t FromAddress, void *ToPtr, uint32_t Size);

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
extern uint16_t hal_flash_write(uint8_t Id, void *FromPtr, uint32_t ToAddress, uint32_t Size);

/**
 * @brief :erase flash
 *
 * @param Id
 * @param FromAddress
 * @param ToPtr
 * @param Size
 *
 * @return 
 */
extern uint16_t hal_flash_erase(uint8_t Id, uint32_t ToAddress, uint32_t Size);



#endif /* HAL_FLASH_HAL_FLASH_H_ */
