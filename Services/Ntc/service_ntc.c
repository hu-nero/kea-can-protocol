/*
 * service_ntc.c
 *
 *  Created on: 2023年9月19日
 *      Author: xinlei.hu
 */
#include "service_ntc.h"
#include "board_config.h"
#include "board_pin_config.h"
#include "Hal.h"

void
ntc_service_io_init(void)
{
    //NTC io ctrl init
    hal_gpio_set_pin_dir(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
}

void
ntc_service_io_reset(void)
{
    //NTC io ctrl init
    hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 0);
    hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 0);
    hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 0);
    hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 0);
}
/**
 * @brief :ctrl ntc r select
 *
 * @param NTC_R:input ntc r
 * @param Ch:channal
 * @param RValue:pointer of r value
 * @param ChState:pointer of ch state
 *
 * @return :0 is success;4 is illegal resistance;other is failed
 */
uint16_t
ntc_service_r_ctrl(uint16_t NTC_R, uint8_t Ch, uint16_t *RValue, uint8_t *ChState)
{
    uint16_t res = 0;
    //按bit设置电阻
#if SERVICES_NTC_CH_TEST
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
#else
    uint32_t u32Value[4] = {0};
    *RValue = *ChState = 0;
    if ((CAT_NUM_BIT(Ch, SERVICES_NTC_CH_0)) && (CAT_NUM_BIT(Ch, SERVICES_NTC_CH_1)))
    {
        //R0=R1(46440,69660)
        if ((NTC_R >= (SERVICES_NTC_R0_R1_58K-0.2*SERVICES_NTC_R0_R1_58K)) && (NTC_R <= (SERVICES_NTC_R0_R1_58K+0.2*SERVICES_NTC_R0_R1_58K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 0);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 1) && (u32Value[1] == 1) && (u32Value[2] == 0) && (u32Value[3] == 0))
                {
                    *RValue = SERVICES_NTC_R0_R1_58K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //R0=R1(10856,16284)
        else if ((NTC_R >= (SERVICES_NTC_R0_R1_13K-0.2*SERVICES_NTC_R0_R1_13K)) && (NTC_R <= (SERVICES_NTC_R0_R1_13K+0.2*SERVICES_NTC_R0_R1_13K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 1);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 1) && (u32Value[1] == 1) && (u32Value[2] == 0) && (u32Value[3] == 1))
                {
                    *RValue = SERVICES_NTC_R0_R1_13K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //R0=R1(2229,2670)
        else if ((NTC_R >= (SERVICES_NTC_R0_R1_2_45K-0.09*SERVICES_NTC_R0_R1_2_45K)) && (NTC_R <= (SERVICES_NTC_R0_R1_2_45K+0.09*SERVICES_NTC_R0_R1_2_45K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 0);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 1) && (u32Value[1] == 1) && (u32Value[2] == 1) && (u32Value[3] == 0))
                {
                    *RValue = SERVICES_NTC_R0_R1_2_45K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //R0=R1(2702,3237)
        else if ((NTC_R >= (SERVICES_NTC_R0_R1_2_97K-0.09*SERVICES_NTC_R0_R1_2_97K)) && (NTC_R <= (SERVICES_NTC_R0_R1_2_97K+0.09*SERVICES_NTC_R0_R1_2_97K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 1);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 1) && (u32Value[1] == 1) && (u32Value[2] == 1) && (u32Value[3] == 1))
                {
                    *RValue = SERVICES_NTC_R0_R1_2_97K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        else
        {
            res = 4;
        }
    } 
    else if ((CAT_NUM_BIT(Ch, SERVICES_NTC_CH_0) == 0) && (CAT_NUM_BIT(Ch, SERVICES_NTC_CH_1) == 1))
    {
        //R0=0,R1(45336,68004)
        if ((NTC_R >= (SERVICES_NTC_R1_56K-0.2*SERVICES_NTC_R1_56K)) && (NTC_R <= (SERVICES_NTC_R1_56K+0.2*SERVICES_NTC_R1_56K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 0);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 0) && (u32Value[1] == 1) && (u32Value[2] == 0) && (u32Value[3] == 0))
                {
                    *RValue = SERVICES_NTC_R1_56K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //R0=0,R1(9616,14424)
        else if ((NTC_R >= (SERVICES_NTC_R1_12K-0.2*SERVICES_NTC_R1_12K)) && (NTC_R <= (SERVICES_NTC_R1_12K+0.2*SERVICES_NTC_R1_12K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 1);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 0) && (u32Value[1] == 1) && (u32Value[2] == 0) && (u32Value[3] == 1))
                {
                    *RValue = SERVICES_NTC_R1_12K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //R0=0,R1(1901,2278)
        else if ((NTC_R >= (SERVICES_NTC_R1_2_09K-0.09*SERVICES_NTC_R1_2_09K)) && (NTC_R <= (SERVICES_NTC_R1_2_09K+0.09*SERVICES_NTC_R1_2_09K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 0);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 0) && (u32Value[1] == 1) && (u32Value[2] == 1) && (u32Value[3] == 0))
                {
                    *RValue = SERVICES_NTC_R1_2_09K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //R0=R1(2338,2801)
        else if ((NTC_R >= (SERVICES_NTC_R1_2_57K-0.09*SERVICES_NTC_R1_2_57K)) && (NTC_R <= (SERVICES_NTC_R1_2_57K+0.09*SERVICES_NTC_R1_2_57K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 1);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 0) && (u32Value[1] == 1) && (u32Value[2] == 1) && (u32Value[3] == 1))
                {
                    *RValue = SERVICES_NTC_R1_2_57K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        else
        {
            res = 4;
        }
    }
    else if ((CAT_NUM_BIT(Ch, SERVICES_NTC_CH_0) == 1) && (CAT_NUM_BIT(Ch, SERVICES_NTC_CH_1) == 0))
    {
        //R0(45392,68088),R1=0
        if ((NTC_R >= (SERVICES_NTC_R0_56K-0.2*SERVICES_NTC_R0_56K)) && (NTC_R <= (SERVICES_NTC_R0_56K+0.2*SERVICES_NTC_R0_56K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 0);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 1) && (u32Value[1] == 0) && (u32Value[2] == 0) && (u32Value[3] == 0))
                {
                    *RValue = SERVICES_NTC_R0_56K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //R0(9696,14544),R1=0
        else if ((NTC_R >= (SERVICES_NTC_R0_12K-0.2*SERVICES_NTC_R0_12K)) && (NTC_R <= (SERVICES_NTC_R0_12K+0.2*SERVICES_NTC_R0_12K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 1);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 1) && (u32Value[1] == 0) && (u32Value[2] == 0) && (u32Value[3] == 1))
                {
                    *RValue = SERVICES_NTC_R0_12K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //R0(1911,2289),R1=0
        else if ((NTC_R >= (SERVICES_NTC_R0_2_10K-0.09*SERVICES_NTC_R0_2_10K)) && (NTC_R <= (SERVICES_NTC_R0_2_10K+0.09*SERVICES_NTC_R0_2_10K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 0);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 1) && (u32Value[1] == 0) && (u32Value[2] == 1) && (u32Value[3] == 0))
                {
                    *RValue = SERVICES_NTC_R0_2_10K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        //R0(2438,2921),R1=0
        else if ((NTC_R >= (SERVICES_NTC_R0_2_68K-0.09*SERVICES_NTC_R0_2_68K)) && (NTC_R <= (SERVICES_NTC_R0_2_68K+0.09*SERVICES_NTC_R0_2_68K)))
        {
            hal_gpio_write_pin(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, 0);
            hal_gpio_write_pin(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, 1);
            hal_gpio_write_pin(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, 1);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_0_PORT, SIG_NTC_IO_0_PIN, &u32Value[0]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_1_PORT, SIG_NTC_IO_1_PIN, &u32Value[1]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_2_PORT, SIG_NTC_IO_2_PIN, &u32Value[2]);
            res |= hal_gpio_get_pin_output_value(SIG_NTC_IO_3_PORT, SIG_NTC_IO_3_PIN, &u32Value[3]);
            if (res == 0)
            {
                if ((u32Value[0] == 1) && (u32Value[1] == 0) && (u32Value[2] == 1) && (u32Value[3] == 1))
                {
                    *RValue = SERVICES_NTC_R0_2_68K;
                }
                else
                {
                    res = 3;
                }
            }
        }
        else
        {
            res = 4;
        }
    }
#endif
    return res;
}


