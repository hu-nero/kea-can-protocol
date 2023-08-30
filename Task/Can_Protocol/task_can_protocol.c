/*
 * task_can_protocol.c
 *
 *  Created on: 2023年8月29日
 *      Author: xinlei.hu
 */
#include "task_can_protocol.h"
#include <string.h>
#include "Hal.h"


TsCanFrame gTsCanFrame = {};
static TsCanFrame *gTsCanFramePtr = NULL;

void
can_protocol_task_init(void)
{
    gTsCanFrame.id = 0x18CFFF4;
    gTsCanFrame.dlc = 8;
    gTsCanFrame.is_can_fd = 0;
    gTsCanFrame.is_ext_id = 1;
    memset(gTsCanFrame.data, 0x55, 8);
}

uint16_t
can_protocol_task(void)
{
    //(void)CAN_FIFO_Write(0, &gTsCanFrame);

    //解析CAN报文
    if ((gTsCanFramePtr = hal_can_rx_queue_de()) != NULL)
    {
        gTsCanFrame.id = 0x18CFFF4;
        gTsCanFrame.dlc = 8;
        gTsCanFrame.is_can_fd = 0;
        gTsCanFrame.is_ext_id = 1;
    }
}
