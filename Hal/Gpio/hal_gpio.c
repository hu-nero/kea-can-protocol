/*
 * hal_gpio.c
 *
 *  Created on: 2023Äê2ÔÂ6ÈÕ
 *      Author: xinlei.hu
 */
#include "hal_gpio.h"
#include "PE_Types.h"
#include "GPIO.h"

static LDD_TDeviceData *halGpioDevicePtr = NULL;

/**
 * @brief: port to gpio type
 *
 * @param Port: port type,ex:HAL_GPIOA_PTA
 *
 * @return:GPIO_BASE_Type
 */
static inline GPIO_BASE_Type
hal_gpio_port_to_type(HAL_GPIO_PORT_Type Port)
{
    GPIO_BASE_Type gpioType = GPIO_MAX_TYPE;
    switch(Port)
    {
        case HAL_GPIOA_PTA:
        case HAL_GPIOA_PTB:
        case HAL_GPIOA_PTC:
        case HAL_GPIOA_PTD:
            {
                gpioType = GPIOA_BASE_TYPE;
            }
            break;
        case HAL_GPIOB_PTE:
        case HAL_GPIOB_PTF:
        case HAL_GPIOB_PTG:
        case HAL_GPIOB_PTH:
            {
                gpioType = GPIOB_BASE_TYPE;
            }
            break;
        case HAL_GPIOC_PTI:
            {
                gpioType = GPIOC_BASE_TYPE;
            }
            break;
        default:break;
    }
    return gpioType;
}

/**
 * @brief :port to offset;each 32bit GPIO has 4 groups of IO
 *
 * @param Port: port type,ex:HAL_GPIOA_PTA
 *
 * @return :offset
 */
static inline uint32_t
hal_gpio_port_to_offset(HAL_GPIO_PORT_Type Port)
{
    uint32_t u32Offset = 0xFF;
    switch(Port)
    {
        case HAL_GPIOA_PTA:
            {
                u32Offset = 0U;
            }
            break;
        case HAL_GPIOA_PTB:
            {
                u32Offset = 8U;
            }
            break;
        case HAL_GPIOA_PTC:
            {
                u32Offset = 16U;
            }
            break;
        case HAL_GPIOA_PTD:
            {
                u32Offset = 24U;
            }
            break;
        case HAL_GPIOB_PTE:
            {
                u32Offset = 0U;
            }
            break;
        case HAL_GPIOB_PTF:
            {
                u32Offset = 8U;
            }
            break;
        case HAL_GPIOB_PTG:
            {
                u32Offset = 16U;
            }
            break;
        case HAL_GPIOB_PTH:
            {
                u32Offset = 24U;
            }
            break;
        case HAL_GPIOC_PTI:
            {
                u32Offset = 0U;
            }
            break;
        default:break;
    }
    return u32Offset;
}

/**
 * @brief :init gpio
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_gpio_init(void)
{
    //init gpio device
    halGpioDevicePtr = GPIO_Init(NULL);
    if(NULL == halGpioDevicePtr)
        return 1;
    return 0;
}
/**
 * @brief :deinit gpio
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_gpio_deinit(void)
{
    //deinit gpio device
    if(NULL == halGpioDevicePtr)
        return 1;
    GPIO_Deinit(halGpioDevicePtr);
    return 0;
}

/**
 * @brief :set pin level
 *
 * @param Port :port
 * @param Pin :pin
 * @param Value :high/low level
 *
 * @return :0 is success,other is error
 */
uint16_t
hal_gpio_write_pin(HAL_GPIO_PORT_Type Port, HAL_GPIO_PIN_Type Pin, uint32_t Value)
{
    GPIO_BASE_Type gpioType = GPIO_MAX_TYPE;
    uint32_t u32Offset = 0;
    uint32_t u32PinMask = 0;

    if( (0 != Value) && (1 != Value))
    {
        return 1;
    }
    gpioType = hal_gpio_port_to_type(Port);
    if(gpioType == GPIO_MAX_TYPE)
        return 1;
    u32Offset = hal_gpio_port_to_offset(Port) + Pin;
    if(u32Offset == 0xFF)
        return 2;
    u32PinMask = (uint32_t)(1 << u32Offset);
    switch(Value)
    {
        case 0:
            {
                GPIO_ClearPortBits(halGpioDevicePtr, gpioType, u32PinMask);
            }
            break;
        case 1:
            {
                GPIO_SetPortBits(halGpioDevicePtr, gpioType, u32PinMask);
            }
            break;
        default:break;
    }
    return 0;
}

uint16_t
hal_gpio_get_pin_output_value(HAL_GPIO_PORT_Type Port, HAL_GPIO_PIN_Type Pin, uint32_t *Value)
{
    GPIO_BASE_Type gpioType = GPIO_MAX_TYPE;
    uint32_t u32Offset = 0;
    uint32_t u32PinMask = 0;

    gpioType = hal_gpio_port_to_type(Port);
    if(gpioType == GPIO_MAX_TYPE)
        return 1;
    u32Offset = hal_gpio_port_to_offset(Port) + Pin;
    if(u32Offset == 0xFF)
        return 2;
    u32PinMask = (uint32_t)(1 << u32Offset);
    *Value = GPIO_GetPortOutputValue(halGpioDevicePtr, gpioType);
    *Value = (((*Value) & u32PinMask) >> u32Offset);
    return  0;
}
/**
 * @brief :get pin level
 *
 * @param Port :port
 * @param Pin :pin
 * @param Value :value to store
 *
 * @return :0 is success,other is error
 */
uint16_t
hal_gpio_read_pin(HAL_GPIO_PORT_Type Port, HAL_GPIO_PIN_Type Pin, uint32_t *Value)
{
    GPIO_BASE_Type gpioType = GPIO_MAX_TYPE;
    uint32_t u32Offset = 0;
    uint32_t u32PinMask = 0;

    gpioType = hal_gpio_port_to_type(Port);
    if(gpioType == GPIO_MAX_TYPE)
        return 1;
    u32Offset = hal_gpio_port_to_offset(Port) + Pin;
    if(u32Offset == 0xFF)
        return 2;
    u32PinMask = (uint32_t)(1 << u32Offset);
    *Value = GPIO_GetPortInputValue(halGpioDevicePtr, gpioType);
    *Value = (((*Value) & u32PinMask) >> u32Offset);
    return  0;
}

/**
 * @brief :toggle pin level
 *
 * @param Port :port
 * @param Pin :pin
 *
 * @return :0 is succrss,other is error
 */
uint16_t
hal_gpio_toggle_pin(HAL_GPIO_PORT_Type Port, HAL_GPIO_PIN_Type Pin)
{
    GPIO_BASE_Type gpioType = GPIO_MAX_TYPE;
    uint32_t u32Offset = 0;
    uint32_t u32PinMask = 0;

    gpioType = hal_gpio_port_to_type(Port);
    if(gpioType == GPIO_MAX_TYPE)
        return 1;
    u32Offset = hal_gpio_port_to_offset(Port) + Pin;
    if(u32Offset == 0xFF)
        return 2;
    u32PinMask = (uint32_t)(1 << u32Offset);
    GPIO_TogglePortBits(halGpioDevicePtr, gpioType, u32PinMask);
    return 0;
}

/**
 * @brief :set pin dir
 *
 * @param Port :port
 * @param Pin :pin
 * @param Dir :input/output
 * @param Value :low/high level in output dir
 *
 * @return :0 is success,other is error
 */
uint16_t
hal_gpio_set_pin_dir(HAL_GPIO_PORT_Type Port, HAL_GPIO_PIN_Type Pin, HAL_GPIO_DIR_Type Dir, uint8_t Value)
{
    GPIO_BASE_Type gpioType = GPIO_MAX_TYPE;
    uint32_t u32Offset = 0;
    uint32_t u32PinMask = 0;

    if(Dir == HAL_GPIO_UNSPECIFIED_DIRECTION)
        return 1;

    gpioType = hal_gpio_port_to_type(Port);
    if(gpioType == GPIO_MAX_TYPE)
        return 1;
    u32Offset = hal_gpio_port_to_offset(Port) + Pin;
    if(u32Offset == 0xFF)
        return 2;
    u32PinMask = (uint32_t)(1 << u32Offset);

    switch(Dir)
    {
        case HAL_GPIO_INPUT_DIRECTION:
            {
                GPIO_SetPortInputDirection(halGpioDevicePtr, gpioType, u32PinMask);
            }
            break;
        case HAL_GPIO_OUTPUT_DIRECTION:
            {
                if(Value)
                {
                    GPIO_SetPortOutputDirection(halGpioDevicePtr, gpioType, u32PinMask, u32PinMask);
                }else
                {
                    GPIO_SetPortOutputDirection(halGpioDevicePtr, gpioType, u32PinMask, Value);
                }
            }
            break;
        default:return 2;
    }
    return 0;
}


