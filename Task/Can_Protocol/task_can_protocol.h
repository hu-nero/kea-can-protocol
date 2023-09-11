/*
 * task_can_protocol.h
 *
 *  Created on: 2023Äê8ÔÂ29ÈÕ
 *      Author: xinlei.hu
 */

#ifndef TASK_CAN_PROTOCOL_TASK_CAN_PROTOCOL_H_
#define TASK_CAN_PROTOCOL_TASK_CAN_PROTOCOL_H_
#include <stdint.h>

#define CAN_PROTOCOL_SYSINFO_VERSION_OFFSET     0
#define CAN_PROTOCOL_SYSINFO_STARTTIME_OFFSET     2
#define CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET     0

typedef enum
{
    CAN_PROTOCOL_ID_REQUEST = 0x1C00B001,
    CAN_PROTOCOL_ID_RESPONSE = 0x1C00B002
} CAN_PROTOCOL_ID_Enum;

typedef enum
{
    CAN_PROTOCOL_CMD_SYSTEM_INFO_REQUEST = 0,
    CAN_PROTOCOL_CMD_ASSIGN_CTRL = 3
} CAN_PROTOCOL_CMD_Enum;

typedef enum
{
    CAN_PROTOCOL_REQ_DATA_ROLL_COUNTER = 0,
    CAN_PROTOCOL_REQ_DATA_CMD = 1,
    CAN_PROTOCOL_REQ_DATA_PARAMETR = 2,
    CAN_PROTOCOL_REQ_DATA_MAX = 7
} CAN_PROTOCOL_REQ_DATA_STRUCT_Enum;

typedef enum
{
    CAN_PROTOCOL_RESP_DATA_ROLL_COUNTER = 0,
    CAN_PROTOCOL_RESP_DATA_CMD = 1,
    CAN_PROTOCOL_RESP_DATA_RESULTS = 2,
    CAN_PROTOCOL_RESP_DATA_PARAMETR = 3,
    CAN_PROTOCOL_RESP_DATA_MAX = 7
} CAN_PROTOCOL_RESP_DATA_STRUCT_Enum;

typedef enum
{
    CAN_PROTOCOL_RESP_RESULTS_OK = 0,
    CAN_PROTOCOL_RESP_RESULTS_ERR = 1
} CAN_PROTOCOL_RESP_RESULTS_Enum;

typedef union
{
    uint32_t u32TimeTick;
    uint8_t  u8Tick[4];
} TimetickUnion;

extern void can_protocol_task_init(void);
extern uint16_t can_protocol_task(void);


#endif /* TASK_CAN_PROTOCOL_TASK_CAN_PROTOCOL_H_ */
