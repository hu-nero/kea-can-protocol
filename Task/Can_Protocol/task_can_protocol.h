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

//assign ctrl
#define DUT_CH_NUM                               16u
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
#define DUT_CH19_PORT                            HAL_GPIOB_PTE
#define DUT_CH19_PIN                             0u
#define DUT_CH20_PORT                            HAL_GPIOB_PTE
#define DUT_CH20_PIN                             1u
#define DUT_CH21_PORT                            HAL_GPIOB_PTG
#define DUT_CH21_PIN                             0u
#define DUT_CH22_PORT                            HAL_GPIOB_PTG
#define DUT_CH22_PIN                             1u
#define DUT_CH23_PORT                            HAL_GPIOB_PTG
#define DUT_CH23_PIN                             2u
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
