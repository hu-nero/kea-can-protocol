/*
 * board_pin_config.c
 *
 *  Created on: 2023年9月1日
 *      Author: xinlei.hu
 */
#include "board_pin_config.h"
#include "board_config.h"
#include "Hal.h"


void
board_pin_switch_output_ctrl_init(void)
{
    //switch output io init
    hal_gpio_set_pin_dir(SIG_INTERLOCK_PORT, SIG_INTERLOCK_PORT, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_KL15_PORT, SIG_KL15_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_DO_0_PORT, SIG_DO_0_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_DO_1_PORT, SIG_DO_1_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
}

void
board_pin_ntc_io_init(void)
{
    //NTC io ctrl init
    hal_gpio_set_pin_dir(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
}

void
board_pin_cc_io_init(void)
{
    //CC io ctrl init
    hal_gpio_set_pin_dir(SIG_CC2_IO_PORT, SIG_CC2_IO_PIN  , HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_CC_IO_0_PORT, SIG_CC_IO_0_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_CC_IO_1_PORT, SIG_CC_IO_1_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_CC_IO_2_PORT, SIG_CC_IO_2_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_CC_IO_3_PORT, SIG_CC_IO_3_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_CC_IO_4_PORT, SIG_CC_IO_4_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_CC_IO_5_PORT, SIG_CC_IO_5_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_CC_IO_6_PORT, SIG_CC_IO_6_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_CC_IO_7_PORT, SIG_CC_IO_7_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
}

void
board_pin_input_di_init(void)
{
    //switch input io init
    hal_gpio_set_pin_dir(SIG_DI_0_PORT, SIG_DI_0_PIN, HAL_GPIO_INPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_DI_1_PORT, SIG_DI_1_PIN, HAL_GPIO_INPUT_DIRECTION, 0);
}

uint16_t
board_pin_switch_output_ctrl(uint8_t BitState, uint8_t Mask, uint32_t *Value)
{
    uint16_t res = 0;
    uint32_t u32Value = 0;
    for (uint8_t bit=0;bit<BOAED_PIN_SWITCH_CTRL_CHSIZE;bit++)
    {
        if (CAT_NUM_BIT(Mask, bit))
        {
            switch (bit)
            {
                case BOARD_PIN_SWITCH_CTRL_INTERLOCK_IO:
                    {
                        hal_gpio_write_pin(SIG_INTERLOCK_PORT, SIG_INTERLOCK_PORT, CAT_NUM_BIT(BitState, bit));
                        if ((res = hal_gpio_get_pin_output_value(SIG_INTERLOCK_PORT, SIG_INTERLOCK_PORT, &u32Value)) != 0)
                        {
                            return res;
                        }
                        if (u32Value)
                        {
                            SET_NUM_BIT(*Value, bit);
                        }
                        else
                        {
                            RESET_NUM_BIT(*Value, bit);
                        }
                    }
                    break;
                case BOARD_PIN_SWITCH_CTRL_KL15:
                    {
                        hal_gpio_write_pin(SIG_KL15_PORT, SIG_KL15_PIN, CAT_NUM_BIT(BitState, bit));
                        if ((res = hal_gpio_get_pin_output_value(SIG_KL15_PORT, SIG_KL15_PIN, &u32Value)) != 0)
                        {
                            return res;
                        }
                        if (u32Value)
                        {
                            SET_NUM_BIT(*Value, bit);
                        }
                        else
                        {
                            RESET_NUM_BIT(*Value, bit);
                        }
                    }
                    break;
                case BOARD_PIN_SWITCH_CTRL_DO_IO_0:
                    {
                        hal_gpio_write_pin(SIG_DO_0_PORT, SIG_DO_0_PIN, CAT_NUM_BIT(BitState, bit));
                        if ((res = hal_gpio_get_pin_output_value(SIG_DO_0_PORT, SIG_DO_0_PIN, &u32Value)) != 0)
                        {
                            return res;
                        }
                        if (u32Value)
                        {
                            SET_NUM_BIT(*Value, bit);
                        }
                        else
                        {
                            RESET_NUM_BIT(*Value, bit);
                        }
                    }
                    break;
                case BOARD_PIN_SWITCH_CTRL_DO_IO_1:
                    {
                        hal_gpio_write_pin(SIG_DO_1_PORT, SIG_DO_1_PIN, CAT_NUM_BIT(BitState, bit));
                        if ((res = hal_gpio_get_pin_output_value(SIG_DO_1_PORT, SIG_DO_1_PIN, &u32Value)) != 0)
                        {
                            return res;
                        }
                        if (u32Value)
                        {
                            SET_NUM_BIT(*Value, bit);
                        }
                        else
                        {
                            RESET_NUM_BIT(*Value, bit);
                        }
                    }
                    break;
                default:break;
            }
        }
    }
    return res;
}

uint16_t
board_pin_ntc_r_ctrl(uint16_t NTC_R, uint8_t Ch, uint16_t *RValue, uint8_t *ChState)
{
    //按bit设置电阻
    for (uint8_t bit=0;bit<4;bit++)
    {
        if (CAT_NUM_BIT(NTC_R, bit))
        {
            if (bit == 0)
                hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 1);
            if (bit == 1)
                hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 1);
            if (bit == 2)
                hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 1);
            if (bit == 3)
                hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 1);
        }
        else
        {
            if (bit == 0)
                hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 0);
            if (bit == 1)
                hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 0);
            if (bit == 2)
                hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 0);
            if (bit == 3)
                hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 0);
        }
    }
    
#if 0
    uint16_t res = 0;
    uint32_t u32Value = 0;
    //RL_0和RL_2
    if (CAT_NUM_BIT(Ch, 0))
    {
        hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 1);
        res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value);
        {
            return res;
        }
        if (u32Value)
        {
            SET_NUM_BIT(*ChState, 0);
            //RL_0=1,RL_2=1
            if (CAT_NUM_BIT(Ch, 2))
            {
                hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 1);
                res = hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value);
                if (res == 0)
                {
                    if (u32Value)
                    {
                        SET_NUM_BIT(*ChState, 2);
                        *RValue = SIG_NTC_R_2_CLOSE_VALUE;
                    }
                    else
                    {
                        res = 3;
                    }
                }
            }
            //RL_0=1,RL_2=0
            else
            {
                hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_2_PIN, 0);
                res = hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value);
                if (res == 0)
                {
                    if (u32Value)
                    {
                        res = 3;
                    }
                    else
                    {
                        RESET_NUM_BIT(*ChState, 2);
                        *RValue = SIG_NTC_R_2_OPEN_VALUE;
                    }
                }
            }
        }
        else
        {
            RESET_NUM_BIT(*ChState, 0);
            res = 4;
        }
    }
    //RL_1和RL_3
    if (CAT_NUM_BIT(Ch, 1))
    {
        hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 1);
        res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value);
        {
            return res;
        }
        if (u32Value)
        {
            SET_NUM_BIT(*ChState, 1);
            //RL_1=1,RL_3=1
            if (CAT_NUM_BIT(Ch, 2))
            {
                hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 1);
                res = hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value);
                if (res == 0)
                {
                    if (u32Value)
                    {
                        SET_NUM_BIT(*ChState, 3);
                        *RValue = SIG_NTC_R_3_CLOSE_VALUE;
                    }
                    else
                    {
                        res = 3;
                    }
                }
            }
            //RL_1=1,RL_3=0
            else
            {
                hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 0);
                res = hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value);
                if (res == 0)
                {
                    if (u32Value)
                    {
                        res = 3;
                    }
                    else
                    {
                        RESET_NUM_BIT(*ChState, 3);
                        *RValue = SIG_NTC_R_3_OPEN_VALUE;
                    }
                }
            }
        }
        else
        {
            RESET_NUM_BIT(*ChState, 1);
            res = 4;
        }
    }
    return res;
#endif
}

uint16_t
board_pin_cc_r_ctrl(uint16_t CC_R, uint8_t CC2, uint8_t Mask, uint16_t *RValue, uint8_t *CC2State)
{
    uint16_t res = 0;
    uint32_t u32Value = 0;
    if (CAT_NUM_BIT(Mask, 0))
    {
        //CC resistance operation
        //r0= 51;r0(40.8,61.2)
        if ((CC_R >= (SIG_CC_R_0_VALUE-0.2*SIG_CC_R_0_VALUE)) && (CC_R <= (SIG_CC_R_0_VALUE+0.2*SIG_CC_R_0_VALUE)))
        {
            hal_gpio_write_pin(SIG_CC_IO_0_PORT, SIG_CC_IO_0_PIN, 1);
            res = hal_gpio_get_pin_output_value(SIG_CC_IO_0_PORT, SIG_CC_IO_0_PIN, &u32Value);
            if (res == 0)
            {
                if (u32Value)
                {
                    *RValue = SIG_CC_R_0_VALUE;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //r1=220;r1(156,244)
        else if ((CC_R >= (SIG_CC_R_1_VALUE-0.2*SIG_CC_R_1_VALUE)) && (CC_R <= (SIG_CC_R_1_VALUE+0.2*SIG_CC_R_1_VALUE)))
        {
            hal_gpio_write_pin(SIG_CC_IO_1_PORT, SIG_CC_IO_1_PIN, 1);
            res = hal_gpio_get_pin_output_value(SIG_CC_IO_1_PORT, SIG_CC_IO_1_PIN, &u32Value);
            if (res == 0)
            {
                if (u32Value)
                {
                    *RValue = SIG_CC_R_1_VALUE;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //r2=670;r2(603,727)
        else if ((CC_R >= (SIG_CC_R_2_VALUE-0.1*SIG_CC_R_2_VALUE)) && (CC_R <= (SIG_CC_R_2_VALUE+0.1*SIG_CC_R_2_VALUE)))
        {
            hal_gpio_write_pin(SIG_CC_IO_2_PORT, SIG_CC_IO_2_PIN, 1);
            res = hal_gpio_get_pin_output_value(SIG_CC_IO_2_PORT, SIG_CC_IO_2_PIN, &u32Value);
            if (res == 0)
            {
                if (u32Value)
                {
                    *RValue = SIG_CC_R_2_VALUE;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //r3=1510;r3(1359,1661)
        else if ((CC_R >= (SIG_CC_R_3_VALUE-0.1*SIG_CC_R_3_VALUE)) && (CC_R <= (SIG_CC_R_3_VALUE+0.1*SIG_CC_R_3_VALUE)))
        {
            hal_gpio_write_pin(SIG_CC_IO_3_PORT, SIG_CC_IO_3_PIN, 1);
            res = hal_gpio_get_pin_output_value(SIG_CC_IO_3_PORT, SIG_CC_IO_3_PIN, &u32Value);
            if (res == 0)
            {
                if (u32Value)
                {
                    *RValue = SIG_CC_R_3_VALUE;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //r4=100;r4(80,120)
        else if ((CC_R >= (SIG_CC_R_4_VALUE-0.2*SIG_CC_R_4_VALUE)) && (CC_R <= (SIG_CC_R_4_VALUE+0.2*SIG_CC_R_4_VALUE)))
        {
            hal_gpio_write_pin(SIG_CC_IO_4_PORT, SIG_CC_IO_4_PIN, 1);
            res = hal_gpio_get_pin_output_value(SIG_CC_IO_4_PORT, SIG_CC_IO_4_PIN, &u32Value);
            if (res == 0)
            {
                if (u32Value)
                {
                    *RValue = SIG_CC_R_4_VALUE;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //r5=510;r5(459,561)
        else if ((CC_R >= (SIG_CC_R_5_VALUE-0.1*SIG_CC_R_5_VALUE)) && (CC_R <= (SIG_CC_R_5_VALUE+0.1*SIG_CC_R_5_VALUE)))
        {
            hal_gpio_write_pin(SIG_CC_IO_5_PORT, SIG_CC_IO_5_PIN, 1);
            res = hal_gpio_get_pin_output_value(SIG_CC_IO_5_PORT, SIG_CC_IO_5_PIN, &u32Value);
            if (res == 0)
            {
                if (u32Value)
                {
                    *RValue = SIG_CC_R_5_VALUE;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //r6=1000;r6(900,1100)
        else if ((CC_R >= (SIG_CC_R_6_VALUE-0.1*SIG_CC_R_6_VALUE)) && (CC_R <= (SIG_CC_R_6_VALUE+0.1*SIG_CC_R_6_VALUE)))
        {
            hal_gpio_write_pin(SIG_CC_IO_6_PORT, SIG_CC_IO_6_PIN, 1);
            res = hal_gpio_get_pin_output_value(SIG_CC_IO_6_PORT, SIG_CC_IO_6_PIN, &u32Value);
            if (res == 0)
            {
                if (u32Value)
                {
                    *RValue = SIG_CC_R_6_VALUE;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //r7=3600;r7(3240,3960)
        else if ((CC_R >= (SIG_CC_R_7_VALUE-0.1*SIG_CC_R_7_VALUE)) && (CC_R <= (SIG_CC_R_7_VALUE+0.1*SIG_CC_R_7_VALUE)))
        {
            hal_gpio_write_pin(SIG_CC_IO_7_PORT, SIG_CC_IO_7_PIN, 1);
            res = hal_gpio_get_pin_output_value(SIG_CC_IO_7_PORT, SIG_CC_IO_7_PIN, &u32Value);
            if (res == 0)
            {
                if (u32Value)
                {
                    *RValue = SIG_CC_R_7_VALUE;
                }
                else
                {
                    res = 3;
                }
            }
        }
        if (CC_R == 0xFFFF)
        {
            //开路
            hal_gpio_write_pin(SIG_CC_IO_0_PORT, SIG_CC_IO_0_PIN, 0);
            hal_gpio_write_pin(SIG_CC_IO_1_PORT, SIG_CC_IO_1_PIN, 0);
            hal_gpio_write_pin(SIG_CC_IO_2_PORT, SIG_CC_IO_2_PIN, 0);
            hal_gpio_write_pin(SIG_CC_IO_3_PORT, SIG_CC_IO_3_PIN, 0);
            hal_gpio_write_pin(SIG_CC_IO_4_PORT, SIG_CC_IO_4_PIN, 0);
            hal_gpio_write_pin(SIG_CC_IO_5_PORT, SIG_CC_IO_5_PIN, 0);
            hal_gpio_write_pin(SIG_CC_IO_6_PORT, SIG_CC_IO_6_PIN, 0);
            hal_gpio_write_pin(SIG_CC_IO_7_PORT, SIG_CC_IO_7_PIN, 0);
        }
    }

    if (CAT_NUM_BIT(Mask, 1))
    {
        //CC2 1k电阻下拉
        if (CC2)
        {
            hal_gpio_write_pin(SIG_CC2_IO_PORT, SIG_CC2_IO_PIN, 1);
            res = hal_gpio_get_pin_output_value(SIG_CC2_IO_PORT, SIG_CC2_IO_PIN, &u32Value);
            if (res == 0)
            {
                //返回cc2下拉状态
                *CC2State = u32Value;
            }
        }
    }
    return res;
}

uint16_t
board_pin_di_read(uint32_t *DI_0, uint32_t *DI_1)
{
    uint16_t res = 0;
    res = hal_gpio_read_pin(SIG_DI_0_PORT, SIG_DI_0_PIN, DI_0);
    res |= hal_gpio_read_pin(SIG_DI_0_PORT, SIG_DI_0_PIN, DI_1);
    return res;
}












