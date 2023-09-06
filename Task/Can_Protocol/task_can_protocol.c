/*
 * task_can_protocol.c
 *
 *  Created on: 2023年8月29日
 *      Author: xinlei.hu
 */
#include "task_can_protocol.h"
#include <string.h>
#include "Hal.h"
#include "board_config.h"


TsCanFrame gTsCanFrame = {};
static TsCanFrame *gTsCanFramePtr = NULL;
static uint8_t gu8Channal = 0xFF;

static uint16_t can_protocol_task_single_ch_assignctrl(uint8_t Ch, uint8_t Value);
static uint16_t can_protocol_task_single_ch_read(uint8_t Ch, uint32_t *Value);

void
can_protocol_task_init(void)
{
    gTsCanFrame.id = CAN_PROTOCOL_ID_RESPONSE;
    gTsCanFrame.dlc = 8;
    gTsCanFrame.is_can_fd = 0;
    gTsCanFrame.is_ext_id = 1;
    memset(gTsCanFrame.data, 0, 8);

    hal_gpio_set_pin_dir(DUT_CH0_PORT, DUT_CH0_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH1_PORT, DUT_CH1_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH2_PORT, DUT_CH2_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH3_PORT, DUT_CH3_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH4_PORT, DUT_CH4_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH5_PORT, DUT_CH5_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH6_PORT, DUT_CH6_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH7_PORT, DUT_CH7_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH8_PORT, DUT_CH8_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH9_PORT, DUT_CH9_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH10_PORT, DUT_CH10_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH11_PORT, DUT_CH11_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH12_PORT, DUT_CH12_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH13_PORT, DUT_CH13_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH14_PORT, DUT_CH14_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH15_PORT, DUT_CH15_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
}

uint16_t
can_protocol_task(void)
{
    //解析CAN报文
    uint8_t res = 0;
    if ((gTsCanFramePtr = hal_can_rx_queue_de()) != NULL)
    {
        memset(gTsCanFrame.data, 0, 8);
        //ID
        switch (gTsCanFramePtr->id)
        {
            case CAN_PROTOCOL_ID_REQUEST:
                {
                    //CMD
                    switch (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_CMD])
                    {
                        case CAN_PROTOCOL_CMD_SYSTEM_INFO_REQUEST:
                            {
                                TimetickUnion timetickTmp;
                                timetickTmp.u32TimeTick = hal_timer_get_time();
                                //TODO:返回软件版本和开机时间
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_SYSTEM_INFO_REQUEST;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SYSINFO_VERSION_OFFSET] = SoftsVer[10];
                                memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SYSINFO_STARTTIME_OFFSET], timetickTmp.u8Tick, sizeof(timetickTmp));
                                (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                            }
                            break;
                        case CAN_PROTOCOL_CMD_ASSIGN_CTRL:
                            {
                                uint32_t u32Value = 0;
                                uint8_t u8Channal = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET];
                                res = can_protocol_task_single_ch_assignctrl(u8Channal, 1);
                                //选通失败
                                if (res == 1)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_ASSIGN_CTRL;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET] = gu8Channal;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                    return 0;
                                }
                                //都不选通
                                else if (res == 2)
                                {
                                    gu8Channal = 0xFF;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_ASSIGN_CTRL;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET] = 0xFF;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                    return 0;
                                }
                                res = can_protocol_task_single_ch_read(u8Channal, &u32Value);
                                //读取失败
                                if (res)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_ASSIGN_CTRL;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET] = gu8Channal;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                //读取成功
                                else
                                {
                                    if (u32Value)
                                    {
                                        gu8Channal = u8Channal;
                                        gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                        gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_ASSIGN_CTRL;
                                        gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                        gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET] = u8Channal;
                                        (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                    }
                                }
                            }
                            break;
                        default:break;
                    }
                }
                break;
            default:break;
        }
    }
}

/**
 * @brief :single assign ctrl
 *
 * @param Ch:channal
 * @param Value:IO value
 *
 * @return :0 is success;1 is failed;2 is None of ch are selected
 */
static uint16_t
can_protocol_task_single_ch_assignctrl(uint8_t Ch, uint8_t Value)
{
    uint16_t res = 1;
    switch (Ch)
    {
        case 0:
            {
                res = hal_gpio_write_pin(DUT_CH0_PORT, DUT_CH0_PIN, Value);
            }
            break;
        case 1:
            {
                res = hal_gpio_write_pin(DUT_CH1_PORT, DUT_CH1_PIN, Value);
            }
            break;
        case 2:
            {
                res = hal_gpio_write_pin(DUT_CH2_PORT, DUT_CH2_PIN, Value);
            }
            break;
        case 3:
            {
                res = hal_gpio_write_pin(DUT_CH3_PORT, DUT_CH3_PIN, Value);
            }
            break;
        case 4:
            {
                res = hal_gpio_write_pin(DUT_CH4_PORT, DUT_CH4_PIN, Value);
            }
            break;
        case 5:
            {
                res = hal_gpio_write_pin(DUT_CH5_PORT, DUT_CH5_PIN, Value);
            }
            break;
        case 6:
            {
                res = hal_gpio_write_pin(DUT_CH6_PORT, DUT_CH6_PIN, Value);
            }
            break;
        case 7:
            {
                res = hal_gpio_write_pin(DUT_CH7_PORT, DUT_CH7_PIN, Value);
            }
            break;
        case 8:
            {
                res = hal_gpio_write_pin(DUT_CH8_PORT, DUT_CH8_PIN, Value);
            }
            break;
        case 9:
            {
                res = hal_gpio_write_pin(DUT_CH9_PORT, DUT_CH9_PIN, Value);
            }
            break;
        case 10:
            {
                res = hal_gpio_write_pin(DUT_CH10_PORT, DUT_CH10_PIN, Value);
            }
            break;
        case 11:
            {
                res = hal_gpio_write_pin(DUT_CH11_PORT, DUT_CH11_PIN, Value);
            }
            break;
        case 12:
            {
                res = hal_gpio_write_pin(DUT_CH12_PORT, DUT_CH12_PIN, Value);
            }
            break;
        case 13:
            {
                res = hal_gpio_write_pin(DUT_CH13_PORT, DUT_CH13_PIN, Value);
            }
            break;
        case 14:
            {
                res = hal_gpio_write_pin(DUT_CH14_PORT, DUT_CH14_PIN, Value);
            }
            break;
        case 15:
            {
                res = hal_gpio_write_pin(DUT_CH15_PORT, DUT_CH15_PIN, Value);
            }
            break;
        default:
            {
                res = 2;
            }
            break;
    }
    return res;
}
/**
 * @brief :read CH status
 *
 * @param Ch:channal
 * @param Value:pointer of channal value
 *
 * @return :0 is success;other is failed
 */
static uint16_t
can_protocol_task_single_ch_read(uint8_t Ch, uint32_t *Value)
{
    uint16_t res = 1;
    switch (Ch)
    {
        case 0:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH0_PORT, DUT_CH0_PIN, Value);
            }
            break;
        case 1:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH1_PORT, DUT_CH1_PIN, Value);
            }
            break;
        case 2:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH2_PORT, DUT_CH2_PIN, Value);
            }
            break;
        case 3:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH3_PORT, DUT_CH3_PIN, Value);
            }
            break;
        case 4:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH4_PORT, DUT_CH4_PIN, Value);
            }
            break;
        case 5:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH5_PORT, DUT_CH5_PIN, Value);
            }
            break;
        case 6:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH6_PORT, DUT_CH6_PIN, Value);
            }
            break;
        case 7:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH7_PORT, DUT_CH7_PIN, Value);
            }
            break;
        case 8:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH8_PORT, DUT_CH8_PIN, Value);
            }
            break;
        case 9:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH9_PORT, DUT_CH9_PIN, Value);
            }
            break;
        case 10:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH10_PORT, DUT_CH10_PIN, Value);
            }
            break;
        case 11:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH11_PORT, DUT_CH11_PIN, Value);
            }
            break;
        case 12:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH12_PORT, DUT_CH12_PIN, Value);
            }
            break;
        case 13:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH13_PORT, DUT_CH13_PIN, Value);
            }
            break;
        case 14:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH14_PORT, DUT_CH14_PIN, Value);
            }
            break;
        case 15:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH15_PORT, DUT_CH15_PIN, Value);
            }
            break;
        default:break;
    }
    return res;
}


