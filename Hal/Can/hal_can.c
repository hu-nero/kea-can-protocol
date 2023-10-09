/*
 * hal_can.c
 *
 *  Created on: 2023年1月30日
 *      Author: xinlei.hu
 */
#include "hal_can.h"
#include "string.h"
#include "hal_systick.h"

static CAN_TDeviceDataPtr halCanDevicePtr = NULL;
static uint8_t canRxBuf[CAN_DRIVER_RX_NUM][CAN_DRIVER_MAX_DLC];
static HAL_CAN_Rx_Struct gHalCanRxStruct;
CAN_BAUD_Enum gHalCanBaudValue = CAN_BAUD_500;
uint32_t gHalCanErrorCount = 0;

//callback func
TfpCanHalCallbackTx SpCAN_CallbackTx[eCanPort_Count] = {NULL};

uint16_t
hal_can_init(uint8_t PortId, CAN_BAUD_Enum Baud)
{
    if (PortId >= eCanPort_Count)
    {
        return false;
    }
    CAN_FIFO_Init(PortId);
    CAN_FIFO_InitCallbacks(PortId);
    CAN_InitWrite(PortId);

    switch(PortId)
    {
        case eCanPort_0:
            {
                //init can device
                //sample points = 75%
            	switch (Baud)
            	{
                    case CAN_BAUD_125:
                    case CAN_BAUD_250:
                    case CAN_BAUD_500:
                    case CAN_BAUD_1000:
                        gHalCanBaudValue = Baud;
                        break;
                    default:
                    	{
                            gHalCanBaudValue = CAN_BAUD_500;
                    	}
                		break;
            	}
                halCanDevicePtr = CAN_Init(NULL, gHalCanBaudValue);
                if(NULL == halCanDevicePtr)
                    return 1;
            }
            break;
        default:return 2;
    }
    gHalCanRxStruct.u8IdxHead = 0;
    gHalCanRxStruct.u8IdxTail = 0;
    gHalCanRxStruct.u32CanRxCount = 0;
    //init can TxMsg
    for( uint8_t i=0; i<CAN_DRIVER_RX_NUM; i++)
    {
        gHalCanRxStruct.lddCanRxTFrame[i].Data = canRxBuf[i];
    }


    return 0;
}

uint16_t
hal_can_deinit(uint8_t PortId)
{
    switch(PortId)
    {
        case eCanPort_0:
            {
                //deinit can device
                if(NULL == halCanDevicePtr)
                    return 1;
                CAN_Deinit(halCanDevicePtr);
            }
            break;
        default:return 2;
    }
    return 0;
}

TeErrorEnum
hal_can_send(TeCanPort port, uint8_t mb, TsCanFrame* frame)
{
    uint16_t u16TimeOut = 0;
	LDD_CAN_TFrame lddCanTFrame = {0};

	if (frame->is_ext_id)
	{
	    lddCanTFrame.MessageID = (frame->id) | CAN_DRIVER_EID_FLAG;
	}
	else
	{
	    lddCanTFrame.MessageID = frame->id;
	}
    lddCanTFrame.FrameType = HAL_CAN_DATA_FRAME;
    if (frame->is_can_fd)
    {
        //TODO:KEA series not support
    }
    else
    {
        lddCanTFrame.Length = frame->dlc <= 8 ? frame->dlc : 8;
    }
    lddCanTFrame.Data = frame->data;
    switch(port)
    {
        case eCanPort_0:
            {
                if(0 == CAN_SendFrame(halCanDevicePtr, mb, &lddCanTFrame))
                {
                    /* Wait for the transmission done */
                    while((u16TimeOut < 10) && (0 != CAN_GetTxFrameState(halCanDevicePtr, mb)))
                    {
                        //最长等待10ms
                        u16TimeOut++;
                    }
                    if(u16TimeOut == 10)
                        return eErrorTimeout;
                    return eErrorOk;
                }
            }
            break;
        default:break;
    }
    return eErrorNotOk;
}

TsCanFrame*
hal_can_rx_queue_de(void)
{
    static TsCanFrame halCanFrameTmp = {};

    EnterCritical();
    //is empty
    if (gHalCanRxStruct.u32CanRxCount == 0)
    {
        ExitCritical();
        return NULL;
    }
    //copy data
    memcpy(&halCanFrameTmp.data, gHalCanRxStruct.lddCanRxTFrame[gHalCanRxStruct.u8IdxHead].Data, CAN_DRIVER_MAX_DLC);
    halCanFrameTmp.dlc = gHalCanRxStruct.lddCanRxTFrame[gHalCanRxStruct.u8IdxHead].Length;
    halCanFrameTmp.is_ext_id =(gHalCanRxStruct.lddCanRxTFrame[gHalCanRxStruct.u8IdxHead].MessageID & CAN_DRIVER_EID_FLAG)!=0 ? 1 : 0;
    if (halCanFrameTmp.is_ext_id)
    {
        halCanFrameTmp.id = gHalCanRxStruct.lddCanRxTFrame[gHalCanRxStruct.u8IdxHead].MessageID & CAN_DRIVER_EXT_M;
    }
    else
    {
        halCanFrameTmp.id = gHalCanRxStruct.lddCanRxTFrame[gHalCanRxStruct.u8IdxHead].MessageID & CAN_DRIVER_STD_M;
    }
    //dequeue
    gHalCanRxStruct.u8IdxHead = (gHalCanRxStruct.u8IdxHead + 1)%CAN_DRIVER_RX_NUM;
    gHalCanRxStruct.u32CanRxCount--;
    ExitCritical();
    return &halCanFrameTmp;
}

void
hal_can_tx_callback_set(uint8_t PortId, TfpCanHalCallbackTx Func)
{
    SpCAN_CallbackTx[PortId] = Func;
}

void
hal_can_tx_callback(uint8_t PortId, uint8_t BufferIdx)
{
    switch(PortId)
    {
        case eCanPort_0:
            {
                (*SpCAN_CallbackTx[PortId])(PortId, BufferIdx);
            }
            break;
        default:break;
    }
}


void
hal_can_rx_callback(uint8_t PortId, uint8_t BufferIdx)
{
    switch(PortId)
    {
        case eCanPort_0:
            {
                if(gHalCanRxStruct.u32CanRxCount > CAN_DRIVER_RX_NUM-1)
                {
                    return;
                }
                (void)CAN_ReadFrame(halCanDevicePtr, BufferIdx,(LDD_CAN_TFrame *)&gHalCanRxStruct.lddCanRxTFrame[gHalCanRxStruct.u8IdxTail]);
                gHalCanRxStruct.u8IdxTail = (gHalCanRxStruct.u8IdxTail + 1)%CAN_DRIVER_RX_NUM;
                gHalCanRxStruct.u32CanRxCount++;
            }
            break;
        default:break;
    }
}

void
hal_can_error_callback(uint8_t PortId, HAL_CAN_ER_TYPE u8CanErrorType)
{
    switch(u8CanErrorType)
    {
        case HAL_CAN_ER_ERROR:
            {
                uint32_t u32ErrorMask = 0;
                CAN_GetError(halCanDevicePtr, &u32ErrorMask);
                gHalCanErrorCount ++;
            }
            break;
        case HAL_CAN_ER_BUSOFF:
            {
                hal_can_deinit(PortId);
                hal_can_init(PortId, gHalCanBaudValue);
            }
            break;
        default:break;
    }
}

