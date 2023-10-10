/*
 * task_can_protocol.h
 *
 *  Created on: 2023Äê8ÔÂ29ÈÕ
 *      Author: xinlei.hu
 */

#ifndef TASK_CAN_PROTOCOL_TASK_CAN_PROTOCOL_H_
#define TASK_CAN_PROTOCOL_TASK_CAN_PROTOCOL_H_
#include <stdint.h>

#define CAN_CALIBRATE_BAUD_SET_OFFSET                                    0u
#define CAN_CALIBRATE_BUSSINESS_COMMUNICATE_RECV_CMD_OFFSET              0u
#define CAN_CALIBRATE_BUSSINESS_COMMUNICATE_RECV_ID_OFFSET               1u
#define CAN_CALIBRATE_BUSSINESS_COMMUNICATE_SEND_CMD_OFFSET              0u
#define CAN_CALIBRATE_BUSSINESS_COMMUNICATE_SEND_ID_OFFSET               1u

#define CAN_PROTOCOL_SYSINFO_VERSION_OFFSET        0
#define CAN_PROTOCOL_SYSINFO_STARTTIME_OFFSET      2

#define CAN_PROTOCOL_ASSIGNCTRL_CHSIZE            24
#define CAN_PROTOCOL_ASSIGNCTRL_CHCTRUL_OFFSET     0
#define CAN_PROTOCOL_ASSIGNCTRL_CHMASK_OFFSET      3

//#define CAN_PROTOCOL_ASSIGNCTRL_CH_OFFSET         0u

//assign ctrl
#define DUT_CH0_PORT                             HAL_GPIOB_PTH
#define DUT_CH0_PIN                              1u
#define DUT_CH1_PORT                             HAL_GPIOB_PTH
#define DUT_CH1_PIN                              0u
#define DUT_CH2_PORT                             HAL_GPIOB_PTE
#define DUT_CH2_PIN                              6u
#define DUT_CH3_PORT                             HAL_GPIOB_PTE
#define DUT_CH3_PIN                              5u
#define DUT_CH4_PORT                             HAL_GPIOA_PTB
#define DUT_CH4_PIN                              5u
#define DUT_CH5_PORT                             HAL_GPIOA_PTB
#define DUT_CH5_PIN                              4u

#define DUT_CH6_PORT                             HAL_GPIOA_PTC
#define DUT_CH6_PIN                              3u
#define DUT_CH7_PORT                             HAL_GPIOA_PTC
#define DUT_CH7_PIN                              2u
#define DUT_CH8_PORT                             HAL_GPIOA_PTD
#define DUT_CH8_PIN                              7u
#define DUT_CH9_PORT                             HAL_GPIOA_PTD
#define DUT_CH9_PIN                              6u
#define DUT_CH10_PORT                            HAL_GPIOA_PTD
#define DUT_CH10_PIN                             5u
#define DUT_CH11_PORT                            HAL_GPIOA_PTC
#define DUT_CH11_PIN                             1u

#define DUT_CH12_PORT                            HAL_GPIOA_PTC
#define DUT_CH12_PIN                             0u
#define DUT_CH13_PORT                            HAL_GPIOB_PTF
#define DUT_CH13_PIN                             7u
#define DUT_CH14_PORT                            HAL_GPIOB_PTF
#define DUT_CH14_PIN                             6u
#define DUT_CH15_PORT                            HAL_GPIOB_PTF
#define DUT_CH15_PIN                             5u
#define DUT_CH16_PORT                            HAL_GPIOB_PTF
#define DUT_CH16_PIN                             4u
#define DUT_CH17_PORT                            HAL_GPIOA_PTB
#define DUT_CH17_PIN                             3u

#define DUT_CH18_PORT                            HAL_GPIOA_PTB
#define DUT_CH18_PIN                             2u
#define DUT_CH19_PORT                            HAL_GPIOB_PTF
#define DUT_CH19_PIN                             1u
#define DUT_CH20_PORT                            HAL_GPIOB_PTF
#define DUT_CH20_PIN                             0u
#define DUT_CH21_PORT                            HAL_GPIOA_PTD
#define DUT_CH21_PIN                             4u
#define DUT_CH22_PORT                            HAL_GPIOA_PTD
#define DUT_CH22_PIN                             3u
#define DUT_CH23_PORT                            HAL_GPIOA_PTD
#define DUT_CH23_PIN                             2u


typedef enum
{
    CAN_CALIBRATE_ID_REQUEST = 0x1234ABCD,
    CAN_CALIBRATE_ID_RESPONSE = 0x1234EEEE
} CAN_CALIBRATE_ID_Enum;

enum
{
    BUSSINESS_COMMUNICATE_RECV_ID_QUERY = 0,
    BUSSINESS_COMMUNICATE_RECV_ID_SET = 0xAC
};

enum
{
    BUSSINESS_COMMUNICATE_SEND_ID_QUERY = 0,
    BUSSINESS_COMMUNICATE_SEND_ID_SET = 0xAC
};

typedef enum
{
    CAN_PROTOCOL_CMD_SYSTEM_INFO_REQUEST = 0,
    CAN_PROTOCOL_CMD_ASSIGN_CTRL = 24
} CAN_PROTOCOL_CMD_Enum;

typedef enum
{
    CAN_CALIBRATE_CMD_ENTER = 0,
    CAN_CALIBRATE_CMD_EXIT = 99,
    CAN_CALIBRATE_CMD_CAN_BAUD_SET = 3,
    CAN_CALIBRATE_CMD_BUSINESS_COMMUNICATE_RECV_ID = 5,
    CAN_CALIBRATE_CMD_BUSINESS_COMMUNICATE_SEND_ID = 7
} CAN_CALIBRATE_CMD_enum;

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
