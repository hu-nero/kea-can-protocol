/*
 * task_cp_simulate.c
 *
 *  Created on: 2023年9月4日
 *      Author: xinlei.hu
 */
#include "task_cp_simulate.h"
#include "board_pin_config.h"
#include "Hal.h"
#include "board_config.h"

#include "Events.h"

static uint8_t gu8CpDutyCycle = 50;
static uint16_t gu16CpFreq = 100;//10ms cycle
static uint16_t gu16Cycle = 0;//us
static uint16_t gu16Delay = 0;//us

void
cp_simulate_task_init(void)
{
    hal_gpio_set_pin_dir(SIG_CP_IO_CTRL_PORT, SIG_CP_IO_CTRL_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_CP_ADC_PORT, SIG_CP_ADC_PIN, HAL_GPIO_INPUT_DIRECTION, 0);
    gu16Cycle = 1000000 * (double)1 / (double)gu16CpFreq;//us
    gu16Delay = (double)gu8CpDutyCycle * (double)gu16Cycle / (double)100;//us
}

uint16_t
cp_simulate_task(void)
{
    hal_gpio_write_pin(SIG_CP_IO_CTRL_PORT, SIG_CP_IO_CTRL_PIN, 1);
    hal_systick_delay_us((uint16_t)gu16Delay);
    hal_gpio_write_pin(SIG_CP_IO_CTRL_PORT, SIG_CP_IO_CTRL_PIN, 0);
    hal_systick_delay_us((uint16_t)gu16Cycle - (uint16_t)gu16Delay);
}

uint8_t
cp_simulate_task_duty_get(void)
{
    uint8_t u8Tmp = gu8CpDutyCycle;
    return u8Tmp;
}

void
cp_simulate_task_duty_set(uint8_t Duty)
{
    gu8CpDutyCycle = Duty;
    EnterCritical();
    gu16Cycle = 1000000 * (double)1 / (double)gu16CpFreq;//us
    gu16Delay = (double)gu8CpDutyCycle * (double)gu16Cycle / (double)100;//us
    ExitCritical();
}

uint16_t
cp_simulate_task_freq_get(void)
{
    return gu16CpFreq;
}

void
cp_simulate_task_freq_set(uint16_t Freq)
{
    gu16CpFreq = Freq;
    EnterCritical();
    gu16Cycle = 1000000 * (double)1 / (double)gu16CpFreq;//us
    gu16Delay = (double)gu8CpDutyCycle * (double)gu16Cycle / (double)100;//us
    ExitCritical();
}

uint16_t
cp_simulate_task_duty_freq_set(uint8_t DutyCycle, uint16_t Freq, uint8_t Mask, uint8_t *RealDuty, uint16_t *RealFreq)
{
    //占空比
    if (CAT_NUM_BIT(Mask, 0))
    {
        cp_simulate_task_duty_set(DutyCycle);
        *RealDuty = DutyCycle;
    }
    //频率
    if (CAT_NUM_BIT(Mask, 1))
    {
        cp_simulate_task_freq_set(Freq);
        *RealFreq = Freq;
    }
    return 0;
}

uint16_t
cp_simulate_task_Vol_get(uint32_t *Vol)
{
    uint16_t res = 0;
    res = hal_gpio_read_pin(SIG_CP_ADC_PORT, SIG_CP_ADC_PIN, Vol);
    return res;
}

