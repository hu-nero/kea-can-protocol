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
#include "board_pin_config.h"

#include "Cp_Simulate/task_cp_simulate.h"
#include "Adc_Measure/task_adc_measure.h"


TsCanFrame gTsCanFrame = {};
static TsCanFrame *gTsCanFramePtr = NULL;
static uint8_t gu8SwitchOutputCtrlIOState = 0;

void
can_protocol_task_init(void)
{
    //switch output ctrl init
    board_pin_switch_output_ctrl_init();
    //input init
    board_pin_input_di_init();
    board_pin_ntc_io_init();
    board_pin_cc_io_init();
    //CAN send buffer
    gTsCanFrame.id = CAN_PROTOCOL_ID_RESPONSE;
    gTsCanFrame.dlc = 8;
    gTsCanFrame.is_can_fd = 0;
    gTsCanFrame.is_ext_id = 1;
    memset(gTsCanFrame.data, 0, 8);
}

uint16_t
can_protocol_task(void)
{
    //(void)CAN_FIFO_Write(0, &gTsCanFrame);
#if 1
    //解析CAN报文
    uint16_t res = 0;
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
                                DoubleWordUnion doubleWordTmp = {0};
                                doubleWordTmp.u32Data = hal_timer_get_time();
                                //TODO:返回软件版本和开机时间
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_SYSTEM_INFO_REQUEST;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SYSINFO_VERSION_OFFSET] = SoftsVer[10];
                                memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SYSINFO_STARTTIME_OFFSET], doubleWordTmp.u8Data, sizeof(doubleWordTmp));
                                (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                            }
                            break;
                        case CAN_PROTOCOL_CMD_RESET:
                            {
                            }
                            break;
                        case CAN_PROTOCOL_CMD_SWITCH_OUTPUT_CTRL:
                            {
                                DoubleWordUnion doubleWordTmp = {0};
                                res = board_pin_switch_output_ctrl(gTsCanFramePtr->data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SWITCH_OUTPUTCTRL_OFFSET],
                                        gTsCanFramePtr->data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SWITCH_OUTPUTCTRL_MASK_OFFSET], &doubleWordTmp.u32Data);
                                if (res)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_RESET;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SWITCH_OUTPUTCTRL_OFFSET] = doubleWordTmp.u8Data[0];
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else
                                {
                                	gu8SwitchOutputCtrlIOState = doubleWordTmp.u8Data[0];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_RESET;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SWITCH_OUTPUTCTRL_OFFSET] = doubleWordTmp.u8Data[0];
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                            }
                            break;
                        case CAN_PROTOCOL_CMD_NTC_R_SIMULATION:
                            {
                                //NTC resistance
                                uint8_t u8ChState = 0; 
                                SingleWordUnion singleWordTmp,singleWordRValue;
                                memcpy(singleWordTmp.u8Data, &gTsCanFramePtr->data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_NTC_R_OFFSET], sizeof(SingleWordUnion));
                                res = board_pin_ntc_r_ctrl(singleWordTmp.u16Data,
                                        gTsCanFramePtr->data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_NTC_CH_STATE_OFFSET], &singleWordRValue.u16Data, &u8ChState);
                                if (res)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_NTC_R_SIMULATION;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_NTC_R_OFFSET], singleWordRValue.u8Data, sizeof(SingleWordUnion));
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_NTC_CH_STATE_OFFSET] = u8ChState;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_NTC_R_SIMULATION;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_NTC_R_OFFSET], singleWordRValue.u8Data, sizeof(SingleWordUnion));
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_NTC_CH_STATE_OFFSET] = u8ChState;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                            }
                            break;
                        case CAN_PROTOCOL_CMD_CC_R_SIMULATION:
                            {
                                //cc2 and cc
                                uint8_t u8CC2State = 0; 
                                SingleWordUnion singleWordTmp,singleWordRValue;
                                memcpy(singleWordTmp.u8Data, &gTsCanFramePtr->data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_CC_R_OFFSET], sizeof(SingleWordUnion));
                                res = board_pin_cc_r_ctrl(singleWordTmp.u16Data,
                                        gTsCanFramePtr->data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_CC2_1K_R_PULLDOWN_OFFSET],
                                        gTsCanFramePtr->data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_CC_MASK], &singleWordRValue.u16Data, &u8CC2State);
                                if (res)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_CC_R_SIMULATION;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_CC_R_OFFSET], &singleWordRValue.u8Data, sizeof(SingleWordUnion));
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_CC2_1K_R_PULLDOWN_OFFSET] = u8CC2State;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_CC_R_SIMULATION;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_CC_R_OFFSET], &singleWordRValue.u8Data, sizeof(SingleWordUnion));
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_CC2_1K_R_PULLDOWN_OFFSET] = u8CC2State;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                            }
                            break;
                        case CAN_PROTOCOL_CMD_CP_SIMULATION:
                            {
                                //TODO
                                //FTM PWM func
                            }
                            break;
                        case CAN_PROTOCOL_CMD_CP_Vol_QUERY:
                            {
                                uint16_t *u16VolValuePtr;
                                u16VolValuePtr = adc_measure_task_vol_get(HAL_ADC_GROUP_0);
                                if (u16VolValuePtr == NULL)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_CP_Vol_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else
                                {
                                    //ADC
                                    SingleWordUnion singleWordTmp = {0};
                                    singleWordTmp.u16Data = u16VolValuePtr[0];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_CP_Vol_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR], &singleWordTmp.u8Data, sizeof(SingleWordUnion));
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                            }
                            break;
                        case CAN_PROTOCOL_CMD_INPUT_PORT_QUERY:
                            {
                                DoubleWordUnion doubleWordDI0,doubleWordDI1;
                                //DI
                                res = board_pin_di_read(&doubleWordDI0.u32Data, &doubleWordDI1.u32Data);
                                //TODO:ADC
                                uint16_t *u16VolValueGroup1Ptr, *u16VolValueGroup2Ptr;
                                u16VolValueGroup1Ptr = adc_measure_task_vol_get(HAL_ADC_GROUP_1);
                                u16VolValueGroup2Ptr = adc_measure_task_vol_get(HAL_ADC_GROUP_2);
                                if ((res != 0) || (u16VolValueGroup1Ptr == NULL) || (u16VolValueGroup2Ptr == NULL))
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_CP_Vol_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else
                                {
                                    uint8_t u8BitValue = 0;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_CP_Vol_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    if (doubleWordDI0.u32Data)
                                        SET_NUM_BIT(u8BitValue, DI_0_BIT);
                                    else
                                        RESET_NUM_BIT(u8BitValue, DI_0_BIT);
                                    if (doubleWordDI1.u32Data)
                                        SET_NUM_BIT(u8BitValue, DI_1_BIT);
                                    else
                                        RESET_NUM_BIT(u8BitValue, DI_1_BIT);
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_DI_VALUE_OFFSET] = u8BitValue;
                                    for (uint8_t index=0;index<LSD_MAX_BIT;index++)
                                    {
                                        if (u16VolValueGroup2Ptr[index])
                                            SET_NUM_BIT(u8BitValue, index);
                                        else
                                            RESET_NUM_BIT(u8BitValue, index);
                                    }
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_CMD_LSD_CURRENT_QUERY] = u8BitValue;
                                    for (uint8_t index=0;index<HSD_MAX_BIT;index++)
                                    {
                                        if (u16VolValueGroup1Ptr[index])
                                            SET_NUM_BIT(u8BitValue, index);
                                        else
                                            RESET_NUM_BIT(u8BitValue, index);
                                    }
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_CMD_HSD_CURRENT_QUERY] = u8BitValue;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                            }
                            break;
                        case CAN_PROTOCOL_CMD_LSD_CURRENT_QUERY:
                            {
                            }
                            break;
                        case CAN_PROTOCOL_CMD_HSD_CURRENT_QUERY:
                            {
                            }
                            break;
                        default:break;
                    }
                }
                break;
            default:break;
        }
    }
#endif
}
