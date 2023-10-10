/*
 * board_init.h
 *
 *  Created on: 2023年8月29日
 *      Author: xinlei.hu
 */

#ifndef BOARD_BOARD_INIT_H_
#define BOARD_BOARD_INIT_H_

//标定区域Flash地址
#define FLASHOPERATEUINT 8U
#define CALI_BASE_ADDR 0x0000FE00 //size=0x14
#define CALI_CFG_FLAG  0xAAAABBBB

typedef struct _CALI_CFG_
{
    unsigned int u32Flag; //固定值：0xAAAABBBB
    unsigned int u32CanBaudValue; //波特率
    unsigned int u32CanRequestId; //请求ID
    unsigned int u32CanResponseId; //回复ID
    unsigned int reserve; //保留
}_calibratecfg_info; //20byte

typedef enum
{
    CAN_DEFAULT_ID_REQUEST = 0x1C00F001UL,
    CAN_DEFAULT_ID_RESPONSE = 0x1C00F002UL
} CAN_DEFAULT_ID_Enum;

extern _calibratecfg_info gBootInfo;
extern void board_init(void);


#endif /* BOARD_BOARD_INIT_H_ */
