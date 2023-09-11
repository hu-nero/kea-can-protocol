/*
 * hal_wdog.h
 *
 *  Created on: 2023Äê9ÔÂ11ÈÕ
 *      Author: xinlei.hu
 */

#ifndef HAL_WDOG_HAL_WDOG_H_
#define HAL_WDOG_HAL_WDOG_H_
#include <stdint.h>

typedef enum
{
    HAL_DEV_WDOG0 = 0,
    HAL_DEV_WDOG_MAX_NUM
} HAL_DEV_WDOG_Id;

extern uint16_t hal_wdog_init(uint8_t Id);


/**
 * @brief :deinit wdog
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_wdog_deinit(uint8_t Id);

/**
 * @brief :enable wdog
 *
 * @param Id :wdog id
 *
 * @return :0 is success
 */
extern uint16_t hal_wdog_enable(uint8_t Id);

/**
 * @brief :disable wdog
 *
 * @param Id :wdog id
 *
 * @return :0 is success
 */
extern uint16_t hal_wdog_disable(uint8_t Id);

/**
 * @brief :feed dog
 *
 * @param Id:wdog id
 *
 * @return :0 is success
 */
extern uint16_t hal_wdog_feed(uint8_t Id);









#endif /* HAL_WDOG_HAL_WDOG_H_ */
