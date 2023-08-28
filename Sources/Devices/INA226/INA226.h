/*
 * INA226.h
 *
 *  Created on: 2023年5月8日
 *      Author: xinlei.hu
 */

#ifndef SOURCES_DEVICES_INA226_INA226_H_
#define SOURCES_DEVICES_INA226_INA226_H_

#include <stdint.h>

#define _INA226_R_ADDR      0x45
#define _INA226_CFG			0x4027	//平均样本1，VBUS测量时间为140us，VSHUNT为1.1ms，数据更新时间为1.24ms


typedef enum {
	Cfg_Reg = 0x00,
	Shunt_Reg = 0x01,
	BusVol_Reg = 0x02,
	Power_Reg = 0x03,
	Current_Reg = 0x04,
	Calib_Reg = 0x05,
	MaskEn_Reg = 0x06,
	AlertLimit_Reg = 0x07,
	ManuID_Reg = 0xFE,
	DieID_Reg = 0xFF
}INA226_PRG_Addr;


extern uint8_t INA226_R_Init(void);

#endif /* SOURCES_DEVICES_INA226_INA226_H_ */
