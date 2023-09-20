/*
 * service_cp.c
 *
 *  Created on: 2023年9月11日
 *      Author: xinlei.hu
 */
#include "service_cp.h"
#include "Hal.h"
#include "board_config.h"

static uint8_t gsu8CpDuty = 0;
static uint16_t gsu16CpFreq = 0;

void
cp_service_init(void)
{
    hal_ftm_pwm_init(HAL_DEV_FTM2);
    hal_ftm_pwm_duty_set(HAL_DEV_FTM2, 0);
}

void
cp_service_reset(void)
{
    cp_service_ctrl(0, 0, 0x03, NULL, NULL);
    gsu8CpDuty = 0;
    gsu16CpFreq = 0;
}

uint16_t
cp_service_ctrl(uint8_t Duty, uint16_t Freq, uint8_t Mask, uint8_t *RealDuty, uint16_t *RealFreq)
{
    uint16_t res = 0;

    if (RealDuty != NULL)
    	*RealDuty = gsu8CpDuty;
    if (RealFreq != NULL)
    	*RealFreq = gsu16CpFreq;
    for (int8_t bit=SERVICE_CP_PARAMETER_CHSIZE-1;bit>=0;bit--)
    {
        if (CAT_NUM_BIT(Mask, bit))
        {
            switch (bit)
            {
                case SERVICE_CP_SIMULATE_DUTY:
                    {
                        //只设置占空比，没有频率返回
                        if ((res = hal_ftm_pwm_duty_set(HAL_DEV_FTM2, Duty)) != 0)
                        {
                            return res;
                        }
                        if (RealDuty != NULL)
                            *RealDuty = Duty;
                        if (RealFreq != NULL)
                        {
                            if (CAT_NUM_BIT(Mask, 1) == 0)
                            {
                                *RealFreq = gsu16CpFreq;
                            }
                        }
                        gsu8CpDuty = Duty;
                    }
                    break;
                case SERVICE_CP_SIMULATE_FREQ:
                    {
                        uint16_t u16PeriodUs = 0;
                        if (Freq == 0)
                        {
                            if ((res = hal_ftm_pwm_period_set(HAL_DEV_FTM2, Freq)) != 0)
                            {
                                return res;
                            }
                        }
                        else
                        {
                            if ((res = hal_ftm_pwm_period_set(HAL_DEV_FTM2, (uint16_t)((uint32_t)1000000 / Freq))) != 0)
                            {
                                return res;
                            }
                        }
						if (CAT_NUM_BIT(Mask, 0) == 0)
						{
							if ((res = hal_ftm_pwm_duty_set(HAL_DEV_FTM2, gsu8CpDuty)) != 0)
							{
                                return res;
                            }
                            if (RealDuty != NULL)
                                *RealDuty = gsu8CpDuty;
                        }
                        if ((res = hal_ftm_pwm_period_get(HAL_DEV_FTM2, &u16PeriodUs)) != 0)
                        {
                            return res;
                        }
                        gsu16CpFreq = (uint16_t)((uint32_t)1000000 / u16PeriodUs);
                        if (RealFreq != NULL)
                            *RealFreq = gsu16CpFreq;
                    }
                    break;
                default:break;
            }
        }
    }
    return res;
}
