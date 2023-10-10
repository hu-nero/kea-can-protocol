/*=======================================================================
**     Bootloader 驱动部分 
**     文件名    : BLChipDriver.h
**     工程名    : BootloaderProject
**     系统平台  : FCVM2011 S12XEG128
**     编译平台  : CodeWarrior IDE version 5.9.0
**     创建日期  : 2012/05/19, 20:07
**     简介      : BL需要的CAN和FLASH的驱动
**     版权      : (c) Copyright KeyPower, 2012
**     开发者    : 张鹏 
**     联系方式  : peng.zhang@key-power.com.cn
**     历史版本  :
**                 V2.1 2012.05.19  实现基本功能 
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
