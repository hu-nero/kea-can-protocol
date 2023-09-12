/*
 * service_cp.c
 *
 *  Created on: 2023Äê9ÔÂ11ÈÕ
 *      Author: xinlei.hu
 */
#include "service_cp.h"
#include "Hal.h"
#include "board_config.h"


uint16_t
cp_service_ctrl(uint8_t Duty, uint16_t Freq, uint8_t Mask, uint8_t *RealDuty, uint16_t *RealFreq)
{
    uint16_t res = 0;
    for (uint8_t bit=0;bit<SERVICE_CP_PARAMETER_CHSIZE;bit++)
    {
        if (CAT_NUM_BIT(Mask, bit))
        {
            switch (bit)
            {
                case SERVICE_CP_SIMULATE_DUTY:
                    {
                        if ((res = hal_ftm_pwm_duty_set(HAL_DEV_FTM2, Duty)) != 0)
                        {
                            return res;
                        }
                    }
                    break;
                case SERVICE_CP_SIMULATE_FREQ:
                    {
                        uint16_t u16PeriodUs = 0;
                        if ((res = hal_ftm_pwm_period_set(HAL_DEV_FTM2, (uint16_t)((uint32_t)1000000 / Freq))) != 0)
                        {
                            return res;
                        }
                        if ((res = hal_ftm_pwm_period_get(HAL_DEV_FTM2, &u16PeriodUs)) != 0)
                        {
                            return res;
                        }
                        *RealFreq = (uint16_t)((uint32_t)1000000 / u16PeriodUs);
                    }
                    break;
                default:break;
            }
        }
    }
}


