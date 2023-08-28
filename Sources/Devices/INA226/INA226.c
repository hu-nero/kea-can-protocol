/*
 * INA226.c
 *
 *  Created on: 2023Äê5ÔÂ8ÈÕ
 *      Author: xinlei.hu
 */
#include "INA226.h"
#include <string.h>
#include "Hal.h"

HAL_I2C_CLIENT_Struct halI2cClient = {
	.devI2cId = HAL_DEV_I2C0,
	.u16DevAddr = _INA226_R_ADDR,
	.bIs10bitAddr = false
};
uint8_t IIC_InaR_TxData[4] = {0};
uint8_t IIC_InaR_RdData[4] = {0};

static uint8_t
INA226_R_SendData(INA226_PRG_Addr Reg, uint8_t *pdata, uint8_t len)
{
    return hal_i2c_write_data_blocking(&halI2cClient, Reg, pdata, len, 10);
}

static uint8_t
INA226_R_ReadData(INA226_PRG_Addr Reg, uint8_t *pdata, uint8_t len)
{
    return hal_i2c_read_data_blocking(&halI2cClient, Reg, pdata, len, 10);
}

uint8_t
INA226_R_Init(void)
{
  	uint16_t u16Err = 0;
    //i2c init
    u16Err = hal_i2c_init(&halI2cClient);
	IIC_InaR_TxData[0] = 0x41;
	IIC_InaR_TxData[1] = 0x67;
	u16Err = INA226_R_SendData(Cfg_Reg, IIC_InaR_TxData, 2);
	return u16Err;
}

uint8_t
INA226_R_GetRegData(uint8_t reg, uint16_t *rdata)
{
	uint8_t err = 0;
	uint16_t rdatabuf = 0;
	IIC_InaR_TxData[0] = reg;
	memset(IIC_InaR_RdData, 0, sizeof(IIC_InaR_RdData));

	err |= INA226_R_SendData(reg, NULL, 0);
	err |= INA226_R_ReadData(reg, IIC_InaR_RdData, 2);
	rdatabuf |= (uint16_t)(((uint16_t)IIC_InaR_RdData[0])<<8);
	rdatabuf |= (uint16_t)(((uint16_t)IIC_InaR_RdData[1]));
	rdata[0] = rdatabuf;
	return err;
}








