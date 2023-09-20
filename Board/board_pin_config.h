/*
 * board_pin_config.h
 *
 *  Created on: 2023Äê2ÔÂ8ÈÕ
 *      Author: xinlei.hu
 */

#ifndef BOARD_BOARD_PIN_CONFIG_H_
#define BOARD_BOARD_PIN_CONFIG_H_
#include <stdint.h>

//KPB17 LED
#define MCU_LED_G_PORT                             HAL_GPIOA_PTC
#define MCU_LED_G_PIN                              5u
//HC03 SIG LED
#define MCU_LED0_PORT                              HAL_GPIOA_PTD
#define MCU_LED0_PIN                               1u
#define MCU_LED1_PORT                              HAL_GPIOA_PTD
#define MCU_LED1_PIN                               0u
//switch output ctrl
#define BOAED_PIN_SWITCH_CTRL_CHSIZE               8u
#define SIG_INTERLOCK_PORT                         HAL_GPIOB_PTH
#define SIG_INTERLOCK_PIN                          6u
#define SIG_KL15_PORT                              HAL_GPIOB_PTH
#define SIG_KL15_PIN                               7u
#define SIG_DO_0_PORT                              HAL_GPIOB_PTE
#define SIG_DO_0_PIN                               5u
#define SIG_DO_1_PORT                              HAL_GPIOA_PTB
#define SIG_DO_1_PIN                               4u
//input DI
#define SIG_DI_0_PORT                              HAL_GPIOA_PTB
#define SIG_DI_0_PIN                               5u
#define SIG_DI_1_PORT                              HAL_GPIOA_PTC
#define SIG_DI_1_PIN                               3u
//NTC resistance
#define SIG_NTC_IO_0_PORT                          HAL_GPIOA_PTB
#define SIG_NTC_IO_0_PIN                           1u
#define SIG_NTC_IO_1_PORT                          HAL_GPIOA_PTB
#define SIG_NTC_IO_1_PIN                           0u
#define SIG_NTC_IO_2_PORT                          HAL_GPIOB_PTF
#define SIG_NTC_IO_2_PIN                           3u
#define SIG_NTC_R_2_OPEN_VALUE                     0u//0.1k
#define SIG_NTC_R_2_CLOSE_VALUE                    0u//0.1k
#define SIG_NTC_IO_3_PORT                          HAL_GPIOB_PTF
#define SIG_NTC_IO_3_PIN                           2u
#define SIG_NTC_R_3_OPEN_VALUE                     1098u//0.1k
#define SIG_NTC_R_3_CLOSE_VALUE                    200u//0.1k
//CC ctrl
#define SIG_CC2_IO_PORT                            HAL_GPIOA_PTA
#define SIG_CC2_IO_PIN                             1u
#define SIG_CC_IO_0_PORT                           HAL_GPIOA_PTA
#define SIG_CC_IO_0_PIN                            7u
#define SIG_CC_R_0_VALUE                           51u
#define SIG_CC_IO_1_PORT                           HAL_GPIOA_PTA
#define SIG_CC_IO_1_PIN                            6u
#define SIG_CC_R_1_VALUE                           220u
#define SIG_CC_IO_2_PORT                           HAL_GPIOB_PTE
#define SIG_CC_IO_2_PIN                            4u
#define SIG_CC_R_2_VALUE                           670u
#define SIG_CC_IO_3_PORT                           HAL_GPIOB_PTF
#define SIG_CC_IO_3_PIN                            1u
#define SIG_CC_R_3_VALUE                           1510u
#define SIG_CC_IO_4_PORT                           HAL_GPIOB_PTF
#define SIG_CC_IO_4_PIN                            0u
#define SIG_CC_R_4_VALUE                           100u
#define SIG_CC_IO_5_PORT                           HAL_GPIOA_PTD
#define SIG_CC_IO_5_PIN                            4u
#define SIG_CC_R_5_VALUE                           510u
#define SIG_CC_IO_6_PORT                           HAL_GPIOA_PTD
#define SIG_CC_IO_6_PIN                            3u
#define SIG_CC_R_6_VALUE                           1000u
#define SIG_CC_IO_7_PORT                           HAL_GPIOA_PTD
#define SIG_CC_IO_7_PIN                            2u
#define SIG_CC_R_7_VALUE                           3600u
//CP
#define SIG_CP_IO_CTRL_PORT                        HAL_GPIOA_PTD
#define SIG_CP_IO_CTRL_PIN                         6u
#define SIG_CP_ADC_PORT                            HAL_GPIOA_PTD
#define SIG_CP_ADC_PIN                             7u


typedef enum
{
    BOARD_PIN_SWITCH_CTRL_INTERLOCK_IO = 0,
    BOARD_PIN_SWITCH_CTRL_KL15 = 1,
    BOARD_PIN_SWITCH_CTRL_DO_IO_0 = 4,
    BOARD_PIN_SWITCH_CTRL_DO_IO_1 = 5
} BOARD_PIN_SWITCH_CTRL_STRUCT_Enum;

#endif /* BOARD_BOARD_PIN_CONFIG_H_ */


