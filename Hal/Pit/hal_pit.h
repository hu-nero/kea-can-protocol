/*
 * hal_pit.h
 *
 *  Created on: 2023Äê6ÔÂ2ÈÕ
 *      Author: xinlei.hu
 */

#ifndef HAL_PIT_HAL_PIT_H_
#define HAL_PIT_HAL_PIT_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    HAL_DEV_PIT0 = 0,
    HAL_DEV_PIT1,
    HAL_DEV_PIT_MAX_NUM
} HAL_DEV_PIT_Id;

extern uint16_t hal_pit_init(uint8_t Id);


/**
 * @brief :deinit pit
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_pit_deinit(uint8_t Id);

/**
 * @brief :enable pit
 *
 * @param Id :pit id
 *
 * @return :0 is success
 */
extern uint16_t hal_pit_enable(uint8_t Id);

/**
 * @brief :disable pit
 *
 * @param Id :pit id
 *
 * @return :0 is success
 */
extern uint16_t hal_pit_disable(uint8_t Id);

/**
 * @brief :set timing time
 *
 * @param Id :pit id
 * @param TimeUs :timeus is timing time
 *
 * @return :0 is set success;other is failed
 */
extern uint16_t hal_pit_timeus_set(uint8_t Id, uint16_t TimeUs);

/**
 * @brief :get timing time
 *
 * @param Id :pit id
 * @param TimeUs :timeus is timing time
 *
 * @return :0 is set success;other is failed
 */
extern uint16_t hal_pit_timeus_get(uint8_t Id, uint16_t *TimeUs);

/**
 * @brief :get expired flag
 *
 * @return :expired flag
 */
extern bool hal_pit_expiredFlag_get(void);

/**
 * @brief :pit0 callback
 */
extern void hal_pit0_callback(void);

#endif /* HAL_PIT_HAL_PIT_H_ */
