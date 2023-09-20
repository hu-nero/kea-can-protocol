/*
 * service_switch_ctrl.c
 *
 *  Created on: 2023Äê9ÔÂ20ÈÕ
 *      Author: xinlei.hu
 */
#include "service_switch_ctrl.h"
#include "board_config.h"
#include "board_pin_config.h"
#include "Hal.h"


void
switch_ctrl_service_output_io_init(void)
{
    //switch output io init
    hal_gpio_set_pin_dir(SIG_INTERLOCK_PORT, SIG_INTERLOCK_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_KL15_PORT, SIG_KL15_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_DO_0_PORT, SIG_DO_0_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(SIG_DO_1_PORT, SIG_DO_1_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
}

uint16_t
switch_ctrl_service_output_ctrl(uint8_t BitState, uint8_t Mask, uint32_t *Value)
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
                        hal_gpio_write_pin(SIG_INTERLOCK_PORT, SIG_INTERLOCK_PIN, CAT_NUM_BIT(BitState, bit));
                        if ((res = hal_gpio_get_pin_output_value(SIG_INTERLOCK_PORT, SIG_INTERLOCK_PIN, &u32Value)) != 0)
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
