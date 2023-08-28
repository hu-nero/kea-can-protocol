/*
 * hal_gpio.h
 *
 *  Created on: 2023Äê2ÔÂ6ÈÕ
 *      Author: xinlei.hu
 */

#ifndef HAL_GPIO_HAL_GPIO_H_
#define HAL_GPIO_HAL_GPIO_H_

#include "stdint.h"

//eka has 3 GPIO,each 32bit GPIO is divided into 4 regions
typedef enum{
    HAL_GPIOA_PTA = 0U,
    HAL_GPIOA_PTB,
    HAL_GPIOA_PTC,
    HAL_GPIOA_PTD,
    HAL_GPIOB_PTE,
    HAL_GPIOB_PTF,
    HAL_GPIOB_PTG,
    HAL_GPIOB_PTH,
    HAL_GPIOC_PTI
} HAL_GPIO_PORT_Type;

typedef enum{
    HAL_GPIO_PIN0 = 0U,
    HAL_GPIO_PIN1,
    HAL_GPIO_PIN2,
    HAL_GPIO_PIN3,
    HAL_GPIO_PIN4,
    HAL_GPIO_PIN5,
    HAL_GPIO_PIN6,
    HAL_GPIO_PIN7
} HAL_GPIO_PIN_Type;

typedef enum
{
    HAL_GPIO_INPUT_DIRECTION        = 0x0U,
    HAL_GPIO_OUTPUT_DIRECTION       = 0x1U,
    HAL_GPIO_UNSPECIFIED_DIRECTION  = 0X2U
} HAL_GPIO_DIR_Type;


extern uint16_t hal_gpio_init(void);
extern uint16_t hal_gpio_deinit(void);
extern uint16_t hal_gpio_write_pin(HAL_GPIO_PORT_Type Port, HAL_GPIO_PIN_Type Pin, uint32_t Value);
extern uint16_t hal_gpio_get_pin_output_value(HAL_GPIO_PORT_Type Port, HAL_GPIO_PIN_Type Pin, uint32_t *Value);
extern uint16_t hal_gpio_read_pin(HAL_GPIO_PORT_Type Port, HAL_GPIO_PIN_Type Pin, uint32_t *Value);
extern uint16_t hal_gpio_toggle_pin(HAL_GPIO_PORT_Type Port, HAL_GPIO_PIN_Type Pin);
extern uint16_t hal_gpio_set_pin_dir(HAL_GPIO_PORT_Type Port, HAL_GPIO_PIN_Type Pin, HAL_GPIO_DIR_Type Dir, uint8_t Value);


#endif /* HAL_GPIO_HAL_GPIO_H_ */
