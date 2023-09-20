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
#include "Services.h"
#include "Cp_Simulate/task_cp_simulate.h"
#include "Adc_Measure/task_adc_measure.h"


TsCanFrame gTsCanFrame = {};
static TsCanFrame *gTsCanFramePtr = NULL;
static uint8_t gu8SwitchOutputCtrlIOState = 0;

void
can_protocol_task_init(void)
{
    //can init
    hal_can_init(eCanPort_0);
    //service init
    di_service_input_io_init();
    switch_ctrl_service_output_io_init();
    cc_service_io_init();
    ntc_service_io_init();
    cp_service_init();
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
                                uint16_t ret = 0;
                                //关闭开关量输出
                                uint32_t u32Value = 0;
                                ret = switch_ctrl_service_output_ctrl(0, 0xFF, &u32Value);
                                if (ret)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_RESET;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                    break;
                                }
                                //取消NTC电阻模拟
                                ntc_service_io_reset();
                                //取消CC电阻模拟
                                cc_service_io_reset();
                                //取消CP模拟
                                cp_service_reset();
                                ret = ret;
                            }
                            break;
                        case CAN_PROTOCOL_CMD_SWITCH_OUTPUT_CTRL:
                            {
                                uint16_t ret = 0;
                                DoubleWordUnion doubleWordTmp = {0};
                                ret = switch_ctrl_service_output_ctrl(gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_SWITCH_OUTPUTCTRL_OFFSET],
                                        gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_SWITCH_OUTPUTCTRL_MASK_OFFSET], &doubleWordTmp.u32Data);
                                if (ret)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_SWITCH_OUTPUT_CTRL;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SWITCH_OUTPUTCTRL_OFFSET] = doubleWordTmp.u8Data[0];
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else
                                {
                                	gu8SwitchOutputCtrlIOState = doubleWordTmp.u8Data[0];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_SWITCH_OUTPUT_CTRL;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_SWITCH_OUTPUTCTRL_OFFSET] = doubleWordTmp.u8Data[0];
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                res = ret;
                            }
                            break;
                        case CAN_PROTOCOL_CMD_NTC_R_SIMULATION:
                            {
                                uint16_t ret = 0;
                                //NTC resistance
                                uint8_t u8ChState = 0; 
                                SingleWordUnion singleWordTmp,singleWordRValue;
                                memcpy(singleWordTmp.u8Data, &gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_NTC_R_OFFSET], sizeof(SingleWordUnion));
                                ret = ntc_service_r_ctrl(singleWordTmp.u16Data,
                                        gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_NTC_CH_STATE_OFFSET], &singleWordRValue.u16Data, &u8ChState);
                                if (ret)
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
                                res = ret;
                            }
                            break;
                        case CAN_PROTOCOL_CMD_CC_R_SIMULATION:
                            {
                                uint16_t ret = 0;
                                //cc2 and cc
                                uint8_t u8CC2State = 0; 
                                SingleWordUnion singleWordTmp,singleWordRValue;
                                memcpy(singleWordTmp.u8Data, &gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_CC_R_OFFSET], sizeof(SingleWordUnion));
                                ret = cc_service_r_ctrl(singleWordTmp.u16Data,
                                        gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_CC2_1K_R_PULLDOWN_OFFSET],
                                        gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_CC_MASK], &singleWordRValue.u16Data, &u8CC2State);
                                if (ret)
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
                                res = ret;
                            }
                            break;
                        case CAN_PROTOCOL_CMD_CP_SIMULATION:
                            {
                                //TODO
                                //FTM PWM func
                                uint8_t u8RealCPDuty;
                                uint16_t ret = 0;
                                uint8_t u8CPDuty = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_CP_DUTY_OFFSET]; 
                                SingleWordUnion singleWordCPFreq,singleWordRealCPFreq;
                                memcpy(singleWordCPFreq.u8Data, &gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_CP_FREQ_OFFSET], sizeof(SingleWordUnion));
                                ret = cp_service_ctrl(u8CPDuty, singleWordCPFreq.u16Data,
                                        gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_PARAMETR+CAN_PROTOCOL_CP_MASK_OFFSET], &u8RealCPDuty, &singleWordRealCPFreq.u16Data);
                                if (0 != ret)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_CP_SIMULATION;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_CP_SIMULATION;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_CP_DUTY_OFFSET] = u8RealCPDuty;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_CP_FREQ_OFFSET], &singleWordRealCPFreq.u8Data, sizeof(SingleWordUnion));
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                res = ret;
                            }
                            break;
                        case CAN_PROTOCOL_CMD_CP_VOL_QUERY:
                            {
                                uint16_t res = 0;
                                uint16_t u16VolValue[ADC_MEASURE_MAX_GROUP0];
                                res = adc_measure_task_output_vol(u16VolValue);
                                if (res)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_CP_VOL_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else
                                {
                                    //ADC
                                    SingleWordUnion singleWordTmp = {0};
                                    singleWordTmp.u16Data = u16VolValue[ADC_CP_INDEX];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_CP_VOL_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    memcpy(&gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR], &singleWordTmp.u8Data, sizeof(SingleWordUnion));
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                            }
                            break;
                        case CAN_PROTOCOL_CMD_INPUT_PORT_QUERY:
                            {
                                uint16_t ret = 0;
                                DoubleWordUnion doubleWordDI0,doubleWordDI1;
                                //DI
                                ret = di_service_input_io_read(&doubleWordDI0.u32Data, &doubleWordDI1.u32Data);
                                //TODO:ADC
                                uint16_t u16VolValueHSDStr[ADC_MEASURE_MAX_GROUP1] = {0};
                                uint16_t u16VolValueLSDStr[ADC_MEASURE_MAX_GROUP2] = {0};
                                ret = adc_measure_task_vol_get(ADC_HSD_GROUP, u16VolValueHSDStr, ADC_MEASURE_MAX_GROUP1);
                                ret = adc_measure_task_vol_get(ADC_LSD_GROUP, u16VolValueLSDStr, ADC_MEASURE_MAX_GROUP2);
                                if (ret)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_INPUT_PORT_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else
                                {
                                    uint8_t u8BitValue = 0;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_INPUT_PORT_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    if (doubleWordDI0.u32Data)
                                        RESET_NUM_BIT(u8BitValue, DI_0_BIT);
                                    else
                                        SET_NUM_BIT(u8BitValue, DI_0_BIT);
                                    if (doubleWordDI1.u32Data)
                                        RESET_NUM_BIT(u8BitValue, DI_1_BIT);
                                    else
                                        SET_NUM_BIT(u8BitValue, DI_1_BIT);
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_DI_VALUE_OFFSET] = u8BitValue;
                                    for (uint8_t index=0;index<LSD_MAX_BIT;index++)
                                    {
                                        if (u16VolValueLSDStr[index] > 0xFF)
                                            RESET_NUM_BIT(u8BitValue, index);
                                        else
                                            SET_NUM_BIT(u8BitValue, index);
                                    }
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_LSD_STATE_OFFSET] = u8BitValue;
                                    for (uint8_t index=0;index<HSD_MAX_BIT;index++)
                                    {
                                        if (u16VolValueHSDStr[index])
                                            SET_NUM_BIT(u8BitValue, index);
                                        else
                                            RESET_NUM_BIT(u8BitValue, index);
                                    }
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_HSD_STATE_OFFSET] = u8BitValue;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                res = ret;
                            }
                            break;
                        case CAN_PROTOCOL_CMD_LSD_CURRENT_QUERY:
                            {
                                //I=U/R
                                uint16_t ret = 0;
                                uint8_t u8LSDState[CAN_PROTOCOL_LSD_MAX_CURRENT_NUM] = {0};
                                uint16_t u16VolValueLSDStr[ADC_MEASURE_MAX_GROUP2];
                                ret = adc_measure_task_vol_get(HAL_ADC_GROUP_2, u16VolValueLSDStr, ADC_MEASURE_MAX_GROUP2);
                                if (ret)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_LSD_CURRENT_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_LSD_CURRENT_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    for (uint8_t index=0;index < CAN_PROTOCOL_LSD_MAX_CURRENT_NUM;index++)
                                    {
                                        if (u16VolValueLSDStr[index] > 0xFF)
                                        {
                                            u8LSDState[index] = CAN_PROTOCOL_LSD_I_VALUE;
                                        }
                                    }
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_LSD_0_CURRENT_OFFSET] = u8LSDState[CAN_PROTOCOL_LSD_0_CURRENT_OFFSET];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_LSD_1_CURRENT_OFFSET] = u8LSDState[CAN_PROTOCOL_LSD_1_CURRENT_OFFSET];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_LSD_2_CURRENT_OFFSET] = u8LSDState[CAN_PROTOCOL_LSD_2_CURRENT_OFFSET];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_LSD_3_CURRENT_OFFSET] = u8LSDState[CAN_PROTOCOL_LSD_3_CURRENT_OFFSET];
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                            }
                            break;
                        case CAN_PROTOCOL_CMD_HSD_CURRENT_QUERY:
                            {
                                //I=U/R
                                uint16_t ret = 0;
                            	uint16_t u16VolValueHSDStr[ADC_MEASURE_MAX_GROUP1];
                                ret = adc_measure_task_vol_get(HAL_ADC_GROUP_1, u16VolValueHSDStr, ADC_MEASURE_MAX_GROUP1);
                                if (ret)
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_HSD_CURRENT_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_ERR;
                                    (void)CAN_FIFO_Write(eCanPort_0, &gTsCanFrame);
                                }
                                else
                                {
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER] = gTsCanFramePtr->data[CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER];
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_CMD] = CAN_PROTOCOL_CMD_HSD_CURRENT_QUERY;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_RESULTS] = CAN_PROTOCOL_RESP_RESULTS_OK;
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_HSD_0_CURRENT_OFFSET] = (uint8_t)(((uint16_t)u16VolValueHSDStr[CAN_PROTOCOL_HSD_0_CURRENT_OFFSET]/(uint16_t)CAN_PROTOCOL_HSD_R_VALUE));
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_HSD_1_CURRENT_OFFSET] = (uint8_t)(((uint16_t)u16VolValueHSDStr[CAN_PROTOCOL_HSD_1_CURRENT_OFFSET]/(uint16_t)CAN_PROTOCOL_HSD_R_VALUE));
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_HSD_2_CURRENT_OFFSET] = (uint8_t)(((uint16_t)u16VolValueHSDStr[CAN_PROTOCOL_HSD_2_CURRENT_OFFSET]/(uint16_t)CAN_PROTOCOL_HSD_R_VALUE));
                                    gTsCanFrame.data[CAN_PROTOCOL_RESP_DATA_PARAMETR+CAN_PROTOCOL_HSD_3_CURRENT_OFFSET] = (uint8_t)(((uint16_t)u16VolValueHSDStr[CAN_PROTOCOL_HSD_3_CURRENT_OFFSET]/(uint16_t)CAN_PROTOCOL_HSD_R_VALUE));
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
    return res;
}

