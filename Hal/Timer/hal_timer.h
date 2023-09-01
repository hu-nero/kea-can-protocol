/*
 * hal_timer.h
 *
 *  Created on: 2023Äê8ÔÂ28ÈÕ
 *      Author: xinlei.hu
 */
#ifndef HAL_TIMER_HAL_TIMER_H_
#define HAL_TIMER_HAL_TIMER_H_
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    HAL_DEV_TIM0 = 0,
    HAL_DEV_TIM1,
    HAL_DEV_TIM_MAX_NUM
} HAL_DEV_TIM_Id;

typedef void (*halTimerCallbackFunc)(void);

extern uint16_t hal_timer_init(uint8_t Id);


/**
 * @brief :deinit timer
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_timer_deinit(uint8_t Id);

/**
 * @brief :enable timer
 *
 * @param Id :timer id
 *
 * @return :0 is success
 */
extern uint16_t hal_timer_enable(uint8_t Id);

/**
 * @brief :disable timer
 *
 * @param Id :timer id
 *
 * @return :0 is success
 */
extern uint16_t hal_timer_disable(uint8_t Id);

/**
 * @brief :set timing time
 *
 * @param Id :timer id
 * @param TimeUs :timeus is timing time
 *
 * @return :0 is set success;other is failed
 */
extern uint16_t hal_timer_timeus_set(uint8_t Id, uint16_t TimeUs);

/**
 * @brief :get timing time
 *
 * @param Id :timer id
 * @param TimeUs :timeus is timing time
 *
 * @return :0 is set success;other is failed
 */
extern uint16_t hal_timer_timeus_get(uint8_t Id, uint16_t *TimeUs);

/**
 * @brief :get run time
 */
extern uint32_t hal_timer_get_time(void);

/**
 * @brief :set timer0 callback
 */
extern void hal_timer0_callback_set(halTimerCallbackFunc Func);
/**
 * @brief :timer0 callback
 */
extern void hal_timer0_callback(void);


#endif /* HAL_TIMER_HAL_TIMER_H_ */
