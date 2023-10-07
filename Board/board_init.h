/*
 * board_init.h
 *
 *  Created on: 2023年8月29日
 *      Author: xinlei.hu
 */

#ifndef BOARD_BOARD_INIT_H_
#define BOARD_BOARD_INIT_H_

//标定区域Flash地址
#define CALI_BASE_ADDR 0xFC00//63K
#pragma pack(2)
typedef struct _BOOT_CFG_
{
    unsigned int FLAG; //固定值：0xAABBCCDD
    unsigned int ActionTime; //时间
    unsigned char CANBaudMode; //CAN波特率模式(枚举值)
    unsigned char _reserved0[3];
    unsigned char SN[32]; //SN
    unsigned char CRC_A[2];
    unsigned char _reserved1[2];
}_bootcfg_information;
#pragma pack()


extern void board_init(void);


#endif /* BOARD_BOARD_INIT_H_ */
