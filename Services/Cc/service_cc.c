/*
 * service_cc.c
 *
 *  Created on: 2023年9月19日
 *      Author: xinlei.hu
 */
#include "service_cc.h"
#include "board_config.h"
#include "board_pin_config.h"
#include "Hal.h"


void
cc_service_io_init(void)
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
cc_service_io_reset(void)
{
    //CC io ctrl init
    hal_gpio_write_pin(SIG_CC2_IO_PORT, SIG_CC2_IO_PIN  , 0);
    hal_gpio_write_pin(SIG_CC_IO_0_PORT, SIG_CC_IO_0_PIN, 0);
    hal_gpio_write_pin(SIG_CC_IO_1_PORT, SIG_CC_IO_1_PIN, 0);
    hal_gpio_write_pin(SIG_CC_IO_2_PORT, SIG_CC_IO_2_PIN, 0);
    hal_gpio_write_pin(SIG_CC_IO_3_PORT, SIG_CC_IO_3_PIN, 0);
    hal_gpio_write_pin(SIG_CC_IO_4_PORT, SIG_CC_IO_4_PIN, 0);
    hal_gpio_write_pin(SIG_CC_IO_5_PORT, SIG_CC_IO_5_PIN, 0);
    hal_gpio_write_pin(SIG_CC_IO_6_PORT, SIG_CC_IO_6_PIN, 0);
    hal_gpio_write_pin(SIG_CC_IO_7_PORT, SIG_CC_IO_7_PIN, 0);
}

uint16_t
cc_service_r_ctrl(uint16_t CC_R, uint8_t CC2, uint8_t Mask, uint16_t *RValue, uint8_t *CC2State)
{
    uint16_t res = 0;
    uint32_t u32Value = 0;
    //init
    *RValue = *CC2State = 0;
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
