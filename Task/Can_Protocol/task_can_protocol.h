/*
 * task_can_protocol.h
 *
 *  Created on: 2023Äê8ÔÂ29ÈÕ
 *      Author: xinlei.hu
 */

#ifndef TASK_CAN_PROTOCOL_TASK_CAN_PROTOCOL_H_
#define TASK_CAN_PROTOCOL_TASK_CAN_PROTOCOL_H_
#include <stdint.h>

#define CAN_PROTOCOL_SYSINFO_VERSION_OFFSET        0u
#define CAN_PROTOCOL_SYSINFO_STARTTIME_OFFSET      2u

#define CAN_PROTOCOL_SWITCH_OUTPUTCTRL_OFFSET      0u
#define CAN_PROTOCOL_SWITCH_OUTPUTCTRL_MASK_OFFSET 1u

#define CAN_PROTOCOL_NTC_R_OFFSET                  0u
#define CAN_PROTOCOL_NTC_CH_STATE_OFFSET           2u

#define CAN_PROTOCOL_CC_R_OFFSET                   0u
#define CAN_PROTOCOL_CC2_1K_R_PULLDOWN_OFFSET      2u
#define CAN_PROTOCOL_CC_MASK                       3u

#define CAN_PROTOCOL_DI_VALUE_OFFSET               0u
#define CAN_PROTOCOL_LSD_STATE_OFFSET              1u
#define CAN_PROTOCOL_HSD_STATE_OFFSET              2u

#define CAN_PROTOCOL_HLSD_R_VALUE              30000u
#define CAN_PROTOCOL_LSD_0_CURRENT_OFFSET          0u
#define CAN_PROTOCOL_LSD_1_CURRENT_OFFSET          1u
#define CAN_PROTOCOL_LSD_2_CURRENT_OFFSET          2u
#define CAN_PROTOCOL_LSD_3_CURRENT_OFFSET          3u

#define CAN_PROTOCOL_HSD_0_CURRENT_OFFSET          0u
#define CAN_PROTOCOL_HSD_1_CURRENT_OFFSET          1u
#define CAN_PROTOCOL_HSD_2_CURRENT_OFFSET          2u
#define CAN_PROTOCOL_HSD_3_CURRENT_OFFSET          3u

#define CAN_PROTOCOL_CP_DUTY_OFFSET                0u
#define CAN_PROTOCOL_CP_FREQ_OFFSET                1u
#define CAN_PROTOCOL_CP_MASK_OFFSET                3u

typedef enum
{
    CAN_PROTOCOL_ID_REQUEST = 0x1C00B001,
    CAN_PROTOCOL_ID_RESPONSE = 0x1C00B002
} CAN_PROTOCOL_ID_Enum;

enum
{
    DI_0_BIT = 0,
    DI_1_BIT
};

enum
{
    LSD_0_BIT = 0,
    LSD_1_BIT,
    LSD_2_BIT,
    LSD_3_BIT,
    LSD_MAX_BIT
};

enum
{
    HSD_0_BIT = 0,
    HSD_1_BIT,
    HSD_2_BIT,
    HSD_3_BIT,
    HSD_MAX_BIT
};

typedef enum
{
    CAN_PROTOCOL_CMD_SYSTEM_INFO_REQUEST = 0,
    CAN_PROTOCOL_CMD_RESET = 1,
    CAN_PROTOCOL_CMD_SWITCH_OUTPUT_CTRL = 3,
    CAN_PROTOCOL_CMD_NTC_R_SIMULATION = 5,
    CAN_PROTOCOL_CMD_CC_R_SIMULATION = 7,
    CAN_PROTOCOL_CMD_CP_SIMULATION = 9,
    CAN_PROTOCOL_CMD_CP_VOL_QUERY = 11,
    CAN_PROTOCOL_CMD_INPUT_PORT_QUERY = 13,
    CAN_PROTOCOL_CMD_LSD_CURRENT_QUERY= 15,
    CAN_PROTOCOL_CMD_HSD_CURRENT_QUERY= 17
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
    uint16_t u16Data;
    uint8_t  u8Data[2];
} SingleWordUnion;

typedef union
{
    uint32_t u32Data;
    uint8_t  u8Data[4];
} DoubleWordUnion;

extern void can_protocol_task_init(void);
extern uint16_t can_protocol_task(void);


#endif /* TASK_CAN_PROTOCOL_TASK_CAN_PROTOCOL_H_ */
