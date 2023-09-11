/*
 * hal_systick.c
 *
 *  Created on: 2023年2月3日
 *      Author: xinlei.hu
 */
#include "hal_systick.h"
#include "SKEAZ1284.h"
#include "SysTick.h"

#define SYSTICK_GET_COUNTER() ((SYST_CVR) & SysTick_CVR_CURRENT_MASK)

//static uint32_t u32UpSecond = 0;
static uint32_t u32SysClock = 0;
static double gFacUs = 0;//每us对应的tick数
static unsigned long gFacMs = 0;//每us对应的tick数

void
hal_systick_init(void)
{
	//待修改
	u32SysClock = 40000000;//CORE_CLOCK = 40M
    gFacUs = ((double)u32SysClock/(double)8000000);//实际测试为8分频
    gFacMs = (unsigned long)(gFacUs * 1000);
}

uint32_t
hal_systick_get_counter(void)
{
    return SYSTICK_GET_COUNTER();
}

PE_ISR(SysTick_Handler)
{
}

void
hal_systick_delay_us(unsigned long Nus)
{
    unsigned long temp;
    unsigned long LastSysValue;
    unsigned long LastSysCtrl;

    if (Nus == 0) return;
    /* 保存当前倒计数值*/
    LastSysValue = SysTick_DEVICE->CVR;
    /* 保存当前systick的配置内容*/
    LastSysCtrl  = SysTick_DEVICE->CSR;

    SysTick_DEVICE->RVR = (unsigned long)(Nus * gFacUs);//取整并转化成uint32_t
    SysTick_DEVICE->CVR = 0x00;
    SysTick_DEVICE->CSR |= SysTick_CSR_ENABLE_MASK;
    do
    {
        temp = SysTick_DEVICE->CSR;
    }
    while(temp & 0x01 && !(temp & (1 << 16)));
    SysTick_DEVICE->CSR = 0x00;
    SysTick_DEVICE->CVR = 0X00;

    /*恢复之前的systick的配置 */
    SysTick_DEVICE->RVR = LastSysValue;
    SysTick_DEVICE->CVR = 0x00;
    SysTick_DEVICE->CSR = LastSysCtrl;
}

void
hal_systick_delay_ms(unsigned long Nms)
{
    unsigned long temp;
    unsigned long LastSysValue;
    unsigned long LastSysCtrl;

    if (Nms == 0) return;
    /* 保存当前倒计数值*/
    LastSysValue = SysTick_DEVICE->CVR;
    /* 保存当前systick的配置内容*/
    LastSysCtrl  = SysTick_DEVICE->CSR;

    SysTick_DEVICE->RVR = (unsigned long)(Nms * gFacMs);//取整并转化成uint32_t
    SysTick_DEVICE->CVR = 0x00;
    SysTick_DEVICE->CSR |= SysTick_CSR_ENABLE_MASK;
    do
    {
        temp = SysTick_DEVICE->CSR;
    }
    while(temp & 0x01 && !(temp & (1 << 16)));
    SysTick_DEVICE->CSR = 0x00;
    SysTick_DEVICE->CVR = 0X00;

    /*恢复之前的systick的配置 */
    SysTick_DEVICE->RVR = LastSysValue;
    SysTick_DEVICE->CVR = 0x00;
    SysTick_DEVICE->CSR = LastSysCtrl;
}
