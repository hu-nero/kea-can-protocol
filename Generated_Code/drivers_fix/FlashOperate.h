/*=======================================================================
**     Bootloader �������� 
**     �ļ���    : BLChipDriver.h
**     ������    : BootloaderProject
**     ϵͳƽ̨  : FCVM2011 S12XEG128
**     ����ƽ̨  : CodeWarrior IDE version 5.9.0
**     ��������  : 2012/05/19, 20:07
**     ���      : BL��Ҫ��CAN��FLASH������
**     ��Ȩ      : (c) Copyright KeyPower, 2012
**     ������    : ���� 
**     ��ϵ��ʽ  : peng.zhang@key-power.com.cn
**     ��ʷ�汾  :
**                 V2.1 2012.05.19  ʵ�ֻ������� 
=======================================================================*/

#ifndef _FLASHOPERATE_H
#define _FLASHOPERATE_H
#include <stdint.h>


extern void   FlashInit();
extern void FlashDataInit();
extern uint8_t   FlashEraseectorsOperate(uint32_t address);
extern uint8_t   FlashEraseBlockOperate(uint32_t address);
extern uint8_t   FlashWritePageOperate(uint32_t address,uint8_t*data,uint8_t len);
#endif
