/*
 * task_can_protocol.c
 *
 *  Created on: 2023年8月29日
 *      Author: xinlei.hu
 */
#include "task_can_protocol.h"
#include <string.h>
#include "Hal.h"
#include "board_init.h"
#include "board_config.h"


TsCanFrame gTsCanFrame = {};
static TsCanFrame *gTsCanFramePtr = NULL;
static uint8_t gu8Channal = 0xFF;

static uint16_t can_protocol_task_ch_assignctrls(uint8_t PreCh, uint8_t CurCh);
static uint16_t can_protocol_task_single_ch_assignctrl(uint8_t Ch, uint8_t Value);
static uint16_t can_protocol_task_single_ch_read(uint8_t Ch, uint32_t *Value);

void
can_protocol_task_init(void)
{
    gTsCanFrame.id = gBootInfo.u32CanResponseId;
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
    //(void)CAN_FIFO_Write(0, &gTsCanFrame);
    static bool bIsCalibrateFlag = true;
    static bool bCalibrateFlag = false;
    static uint16_t u8CalibrateCount = 0;
    uint16_t res = 0;
    if (bCalibrateFlag)
    {
        if (u8CalibrateCount < 2000) //20s
        {
            u8CalibrateCount ++;
        }
        else
        {
            CPU_SystemReset();
        }
    }
    else
    {
        if (u8CalibrateCount < 5) //50ms
        {
            u8CalibrateCount ++;
        }
        else
        {
            if (bIsCalibrateFlag == true)
                bIsCalibrateFlag = false;
        }
    }


    //解析CAN报文
    if ((gTsCanFramePtr = hal_can_rx_queue_de()) != NULL)
    {
        memset(gTsCanFrame.data, 0, 8);
        //calibration mode
        if (bIsCalibrateFlag)
        {
            if (gTsCanFramePtr->id == CAN_CALIBRATE_ID_REQUEST)
            {
                if ((gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_CMD] == CAN_CALIBRATE_CMD_ENTER) &&
                    (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR] == 0xAA) &&
                    (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+1] == 0xBB) &&
                    (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+2] == 0xCC) &&
                    (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+3] == 0xDD) &&
                    (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+4] == 0xEE) &&
                    (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+5] == 0xFF))
                {
                    bCalibrateFlag = true;
                }
                if (bCalibrateFlag)
                {
                    //reset counter
                    u8CalibrateCount = 0;
                    switch (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_CMD])
                    {
                        case CAN_CALIBRATE_CMD_EXIT:
                            {
                            	CPU_SystemReset();
                            }
                            break;
                        case CAN_CALIBRATE_CMD_CAN_BAUD_SET:
                            {
                                SingleWordUnion singleWordTmp = {0};
                                memcpy(singleWordTmp.u8Data, &gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR], sizeof(SingleWordUnion));
                                switch (singleWordTmp.u16Data)
                                {
                                    case CAN_REAL_BAUD_125:
                                        {
                                            gBootInfo.u32CanBaudValue = CAN_BAUD_125;
                                        }
                                        break;
                                    case CAN_REAL_BAUD_250:
                                        {
                                            gBootInfo.u32CanBaudValue = CAN_BAUD_250;
                                        }
                                        break;
                                    case CAN_REAL_BAUD_500:
                                        {
                                            gBootInfo.u32CanBaudValue = CAN_BAUD_500;
                                        }
                                        break;
                                    case CAN_REAL_BAUD_1000:
                                        {
                                            gBootInfo.u32CanBaudValue = CAN_BAUD_1000;
                                        }
                                        break;
                                    default:
                                        {
                                            gBootInfo.u32CanBaudValue = CAN_BAUD_500;
                                        }
                                        break;
                                }

                                //write flash
                                hal_flash_erase(HAL_DEV_FLASH0, CALI_BASE_ADDR);
                                uint8_t len = sizeof(gBootInfo);
                                uint32_t addr = CALI_BASE_ADDR;
                                uint8_t *ptr = (uint8_t*)&gBootInfo;
                                while (len)
                                {
                                    if (len >= FLASHOPERATEUINT)
                                    {
                                        len -= FLASHOPERATEUINT;
                                        hal_flash_write(HAL_DEV_FLASH0, ptr, addr, FLASHOPERATEUINT);
                                        ptr += FLASHOPERATEUINT;
                                        addr += FLASHOPERATEUINT;
                                    }
                                    else
                                    {
                                        hal_flash_write(HAL_DEV_FLASH0, ptr, addr, len);
                                        len = 0;
                                    }
                                }
                                //send can msg
                                gTsCanFrame.id = CAN_CALIBRATE_ID_RESPONSE;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_CALIBRATE_CMD_CAN_BAUD_SET;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_CALIBRATE_BAUD_SET_OFFSET], singleWordTmp.u8Data, sizeof(singleWordTmp));
                                (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                            }
                            break;
                        case CAN_CALIBRATE_CMD_BUSINESS_COMMUNICATE_RECV_ID:
                            {
                                DoubleWordUnion doubleWordTmp = {0};
                                if (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_CALIBRATE_BUSSINESS_COMMUNICATE_RECV_CMD_OFFSET] == BUSSINESS_COMMUNICATE_RECV_ID_QUERY)
                                {
                                    doubleWordTmp.u32Data = gBootInfo.u32CanResponseId;
                                    //send can msg
                                    gTsCanFrame.id = CAN_CALIBRATE_ID_RESPONSE;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_CALIBRATE_CMD_BUSINESS_COMMUNICATE_RECV_ID;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR], doubleWordTmp.u8Data, sizeof(doubleWordTmp));
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else if (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_CALIBRATE_BUSSINESS_COMMUNICATE_RECV_CMD_OFFSET] == BUSSINESS_COMMUNICATE_RECV_ID_SET)
                                {
                                    memcpy(doubleWordTmp.u8Data, &gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_CALIBRATE_BUSSINESS_COMMUNICATE_RECV_ID_OFFSET], sizeof(doubleWordTmp));
                                    gBootInfo.u32CanResponseId = doubleWordTmp.u32Data;

                                    //write flash
                                    hal_flash_erase(HAL_DEV_FLASH0, CALI_BASE_ADDR);
                                    uint8_t len = sizeof(gBootInfo);
                                    uint32_t addr = CALI_BASE_ADDR;
                                    uint8_t *ptr = (uint8_t*)&gBootInfo;
                                    while (len)
                                    {
                                        if (len >= FLASHOPERATEUINT)
                                        {
                                            len -= FLASHOPERATEUINT;
                                            hal_flash_write(HAL_DEV_FLASH0, ptr, addr, FLASHOPERATEUINT);
                                            ptr += FLASHOPERATEUINT;
                                            addr += FLASHOPERATEUINT;
                                        }
                                        else
                                        {
                                            hal_flash_write(HAL_DEV_FLASH0, ptr, addr, len);
                                            len = 0;
                                        }
                                    }
                                    //send can msg
                                    gTsCanFrame.id = CAN_CALIBRATE_ID_RESPONSE;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_CALIBRATE_CMD_BUSINESS_COMMUNICATE_RECV_ID;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR], doubleWordTmp.u8Data, sizeof(doubleWordTmp));
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                            }
                            break;
                        case CAN_CALIBRATE_CMD_BUSINESS_COMMUNICATE_SEND_ID:
                            {
                                DoubleWordUnion doubleWordTmp = {0};
                                if (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_CALIBRATE_BUSSINESS_COMMUNICATE_SEND_CMD_OFFSET] == BUSSINESS_COMMUNICATE_SEND_ID_QUERY)
                                {
                                    doubleWordTmp.u32Data = gBootInfo.u32CanRequestId;
                                    //send can msg
                                    gTsCanFrame.id = CAN_CALIBRATE_ID_RESPONSE;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_CALIBRATE_CMD_BUSINESS_COMMUNICATE_SEND_ID;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR], doubleWordTmp.u8Data, sizeof(doubleWordTmp));
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else if (gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_CALIBRATE_BUSSINESS_COMMUNICATE_SEND_CMD_OFFSET] == BUSSINESS_COMMUNICATE_SEND_ID_SET)
                                {
                                    memcpy(doubleWordTmp.u8Data, &gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_CALIBRATE_BUSSINESS_COMMUNICATE_SEND_ID_OFFSET], sizeof(doubleWordTmp));
                                    gBootInfo.u32CanRequestId = doubleWordTmp.u32Data;

                                    //write flash
                                    hal_flash_erase(HAL_DEV_FLASH0, CALI_BASE_ADDR);
                                    uint8_t len = sizeof(gBootInfo);
                                    uint32_t addr = CALI_BASE_ADDR;
                                    uint8_t *ptr = (uint8_t*)&gBootInfo;
                                    while (len)
                                    {
                                        if (len >= FLASHOPERATEUINT)
                                        {
                                            len -= FLASHOPERATEUINT;
                                            hal_flash_write(HAL_DEV_FLASH0, ptr, addr, FLASHOPERATEUINT);
                                            ptr += FLASHOPERATEUINT;
                                            addr += FLASHOPERATEUINT;
                                        }
                                        else
                                        {
                                            hal_flash_write(HAL_DEV_FLASH0, ptr, addr, len);
                                            len = 0;
                                        }
                                    }
                                    //send can msg
                                    gTsCanFrame.id = CAN_CALIBRATE_ID_RESPONSE;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_CALIBRATE_CMD_BUSINESS_COMMUNICATE_SEND_ID;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR], doubleWordTmp.u8Data, sizeof(doubleWordTmp));
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                            }
                            break;
                        default:break;
                    }
                }
            }
        }
        else
        {
            //normal mode
            if (gTsCanFramePtr->id == gBootInfo.u32CanRequestId)
            {
                gTsCanFrame.id = gBootInfo.u32CanResponseId;
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
                            uint8_t u8Channal = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET];
                            res = can_protocol_task_ch_assignctrls(gu8Channal, u8Channal);
                            if (res == 0)
                            {
                                gu8Channal = u8Channal;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_ASSIGN_CTRL;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET] = u8Channal;
                                (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                            }
                            //都不选通
                            else if (res == 4)
                            {
                                gu8Channal = 0xFF;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_ASSIGN_CTRL;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET] = 0xFF;
                                (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                return 0;
                            }
                            //选通失败
                            else if (res)
                            {
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_ASSIGN_CTRL;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET] = gu8Channal;
                                (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                return 0;
                            }
                        }
                        break;
                    default:break;
                }
            }
        }
    }
    return res;
}

/**
 * @brief :ch assign ctrl
 *
 * @param PreCh:previous ch
 * @param CurCh:current ch
 *
 * @return :0 is success;4 is select none;other is failed
 */
static uint16_t
can_protocol_task_ch_assignctrls(uint8_t PreCh, uint8_t CurCh)
{
    uint16_t res = 0;
    uint32_t u32Value = 0;

    //当前值为有效值
    if ((CurCh >= 0) && (CurCh < DUT_CH_NUM))
    {
        //保存值为有效值
        if ((PreCh >= 0) && (PreCh < DUT_CH_NUM))
        {
            if (PreCh != CurCh)
            {
                can_protocol_task_single_ch_assignctrl(PreCh, 0);
                can_protocol_task_single_ch_read(PreCh, &u32Value);
                if (u32Value)
                {
                    res = 1;
                }
            }
            can_protocol_task_single_ch_assignctrl(CurCh, 1);
            can_protocol_task_single_ch_read(CurCh, &u32Value);
            if (u32Value != 1)
            {
                res = 2;
            }
        }
        //保存值为无效值
        else
        {
            can_protocol_task_single_ch_assignctrl(CurCh, 1);
            can_protocol_task_single_ch_read(CurCh, &u32Value);
            if (u32Value != 1)
            {
                res = 3;
            }
        }
    }
    //当前值为无效值
    else
    {
        //保存值为有效值,取消选通
        if ((PreCh >= 0) && (PreCh < DUT_CH_NUM))
        {
            can_protocol_task_single_ch_assignctrl(PreCh, 0);
            can_protocol_task_single_ch_read(PreCh, &u32Value);
            if (u32Value == 0)
            {
                res = 4;
            }
            else
            {
                res = 5;
            }
        }
        //保存值为无效值
        else
        {
            ;
        }
    }
    return res;
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


