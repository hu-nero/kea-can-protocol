/*
 * board_init.h
 *
 *  Created on: 2023��8��29��
 *      Author: xinlei.hu
 */

#ifndef BOARD_BOARD_INIT_H_
#define BOARD_BOARD_INIT_H_

//�궨����Flash��ַ
#define CALI_BASE_ADDR 0xFC00//63K
#pragma pack(2)
typedef struct _BOOT_CFG_
{
    unsigned int FLAG; //�̶�ֵ��0xAABBCCDD
    unsigned int ActionTime; //ʱ��
    unsigned char CANBaudMode; //CAN������ģʽ(ö��ֵ)
    unsigned char _reserved0[3];
    unsigned char SN[32]; //SN
    unsigned char CRC_A[2];
    unsigned char _reserved1[2];
}_bootcfg_information;
#pragma pack()


extern void board_init(void);


#endif /* BOARD_BOARD_INIT_H_ */
