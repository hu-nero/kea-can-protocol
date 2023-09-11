/*
 * hal_systick.c
 *
 *  Created on: 2023��2��3��
 *      Author: xinlei.hu
 */
#include "hal_systick.h"
#include "SKEAZ1284.h"
#include "SysTick.h"

#define SYSTICK_GET_COUNTER() ((SYST_CVR) & SysTick_CVR_CURRENT_MASK)

//static uint32_t u32UpSecond = 0;
static uint32_t u32SysClock = 0;
static double gFacUs = 0;//ÿus��Ӧ��tick��
static unsigned long gFacMs = 0;//ÿus��Ӧ��tick��

void
hal_systick_init(void)
{
	//���޸�
	u32SysClock = 40000000;//CORE_CLOCK = 40M
    gFacUs = ((double)u32SysClock/(double)8000000);//ʵ�ʲ���Ϊ8��Ƶ
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
    /* ���浱ǰ������ֵ*/
    LastSysValue = SysTick_DEVICE->CVR;
    /* ���浱ǰsystick����������*/
    LastSysCtrl  = SysTick_DEVICE->CSR;

    SysTick_DEVICE->RVR = (unsigned long)(Nus * gFacUs);//ȡ����ת����uint32_t
    SysTick_DEVICE->CVR = 0x00;
    SysTick_DEVICE->CSR |= SysTick_CSR_ENABLE_MASK;
    do
    {
        temp = SysTick_DEVICE->CSR;
    }
    while(temp & 0x01 && !(temp & (1 << 16)));
    SysTick_DEVICE->CSR = 0x00;
    SysTick_DEVICE->CVR = 0X00;

    /*�ָ�֮ǰ��systick������ */
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
    /* ���浱ǰ������ֵ*/
    LastSysValue = SysTick_DEVICE->CVR;
    /* ���浱ǰsystick����������*/
    LastSysCtrl  = SysTick_DEVICE->CSR;

    SysTick_DEVICE->RVR = (unsigned long)(Nms * gFacMs);//ȡ����ת����uint32_t
    SysTick_DEVICE->CVR = 0x00;
    SysTick_DEVICE->CSR |= SysTick_CSR_ENABLE_MASK;
    do
    {
        temp = SysTick_DEVICE->CSR;
    }
    while(temp & 0x01 && !(temp & (1 << 16)));
    SysTick_DEVICE->CSR = 0x00;
    SysTick_DEVICE->CVR = 0X00;

    /*�ָ�֮ǰ��systick������ */
    SysTick_DEVICE->RVR = LastSysValue;
    SysTick_DEVICE->CVR = 0x00;
    SysTick_DEVICE->CSR = LastSysCtrl;
}
