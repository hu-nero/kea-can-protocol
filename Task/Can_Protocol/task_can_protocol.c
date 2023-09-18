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
//static uint8_t gu8Channal = 0xFF;
//
//static uint16_t can_protocol_task_ch_assignctrls(uint8_t PreCh, uint8_t CurCh);
//static uint16_t can_protocol_task_single_ch_assignctrl(uint8_t Ch, uint8_t Value);
//static uint16_t can_protocol_task_single_ch_read(uint8_t Ch, uint32_t *Value);

void
can_protocol_task_init(void)
{
    gTsCanFrame.id = CAN_PROTOCOL_ID_RESPONSE;
    gTsCanFrame.dlc = 8;
    gTsCanFrame.is_can_fd = 0;
    gTsCanFrame.is_ext_id = 1;
    memset(gTsCanFrame.data, 0, 8);

}

uint16_t
can_protocol_task(void)
{
    //解析CAN报文
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
                                //uint8_t u8Channal = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET];
                                //res = can_protocol_task_ch_assignctrls(gu8Channal, u8Channal);
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

