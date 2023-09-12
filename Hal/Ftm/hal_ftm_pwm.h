/*
 * hal_ftm.h
 *
 *  Created on: 2023Äê9ÔÂ11ÈÕ
 *      Author: xinlei.hu
 */

#ifndef HAL_FTM_HAL_FTM_H_
#define HAL_FTM_HAL_FTM_H_
#include <stdint.h>

#define HAL_FTM_CLK_FREQ       20000000UL//20M
#define HAL_FTM_CLK_CYCLE      0.05
                                          //
typedef enum
{
    HAL_DEV_FTM0 = 0,
    HAL_DEV_FTM1,
    HAL_DEV_FTM2,
    HAL_DEV_FTM_MAX_NUM
} HAL_DEV_FTM_Id;

/**
 * @brief :FTM timer init
 *
 * @param Id :FTM dev id
 * @return :0 is success;other is error
 */
extern uint16_t hal_ftm_pwm_init(uint8_t Id);


/**
 * @brief :deinit FTM
 *
 * @return :0 is success,1 is error
 */
extern uint16_t hal_ftm_pwm_deinit(uint8_t Id);

/**
 * @brief :enable FTM
 *
 * @param Id :FTM id
 *
 * @return :0 is success
 */
extern uint16_t hal_ftm_pwm_enable(uint8_t Id);

/**
 * @brief :disable FTM
 *
 * @param Id :FTM id
 *
 * @return :0 is success
 */
extern uint16_t hal_ftm_pwm_disable(uint8_t Id);

/**
 * @brief :get pwm Period
 *
 * @param Id:device id
 * @param PeriodUs:pointer of period,in union of us
 *
 * @return :0 is success
 */
extern uint16_t hal_ftm_pwm_period_get(uint8_t Id, uint16_t *PeriodUs);

/**
 * @brief :set pwm Period
 *
 * @param Id:device id
 * @param PeriodUs:period,in union of us
 *
 * @return :0 is success
 */
extern uint16_t hal_ftm_pwm_period_set(uint8_t Id, uint16_t PeriodUs);

/**
 * @brief :set pwm duty
 *
 * @param Id:device id
 * @param Duty:duty(0~100)
 *
 * @return :0 is success
 */
extern uint16_t hal_ftm_pwm_duty_set(uint8_t Id, uint16_t Duty);



#endif /* HAL_FTM_HAL_FTM_H_ */
