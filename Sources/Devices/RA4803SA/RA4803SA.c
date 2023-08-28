/*
 * RA4803SA.c
 *
 *  Created on: 2021��9��29��
 *      Author: jiangliang.liu
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "RA4803SA.h"
#include <string.h>
#include <stdlib.h>
//#include "FuncCom.h"
//#include "RTC_CE.h"
//#include "RX8130.h"
#include "Hal.h"




uint8_t SetRTCtimerFlagCache_RA4803 = 0;
uint8_t SetWakeUpFlagCache_RA4803 = 0;
uint16_t PowerOffInterruptTimer = 0xFFF0;

uint8_t SPI_Test0[8] = {0x90,0,0,0,0,0,0,0};
uint8_t SPI_Test1[8] = {0};

static uint8_t gsu8RA4803SendBuf[20] = {0};
static uint8_t gsu8RA4803RecvBuf[20] = {0};


uint8_t 
RA4803_SPI_Transmit(uint8_t *sdata, uint8_t *rdata, uint8_t len)
{
    return (uint8_t)hal_spi_master_transfer_data_blocking(HAL_DEV_SPI1, sdata, rdata, len, 10);
}


static uint8_t
BCC_PackFrame(uint8_t Reg, RA4803_Action Action, const uint8_t *Data, uint8_t DataLen, uint8_t *Frame, uint8_t FrameLen)
{
    if(DataLen > 19)
        return 1;
    if(FrameLen <= DataLen)
        return 2;
    Frame[0] = Action|(Reg&0x0F);
    if(DataLen != 0)
    {
        memcpy((uint8_t *)(&Frame[1]), Data, DataLen);
    }
}

uint8_t 
RA4803_ReadData(uint8_t Reg, uint8_t *Rdata, uint8_t Len) {
	uint8_t err = 0;
	if(Len >= 20) {
		return 1;
	}
    BCC_PackFrame(Reg, Action_Read_Data, NULL, 0, gsu8RA4803SendBuf, 20);
	err = RA4803_SPI_Transmit(gsu8RA4803SendBuf, gsu8RA4803RecvBuf, Len+1);
	memcpy(Rdata, (uint8_t *)(&gsu8RA4803RecvBuf[1]), Len);
	return err;
}

uint8_t 
RA4803_WriteData(uint8_t Reg, const uint8_t *Sdata, uint8_t Len) {
	uint8_t err = 0;
	if(Len >= 20) {
		return 1;
	}
    BCC_PackFrame(Reg, Action_Write_RegData, Sdata, Len, gsu8RA4803SendBuf, 20);
	err = RA4803_SPI_Transmit(gsu8RA4803SendBuf, gsu8RA4803RecvBuf, Len+1);
	return err;
}

uint8_t 
RA4803_WriteReg(uint8_t Reg, const uint8_t Sdata)
{
	uint8_t err = 0;
    BCC_PackFrame(Reg, Action_Write_RegData, &Sdata, 1, gsu8RA4803SendBuf, 20);
	err = RA4803_SPI_Transmit(gsu8RA4803SendBuf, gsu8RA4803RecvBuf, 2);
	return err;
}

uint8_t 
RA4803_ReadData_2(uint8_t Reg, uint8_t *Rdata, uint8_t Len)
{
	uint8_t err = 0;
	if(Len >= 20) {
		return 1;
	}
    BCC_PackFrame(Reg, Action_Read_Data2, NULL, 0, gsu8RA4803SendBuf, 20);
	err = RA4803_SPI_Transmit(gsu8RA4803SendBuf, gsu8RA4803RecvBuf, Len+1);
	memcpy(Rdata, (uint8_t *)(&gsu8RA4803RecvBuf[1]), Len);
	return err;
}

uint8_t 
RA4803_WriteData_2(uint8_t Reg, const uint8_t *Sdata, uint8_t Len) 
{
	uint8_t err = 0;
	if(Len >= 20) {
		return 1;
	}
    BCC_PackFrame(Reg, Action_Write_RegData2, Sdata, Len, gsu8RA4803SendBuf, 20);
	err = RA4803_SPI_Transmit(gsu8RA4803SendBuf, gsu8RA4803RecvBuf, Len+1);
	return err;
}

uint8_t 
RA4803_WriteReg_2(uint8_t Reg, const uint8_t Sdata)
{
	uint8_t err = 0;
    BCC_PackFrame(Reg, Action_Write_RegData2, &Sdata, 1, gsu8RA4803SendBuf, 20);
	err = RA4803_SPI_Transmit(gsu8RA4803SendBuf, gsu8RA4803RecvBuf, 2);
	return err;
}

uint8_t 
RA4803_ReadData_3(uint8_t Reg, uint8_t *Rdata, uint8_t Len)
{
	uint8_t err = 0;
	if(Len >= 20) {
		return 1;
	}
    BCC_PackFrame(Reg, Action_Write_RegData3, NULL, 0, gsu8RA4803SendBuf, 20);
	err = RA4803_SPI_Transmit(gsu8RA4803SendBuf, gsu8RA4803RecvBuf, Len+1);
	memcpy(Rdata, (uint8_t *)(&gsu8RA4803RecvBuf[1]), Len);
	return err;
}

uint8_t 
RA4803_WriteData_3(uint8_t Reg, const uint8_t *Sdata, uint8_t Len) 
{
	uint8_t err = 0;
	if(Len >= 20) {
		return 1;
	}
    BCC_PackFrame(Reg, Action_Write_RegData3, Sdata, Len, gsu8RA4803SendBuf, 20);
	err = RA4803_SPI_Transmit(gsu8RA4803SendBuf, gsu8RA4803RecvBuf, Len+1);
	return err;
}

uint8_t
RA4803_WriteReg_3(uint8_t Reg, const uint8_t Sdata)
{
	uint8_t err = 0;
    BCC_PackFrame(Reg, Action_Write_RegData3, &Sdata, 1, gsu8RA4803SendBuf, 20);
	err = RA4803_SPI_Transmit(gsu8RA4803SendBuf, gsu8RA4803RecvBuf, 2);
	return err;
}

uint8_t SetFixCycleTimerInterrupt_RA4803(uint16_t timer){
	uint8_t err = 0;
	uint8_t Regbuf[3] = {0};
	err |= RA4803_ReadData(Extension_Register_RA4803, Regbuf, 3);//0Dh 0Eh 0Fh

	Regbuf[0] = ((Regbuf[0]&(~_TE_RA4803))&0xFC) | 0x03;//��������  TSEL1 0->11 1min
	//Regbuf[0] = ((Regbuf[0]&(~_TE_RA4803))&0xFC) | 0x02;//���ܲ�ʹ�ã�1s  TSEL1 0->10 1s
	err |= RA4803_WriteReg(Extension_Register_RA4803, Regbuf[0]);//0Dh:TE->0; TSEL1 0->10 1s	TSEL1 0->11 1min

	Regbuf[2] = Regbuf[2]&(~_TIE_RA4803);
	err |= RA4803_WriteReg(Control_Register_RA4803, Regbuf[2]);//0Fh:TIE->0

	Regbuf[1] = Regbuf[1]&(~_FlagReg_TF_RA4803);//_FlagReg_TF_RA4803
	err |= RA4803_WriteReg(Flag_Register_RA4803, Regbuf[1]);//0Eh:TF->0;

	err |= RA4803_WriteReg(TimerCounter0_RA4803, (uint8_t)(timer&0xFF)); //�������ڣ���λmin/s
	err |= RA4803_WriteReg(TimerCounter1_RA4803, (uint8_t)((timer>>8)&0x0F));

	Regbuf[2] = Regbuf[2]|_TIE_RA4803;
	err |= RA4803_WriteReg(Control_Register_RA4803, Regbuf[2]);//0Fh:TIE->1		TIE���ж�ֹͣ����

	Regbuf[0] = Regbuf[0]|_TE_RA4803;
	err |= RA4803_WriteReg(Extension_Register_RA4803, Regbuf[0]);//1Ch:TE->1;	TE����������




	err |= RA4803_ReadData(Extension_Register_RA4803, Regbuf, 3);//0Dh 0Eh 0Fh


	return err;
}

uint8_t ClrFixCycleTimerInterrupt_RA4803(void) {
	uint8_t Regbuf[3] = {0};
	RA4803_ReadData(Extension_Register_RA4803, Regbuf, 1);//0Dh 0Eh 0Fh
	Regbuf[0] = ((Regbuf[0]&(~_TE_RA4803))&0xFF);//TE->0
	RA4803_WriteReg(Extension_Register_RA4803, Regbuf[0]);//1Ch:TE->0;
	RA4803_WriteReg(Control_Register_RA4803, 0x80);      //0Fh:CSEL1->1;CSEL0->0;UIE->0;TIE->0;AIE->0;RESET->0
	//RA4803_WriteReg(Control_Register_RA4803, 0x40);      //0Fh:CSEL1->1;CSEL0->0;UIE->0;TIE->0;AIE->0;RESET->0
	PowerOffInterruptTimer = 0;//��Чֵ
	return 0;

}

uint16_t getTimerCounterallCache_RA4803 = 0;

uint8_t FixCycleTimerCheck_RA4803(uint16_t timer) {
  uint8_t err = 0;
  static uint8_t getTimerCounter[2] = {0};
  uint16_t getTimerCounterall = 0;
  err = RA4803_ReadData(TimerCounter0_RA4803, getTimerCounter, 2);
  getTimerCounterall = (uint16_t)(((uint16_t)getTimerCounter[0]) | ((uint16_t)((uint16_t)getTimerCounter[1]<<8)));
  if (getTimerCounterall != timer) {
      err |= 0x80;
  }
  getTimerCounterallCache_RA4803 = getTimerCounterall;
  return err;
}

/*************************************************************************************
**�������ƣ� SetPowerOffTimer
**����		timer - Power Off time (min)
**���ܣ�   set Power Off Timer���������߻��Ѻ�ʱ��
**����ֵ�� 0-OK	 other-ERR
**************************************************************************************/

uint8_t SetPowerOffTimer_RA4803(uint16_t timer) {
  uint8_t err = 0;
  int8_t cycle = 5;
  uint16_t PowerOffInterruptTimerBuf = 0;

  PowerOffInterruptTimerBuf = PowerOffInterruptTimer;
  if (timer < 1) {
  	PowerOffInterruptTimer = 1;
  } else {
  	PowerOffInterruptTimer = timer;
  }
  //PowerOffInterruptTimer = 2;
  while (cycle --) {
    err = SetFixCycleTimerInterrupt_RA4803(PowerOffInterruptTimer);
    err |= FixCycleTimerCheck_RA4803(PowerOffInterruptTimer);
    if (err == 0) {
        break;
    }
  }
  if (cycle < 0) {
  	err |= 0x80;
  }
  if (err != 0) {
	  PowerOffInterruptTimer = PowerOffInterruptTimerBuf;
  }
  return err;
}

uint8_t GetCycleTimer_RA4803(uint16_t *timer) {
  uint8_t err = 0;
  static uint8_t getTimer[2] = {0};
  uint16_t getTimerCounterall = 0;
  err = RA4803_ReadData(TimerCounter0_RA4803, getTimer, 2);
  getTimerCounterall = (uint16_t)(((uint16_t)getTimer[0]) | ((uint16_t)((uint16_t)getTimer[1]<<8)));
  timer[0] = getTimerCounterall;

  return err;
}

uint8_t ClearFixCycleTimerFlag_RA4803(void) {
  uint8_t err = 0;
  uint8_t Regbuf = 0;
  err |= RA4803_ReadData(Flag_Register_RA4803, &Regbuf, 1);//1Eh
  Regbuf = Regbuf&(~_FlagReg_TF_RA4803);
  err |= RA4803_WriteReg(Flag_Register_RA4803, Regbuf);//1Dh:TF->0;
  return err;
}

uint8_t GetFixCycleTimerFlag_RA4803(uint8_t *flag) {
  uint8_t err = 0;
  uint8_t Regbuf = 0;
  err |= RA4803_ReadData(Flag_Register_RA4803, &Regbuf, 1);//1Eh
  *flag = Regbuf&(_FlagReg_TF_RA4803);
  return err;
}

uint8_t SetAlarmInterrupt_RA4803(uint16_t timer) {
  uint8_t err = 0;

  return err;
}

uint8_t SetTimerUpdate_RA4803(void) {
  uint8_t err = 0;

  return err;
}

uint8_t PutBitRESET_RA4803(uint8_t bit) {//0x40
  uint8_t err = 0;
  uint8_t Regbuf = 0;
  err |= RA4803_ReadData(Control_Register_RA4803, &Regbuf, 1);//0Fh
  if (bit&_RESET_RA4803) {
    Regbuf = Regbuf | _RESET_RA4803;
  } else {
    Regbuf = Regbuf & (~_RESET_RA4803);
  }
  err |= RA4803_WriteReg(Control_Register_RA4803, Regbuf);//0Fh:RESET
  return err;
}

uint8_t GetBitRESET_RA4803(uint8_t *bit) {
  uint8_t err = 0;
  //uint8_t Regbuf = 0;
  err |= RA4803_ReadData(Control_Register_RA4803, bit, 1);//1Eh
  return err;
}

uint8_t GetBitVLF_RA4803(uint8_t *bit) {
  uint8_t err = 0;
  err |= RA4803_ReadData(Flag_Register_RA4803, bit, 1);//1Dh
  return err;
}

uint8_t ClearBitVLF_RA4803(void) {
  uint8_t err = 0;
  uint8_t Regbuf = 0;
  err |= RA4803_ReadData(Flag_Register_RA4803, &Regbuf, 1);//1Dh
  Regbuf = Regbuf & (~_FlagReg_VLF_RA4803);
  err |= RA4803_WriteReg(Flag_Register_RA4803, Regbuf);//1Dh:VLF->0;
  return err;
}

/**********************************************************
**�������ƣ� Init_RX8130
**���ܣ�   ��ʼRX8130
**����ֵ�� ͨ�Ź���
***********************************************************/
uint8_t testRA4803_Data[16] = {0};

static uint8_t BackupFunc_Data = 0;
static uint8_t Control_Data = 0;
static uint8_t OscOffset_Data = 0;
static uint8_t EventControl_Data = 0;

uint8_t 
Init_RA4803(void)
{
  uint8_t err = 0;

  err |= RA4803_WriteReg(Extension_Register_RA4803, 0x0A);     //0Dh:TEST->0;WADA->0;USEL->0;TE->0;FSEL1 0->1 0;TSEL1 0->1 0	00001010
  err |= RA4803_WriteReg(Flag_Register_RA4803, 0x00);          //0Eh:0; 0; UF->0;TF->0;AF->0; 0; VLF->0; VDET->0;
  err |= RA4803_WriteReg(Control_Register_RA4803, 0x80);      //0Fh:CSEL1 0->1 0;UIE->0;TIE->0;AIE->0; 0; 0;RESET->0;

  RA4803_ReadData_3(_OSC_Offset_RA4803, &OscOffset_Data, 1);
  RA4803_ReadData_3(_Event_Control_RA4803, &EventControl_Data, 1);
  if (OscOffset_Data != 0x00) {
	  err |= RA4803_WriteReg_3(_OSC_Offset_RA4803, 0x00);
  }
  if (EventControl_Data != 0x00) {
	  err |= RA4803_WriteReg_3(_Event_Control_RA4803, 0x00);
  }

  /*****************************************************************/
  PutBitRESET_RA4803(_RESET_RA4803);
  err |= RA4803_WriteReg(YEAR_RA4803, 0x21); //21��                  //16h
  err |= RA4803_WriteReg(MONTH_RA4803, 0x07); //7��
  err |= RA4803_WriteReg(DAY_RA4803, 0x05);	//5��
  err |= RA4803_WriteReg(WEEK_RA4803, 0x02);//��һ
  err |= RA4803_WriteReg(HOUR_RA4803, 0x01);//1ʱ
  err |= RA4803_WriteReg(MIN_RA4803, 0x01);	//1��
  err |= RA4803_WriteReg(SEC_RA4803, 0x01);	//1��
  PutBitRESET_RA4803(0);
  /*****************************************************************/

  RA4803_ReadData(SEC_RA4803, testRA4803_Data, 16);


  err |= RA4803_ReadData(Control_Register_RA4803, &Control_Data, 1);//0Fh


  return err;
}


uint8_t RA4803Func_Check(void) {
	uint8_t err = 0;
	err = RA4803_ReadData(Control_Register_RA4803, &Control_Data, 1);//0Fh
	BackupFunc_Data = Control_Data;
	if (err == 0) {
		if ((Control_Data&0xC1) != 0x80) {
			Control_Data = (Control_Data&0x3E)|0x80;
			RA4803_WriteReg(Control_Register_RA4803, Control_Data);

		}
	}

	return 0;
}


uint8_t RA4803_Check(void) {
	uint8_t err = 0;

	RA4803Func_Check();
	//TODO
#if 0
	//SetWakeUpFlag and SetWakeUpTime wait for SPI0 communication
	if (SetWakeUpFlagCache_RA4803 != SetWakeUpFlag) {
		SetWakeUpFlagCache_RA4803 = SetWakeUpFlag;
		SetPowerOffTimer_RA4803(SetWakeUpTime);
	}
#endif
	return err;
}

/**********************************************************
**�������ƣ� RX8900_CheckInit
**���ܣ�   ��ʼRX8130��⼰��ʼ��
**����ֵ�� ͨ�Ź���
***********************************************************/
uint8_t CharVLFBuf_RA4803 = 0;
uint8_t RA4803_CheckInit(void)
{
  uint8_t err = 0;
  static uint8_t CharVLF_RA4803 = 0;
  hal_spi_init(HAL_DEV_SPI1, HAL_GPIOB_PTG, HAL_GPIO_PIN0);
  hal_systick_delay_ms(50);
  err |= GetBitVLF_RA4803(&CharVLF_RA4803);
  CharVLFBuf_RA4803 = CharVLF_RA4803;
  //CANTxData(0x18CC0000|0x80000000, (uint8_t *)(&CharVLFBuf), 1);
  while(1){
    if (CharVLF_RA4803&_FlagReg_VLF_RA4803) {
      ClearBitVLF_RA4803();
      hal_systick_delay_ms(300);
      err |= GetBitVLF_RA4803(&CharVLF_RA4803);
      if ((CharVLF_RA4803&_FlagReg_VLF_RA4803) == 0) {
         //CANTxData(0x18EE0000|0x80000000, (uint8_t *)(&CharVLF), 1);
         err |= Init_RA4803();
      } else {

      }
    } else {
      break;
    }
  }

  ClrFixCycleTimerInterrupt_RA4803();
  //SetWakeUpTime = 1;
  //SetPowerOffTimer_RA4803(SetWakeUpTime);

  return err;
}

uint8_t RTC_TransData_RA4803[7] ={0};
uint8_t GetRTCMsg_RA4803(uint8_t * pdata)
{
  uint8_t err = 0;
  RX8900_time tmpKPB08_Time;

  err = RA4803_ReadData(SEC_RA4803, RTC_TransData_RA4803, 7);

  if(err==0){
      //memcpy( pdata, RTC_TransData, 7);
     CharConvertDateTime(RTC_TransData_RA4803,&tmpKPB08_Time);

      pdata[0] =  tmpKPB08_Time.Year;
      pdata[1] =  tmpKPB08_Time.Month;
      pdata[2] =  tmpKPB08_Time.Day;
      pdata[3] =  tmpKPB08_Time.Hours;
      pdata[4] =  tmpKPB08_Time.Minute;
      pdata[5] =  tmpKPB08_Time.Second;
      pdata[6] =  tmpKPB08_Time.Week;

  }

  return err;
}


/*
uint8_t GetClockCalendar(uint8_t *rdata, uint8_t length){      //��ȡʱ��
  uint8_t err = 0;
  if (length > 7) return -1;
  err = RX8900_ReadData(SEC_RX8900, rdata, length);
  return err;
}

//����ʱ�� head[SEC]
uint8_t SetClockCalendar(uint8_t *sdata, uint8_t length) {
  uint8_t err = 0;
  uint8_t Regbuf = 0;
  //uint8_t RegCache = 0;
  if (length > 7) return -1;
  err |= RX8900_ReadData(Control_Register0, &Regbuf, 1);
  //RegCache = Regbuf|_CONTROL_REG0_STOP_;
  //err |= RX8130_WriteData(Control_Register0, &RegCache, 1);
  err |= RX8900_WriteReg(Control_Register0, Regbuf|_CONTROL_REG0_STOP_);
  err |= RX8900_ReadData(Control_Register0, &Regbuf, 1);

  if (Regbuf&_CONTROL_REG0_STOP_) {
    err |= RX8900_WriteData(SEC, sdata, length);
  } else {
    err = -1;
  }

  return err;
}
*/

uint8_t RA4803_SPI_Test(void) {
	RA4803_ReadData_3(_OSC_Offset_RA4803, &SPI_Test1[0], 1);
	RA4803_ReadData_3(_Event_Control_RA4803, &SPI_Test1[1], 1);
	//CAN_TranData(SPI_Test1, 0x700, 8);
}

uint8_t SPI1_Test_Send[8] = {1,2,3,4,5,6,7,8};
uint8_t SPI1_Test_Read[8] = {0};
uint8_t RA4803_SPI_Test2(void) {
	RA4803_SPI_Transmit(SPI1_Test_Send, SPI1_Test_Read, 8);
}


//8423BCD��ת 10����
void CharConvertDateTime(uint8_t* psdata,RX8900_time* ptime)
{
	int i=0;
	uint8_t week = 0;
	ptime->Second = (psdata[0]&0x0F)+((psdata[0]&0x70)>>4)*10;
	ptime->Minute  = (psdata[1]&0x0F)+((psdata[1]&0x70)>>4)*10;
	ptime->Hours = (psdata[2]&0x0F)+((psdata[2]&0x30)>>4)*10;
	ptime->Week = 0;
	week = psdata[3]&0x7F;
	while((week>>i))
	{
		ptime->Week = i;
		i++;
	}
	ptime->Day = (psdata[4]&0x0F)+((psdata[4]&0x30)>>4)*10;
	ptime->Month = (psdata[5]&0x0F)+((psdata[5]&0x10)>>4)*10;
	ptime->Year = (psdata[6]&0x0F)+((psdata[6]&0xF0)>>4)*10;
}

#ifdef __cplusplus
}
#endif
