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
static uint32_t gu32Channal = 0;

static uint16_t can_protocol_task_assignctrls(uint32_t Ch, uint32_t Mask, uint32_t *Value);
static uint16_t can_protocol_task_single_ch_read(uint8_t Ch, uint32_t *Value);
static uint16_t can_protocol_task_single_assignctrl(uint8_t Ch, uint8_t Value);

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
    hal_gpio_set_pin_dir(DUT_CH16_PORT, DUT_CH16_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH17_PORT, DUT_CH17_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH18_PORT, DUT_CH18_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH19_PORT, DUT_CH19_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH20_PORT, DUT_CH20_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH21_PORT, DUT_CH21_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH22_PORT, DUT_CH22_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
    hal_gpio_set_pin_dir(DUT_CH23_PORT, DUT_CH23_PIN, HAL_GPIO_OUTPUT_DIRECTION, 0);
}

uint16_t
can_protocol_task(void)
{
    //(void)CAN_FIFO_Write(0, &gTsCanFrame);
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
                                DoubleWordUnion timetickTmp;
                                timetickTmp.u32Data = hal_timer_get_time();
                                //TODO:返回软件版本和开机时间
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_SYSTEM_INFO_REQUEST;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SYSINFO_VERSION_OFFSET] = SoftsVer[10];
                                memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SYSINFO_STARTTIME_OFFSET], timetickTmp.u8Data, sizeof(timetickTmp));
                                (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                            }
                            break;
                        case CAN_PROTOCOL_CMD_ASSIGN_CTRL:
                            {
                                DoubleWordUnion doubleWordChCtrl,doubleWordChMask,doubleWordPara;
                                memcpy(doubleWordChCtrl.u8Data, &gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CHCTRUL_OFFSET], sizeof(DoubleWordUnion));
                                memcpy(doubleWordChMask.u8Data, &gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CHMASK_OFFSET], sizeof(DoubleWordUnion));
                                res = can_protocol_task_assignctrls(doubleWordChCtrl.u32Data, doubleWordChMask.u32Data, &doubleWordPara.u32Data);
                                //operation failed
                                if (res)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_ASSIGN_CTRL;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CHCTRUL_OFFSET], &doubleWordPara.u8Data, sizeof(DoubleWordUnion)-1);
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                //operation success
                                else
                                {
                                    gu32Channal = doubleWordPara.u32Data;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_ASSIGN_CTRL;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CHCTRUL_OFFSET], &doubleWordPara.u8Data, sizeof(DoubleWordUnion)-1);
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
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
 * @brief :assign ctrl
 *
 * @param Ch:channal
 * @param Mask:channal mask
 * @param Value:pointer of channal read value
 *
 * @return :0 is success;other is failed
 */
static uint16_t
can_protocol_task_assignctrls(uint32_t Ch, uint32_t Mask, uint32_t *Value)
{
    uint16_t res = 0;
    uint32_t u32Value = 0;
    //Perform the disconnect operation first
    for (uint8_t bit=0;bit<CAN_PROTOCOL_ASSIGNCTRL_CHSIZE;bit++)
    {
        if (CAT_NUM_BIT(Mask, bit))
        {
            if (CAT_NUM_BIT(Ch, bit) == 0)
            {
                //ctrl ch
                can_protocol_task_single_assignctrl(bit, 0);
                //set return bit
                if ((res = can_protocol_task_single_ch_read(bit, &u32Value)) !=0)
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
        }
    }
    //Perform the disconnect operation first
    for (uint8_t bit=0;bit<CAN_PROTOCOL_ASSIGNCTRL_CHSIZE;bit++)
    {
        if (CAT_NUM_BIT(Mask, bit))
        {
            if (CAT_NUM_BIT(Ch, bit) == 1)
            {
                //ctrl ch
                can_protocol_task_single_assignctrl(bit, 1);
                //set return bit
                if ((res = can_protocol_task_single_ch_read(bit, &u32Value)) !=0)
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
        }
    }
    return res;
}

/**
 * @brief :set single ch
 *
 * @param Ch:channal
 * @param Value:IO value
 *
 * @return :0 is success;other is failed
 */
static uint16_t
can_protocol_task_single_assignctrl(uint8_t Ch, uint8_t Value)
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
        case 16:
            {
                res = hal_gpio_write_pin(DUT_CH16_PORT, DUT_CH16_PIN, Value);
            }
            break;
        case 17:
            {
                res = hal_gpio_write_pin(DUT_CH17_PORT, DUT_CH17_PIN, Value);
            }
            break;
        case 18:
            {
                res = hal_gpio_write_pin(DUT_CH18_PORT, DUT_CH18_PIN, Value);
            }
            break;
        case 19:
            {
                res = hal_gpio_write_pin(DUT_CH19_PORT, DUT_CH19_PIN, Value);
            }
            break;
        case 20:
            {
                res = hal_gpio_write_pin(DUT_CH20_PORT, DUT_CH20_PIN, Value);
            }
            break;
        case 21:
            {
                res = hal_gpio_write_pin(DUT_CH21_PORT, DUT_CH21_PIN, Value);
            }
            break;
        case 22:
            {
                res = hal_gpio_write_pin(DUT_CH22_PORT, DUT_CH22_PIN, Value);
            }
            break;
        case 23:
            {
                res = hal_gpio_write_pin(DUT_CH23_PORT, DUT_CH23_PIN, Value);
            }
            break;
        default:break;
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
        case 16:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH16_PORT, DUT_CH16_PIN, Value);
            }
            break;
        case 17:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH17_PORT, DUT_CH17_PIN, Value);
            }
            break;
        case 18:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH18_PORT, DUT_CH18_PIN, Value);
            }
            break;
        case 19:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH19_PORT, DUT_CH19_PIN, Value);
            }
            break;
        case 20:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH20_PORT, DUT_CH20_PIN, Value);
            }
            break;
        case 21:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH21_PORT, DUT_CH21_PIN, Value);
            }
            break;
        case 22:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH22_PORT, DUT_CH22_PIN, Value);
            }
            break;
        case 23:
            {
                res = hal_gpio_get_pin_output_value(DUT_CH23_PORT, DUT_CH23_PIN, Value);
            }
            break;
        default:break;
    }
    return res;
}


