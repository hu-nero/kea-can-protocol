/*=======================================================================
**     Bootloader 驱动部分 
**     文件名    : BLChipDriver.c
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

#include "FlashOperate.h"
#include <string.h>
#include "SKEAZ1284.h"
#include "PE_Types.h"
#include "PE_Error.h"
#define FLASH1_BLOCK0_ADDRESS   0xFC00u
#define FLASH1_BLOCK0_SIZE     	0x00010000LU
#define SAFE_ROUTINE_SIZE 		28U
/***********************************************************************************************************/
typedef void (* LDD_FLASH_TSafeRoutinePtr)(void); /* Safe routine pointer type */
LDD_FLASH_TSafeRoutinePtr  SafeRoutinePtr;     /* Safe routine pointer */

static const uint8_t SafeRoutine[SAFE_ROUTINE_SIZE - 4U] = {
  0x00U,0xb5U,               /*  push	{lr}                                         */
  0x04U,0x4aU,               /*  ldr	r2, [pc, #16]	; (bc <SafeRoutine___+0x14>) */
  0x80U,0x23U,               /*  movs	r3, #128	; 0x80                       */
  0x13U,0x70U,               /*  strb	r3, [r2, #0]                                 */
  0x11U,0x78U,               /*  ldrb	r1, [r2, #0]                                 */
  0x48U,0xb2U,               /*  sxtb	r0, r1                                       */
  0x00U,0x28U,               /*  cmp	r0, #0                                       */
  0xfbU,0xdaU,               /*  bge.n	b0 <SafeRoutine___+0x8>                      */
  0x00U,0xbdU,               /*  pop	{pc}                                         */
  0xc0U,0x46U,               /*  nop			; (mov r8, r8)               */
  (uint8_t)((0x40020005U>>0)&0xFFU),  /* FTMRE_FSTAT register address                 */
  (uint8_t)((0x40020005U>>8)&0xFFU),
  (uint8_t)((0x40020005U>>16)&0xFFU),
  (uint8_t)((0x40020005U>>24)&0xFFU)
};
static uint32_t SafeRoutineBuffer[8];
static uint8_t CheckStateAndRange(uint32_t addr,uint32_t len)
{
	if(addr < FLASH1_BLOCK0_ADDRESS)
	{
		return ERR_PARAM_ADDRESS;
	}
	addr += len;
	if (addr > (FLASH1_BLOCK0_ADDRESS + FLASH1_BLOCK0_SIZE)) {
		return ERR_PARAM_SIZE;
	}

	return ERR_OK;
}
void FlashInit()
{
	SIM_SCGC |= SIM_SCGC_FLASH_MASK;//enable flash module clock
	/* FTMRE_FCLKDIV: FDIVLD=0,FDIVLCK=0,FDIV=0x13 */
	FTMRE_FCLKDIV = FTMRE_FCLKDIV_FDIV(0x13); /* Set clock divider */
	FTMRE_FCNFG = 0;//disable interrupt
}
void FlashDataInit()
{
	memcpy(SafeRoutineBuffer,SafeRoutine,sizeof(SafeRoutine));
	SafeRoutinePtr = (void*)SafeRoutineBuffer;
}
uint8_t   FlashEraseectorsOperate(uint32_t address) {
	if(ERR_OK!=CheckStateAndRange(address,0))
		return CheckStateAndRange(address,0);


    if(!(FTMRE_FCLKDIV& FTMRE_FCLKDIV_FDIVLD_MASK)) {
    	FTMRE_FCLKDIV = FTMRE_FCLKDIV_FDIV(0x13); /* Set clock divider */
    }
    while(!(FTMRE_FSTAT & FTMRE_FSTAT_CCIF_MASK))
	{
		//wait last operate finished
	}
    if(FTMRE_FSTAT &(FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK)) {
    	FTMRE_FSTAT |= (FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK);
    }
  //  address=address&(~(0x3ff));
    //set parameter
    FTMRE_FCCOBIX=FTMRE_FCCOBIX_CCOBIX(0x0);
    FTMRE_FCCOBLO=(uint8_t)((address>>16)&0xff);
    FTMRE_FCCOBHI=0x0a;
    
    FTMRE_FCCOBIX=FTMRE_FCCOBIX_CCOBIX(0x01);
    FTMRE_FCCOBLO=(uint8_t)((address>>0)&0xff);
    FTMRE_FCCOBHI=(uint8_t)((address>>8)&0xff);
    
    __DI();
    ((LDD_FLASH_TSafeRoutinePtr)((uint32_t)(SafeRoutinePtr) | 1U))();
    __EI();
    return FTMRE_FSTAT;
}
uint8_t   FlashEraseBlockOperate(uint32_t address) {

	if(ERR_OK!=CheckStateAndRange(address,0))
		return CheckStateAndRange(address,0);


	if(!(FTMRE_FCLKDIV& FTMRE_FCLKDIV_FDIVLD_MASK)) {
		FTMRE_FCLKDIV = FTMRE_FCLKDIV_FDIV(0x13); /* Set clock divider */
	}
	while(!(FTMRE_FSTAT & FTMRE_FSTAT_CCIF_MASK))
	{
		//wait last operate finished
	}
	if(FTMRE_FSTAT &(FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK)) {
		FTMRE_FSTAT |= (FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK);
	}
    //address=address&(~(0x3ff));
    //set parameter
	FTMRE_FCCOBIX=0x0;
	FTMRE_FCCOBLO=(uint8_t)((address>>16)&0xff);
	FTMRE_FCCOBHI=0x09;
    
	FTMRE_FCCOBIX=0x1;
	FTMRE_FCCOBLO=(uint8_t)((address>>0)&0xff);
	FTMRE_FCCOBHI=(uint8_t)((address>>8)&0xff);
    
    __DI();
    ((LDD_FLASH_TSafeRoutinePtr)((uint32_t)(SafeRoutinePtr) | 1U))();
    __EI();
    return FTMRE_FSTAT;
}
uint8_t  FlashWritePageOperate(uint32_t address,uint8_t*data,uint8_t len) {
    uint32_t index;
	if(ERR_OK!=CheckStateAndRange(address,len))
		return CheckStateAndRange(address,len);


	if(!(FTMRE_FCLKDIV& FTMRE_FCLKDIV_FDIVLD_MASK)) {
		FTMRE_FCLKDIV = FTMRE_FCLKDIV_FDIV(0x13); /* Set clock divider */
	}
	while(!(FTMRE_FSTAT & FTMRE_FSTAT_CCIF_MASK))
	{
		//wait last operate finished
	}
	if(FTMRE_FSTAT &(FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK)) {
		FTMRE_FSTAT |= (FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK);
	}
    //set parameter
	FTMRE_FCCOBIX=0x0u;
	FTMRE_FCCOBLO=(uint8_t)((address>>16)&0xff);
	FTMRE_FCCOBHI=0x06;
    
	FTMRE_FCCOBIX=0x1u;
	FTMRE_FCCOBLO=(uint8_t)((address>>0)&0xff);
	FTMRE_FCCOBHI=(uint8_t)((address>>8)&0xff);
    index=0;
    if(len>8)return 0x80;
    for(;;) {
    	FTMRE_FCCOBIX=0x2+index/2;
    	FTMRE_FCCOBLO=*(data+index);
    	FTMRE_FCCOBHI=*(data+index+1);
        index+=2;
        if(index>=8)break;
    }
    __DI();
	((LDD_FLASH_TSafeRoutinePtr)((uint32_t)(SafeRoutinePtr) | 1U))();
	__EI();
    return FTMRE_FSTAT;
}


