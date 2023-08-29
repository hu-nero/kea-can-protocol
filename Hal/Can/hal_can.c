/*
 * hal_can.c
 *
 *  Created on: 2023年1月30日
 *      Author: xinlei.hu
 */
#include "hal_can.h"
#include "string.h"
#include "CAN.h"
#include "hal_systick.h"

uint8_t canRxBuf[CAN_DRIVER_RX_NUM][CAN_DRIVER_MAX_DLC];

static CAN_TDeviceDataPtr halCanDevicePtr = NULL;
HAL_CAN_Tx_Struct gHalCanTxStruct;
HAL_CAN_Rx_Struct gHalCanRxStruct;
uint32_t gHalCanErrorCount;

uint16_t
hal_can_init(void)
{
    //init can TxMsg
    for( uint8_t i=0; i<CAN_DRIVER_RX_NUM; i++)
    {
        gHalCanRxStruct.halCanRxFrame[i].u8Data = canRxBuf[i];
    }
    gHalCanTxStruct.u8MsgId = 0;
    gHalCanRxStruct.u8MsgId = 0;
    gHalCanTxStruct.u32CanTxCount = 0;
    gHalCanRxStruct.u32CanRxCount = 0;
    gHalCanErrorCount = 0;
    //init can device
    halCanDevicePtr = CAN_Init(NULL);
    if(NULL == halCanDevicePtr)
        return 1;
    return 0;
}

uint16_t
hal_can_deinit(void)
{
    //deinit can device
    if(NULL == halCanDevicePtr)
        return 1;
    CAN_Deinit(halCanDevicePtr);
    return 0;
}

uint16_t
hal_can_transmit(uint32_t u32CanId, HAL_CAN_TFrameType hType, uint8_t u8Dlc, const uint8_t *u8BufPtr)
{
	LDD_CAN_TFrame frame = {0};
	uint8_t u8CanTxBuf[8];
    uint16_t u16TimeOut = 0;

    frame.MessageID = u32CanId;
    frame.FrameType = hType;
    frame.Length = u8Dlc <= 8 ? u8Dlc : 8;
    frame.Data = u8CanTxBuf;
    memcpy(frame.Data, u8BufPtr, frame.Length);
    if(0 == CAN_SendFrame(halCanDevicePtr, 1, &frame))
    {
        /* Wait for the transmission done */
        while((u16TimeOut < 10) && (0 != CAN_GetTxFrameState(halCanDevicePtr, 1)))
        {
            //最长等待10ms
//            hal_systick_delay_ms(1);
            u16TimeOut++;
        }
        return 0;
    }
    return 1;
}

void
hal_can_error_callback(HAL_CAN_ER_TYPE u8CanErrorType)
{
    switch(u8CanErrorType)
    {
        case HAL_CAN_ER_ERROR:
            {
                //获取错误状态，并记录日志或打印
                uint32_t u32ErrorMask = 0;
                CAN_GetError(halCanDevicePtr, &u32ErrorMask);
                gHalCanErrorCount ++;
                //printf("[CAN] Error state:%x;\r\n",u32ErrorMask);
            }
            break;
        case HAL_CAN_ER_BUSOFF:
            {
                //重启CAN controller
                hal_can_deinit();
                hal_can_init();
            }
            break;
        default:break;
    }
}

void
hal_can_tx_callback(uint8_t BufferIdx)
{
	(void) BufferIdx;
    gHalCanTxStruct.u32CanTxCount++;
    //TODO:transmit next msg
}

void
hal_can_rx_callback(uint8_t BufferIdx)
{
    //Msg is overflow
    if(gHalCanRxStruct.u8MsgId > CAN_DRIVER_RX_NUM-1)
    {
        //TODO:alert user
        //discard Msg
        return;
    }
    (void)CAN_ReadFrame(halCanDevicePtr, BufferIdx,(LDD_CAN_TFrame *)&gHalCanRxStruct.halCanRxFrame[gHalCanRxStruct.u8MsgId]);
    gHalCanRxStruct.u8MsgId++;
    gHalCanRxStruct.u32CanRxCount++;
}



