/*
 * hal_i2c.c
 *
 *  Created on: 2023Äê2ÔÂ9ÈÕ
 *      Author: xinlei.hu
 */
#include "hal_i2c.h"
#include "hal_systick.h"
#include <string.h>
#include "CI2C0.h"
#include "CI2C1.h"


static LDD_TDeviceData *halI2c0DevicePtr = NULL;
static LDD_TDeviceData *halI2c1DevicePtr = NULL;

/**
 * @brief :init i2c
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_i2c_init(HAL_I2C_CLIENT_Struct *Client)
{
    //init i2c device
    switch(Client->devI2cId)
    {
        case HAL_DEV_I2C0:
            {
                halI2c0DevicePtr = CI2C0_Init(NULL);
                if(NULL == halI2c0DevicePtr)
                    return 1;
            }
            break;
        case HAL_DEV_I2C1:
            {
                halI2c1DevicePtr = CI2C1_Init(NULL);
                if(NULL == halI2c1DevicePtr)
                    return 1;
            }
            break;
        default:return 2;
    }
    return 0;
}

/**
 * @brief :deinit i2c
 *
 * @return :0 is success,1 is error
 */
uint16_t
hal_i2c_deinit(HAL_I2C_CLIENT_Struct *Client)
{
    //init i2c device
    switch(Client->devI2cId)
    {
        case HAL_DEV_I2C0:
            CI2C0_Deinit(NULL);
            break;
        case HAL_DEV_I2C1:
            CI2C1_Deinit(NULL);
            break;
        default:return 1;
    }
    return 0;
}

/**
 * @brief :enable i2c
 *
 * @return :0 is success,other is error
 */
uint16_t
hal_i2c_enable(HAL_I2C_CLIENT_Struct *Client)
{
    uint16_t u16Err = 1;
    //init i2c device
    switch(Client->devI2cId)
    {
        case HAL_DEV_I2C0:
            u16Err = CI2C0_Enable(NULL);
            break;
        case HAL_DEV_I2C1:
            u16Err = CI2C1_Enable(NULL);
            break;
        default:break;
    }
    return u16Err;
}

/**
 * @brief :disable i2c
 *
 * @return :0 is success,other is error
 */
uint16_t
hal_i2c_disable(HAL_I2C_CLIENT_Struct *Client)
{
    uint16_t u16Err = 1;
    //init i2c device
    switch(Client->devI2cId)
    {
        case HAL_DEV_I2C0:
            u16Err = CI2C0_Disable(NULL);
            break;
        case HAL_DEV_I2C1:
            u16Err = CI2C1_Disable(NULL);
            break;
        default:break;
    }
    return u16Err;
}

/**
 * @brief :set slave device addr
 *
 * @param Id :I2C dev id
 * @param AddrType :7bit or 10 bit
 * @param Addr :dev addr
 *
 * @return :0 is success;other is error
 */
uint16_t
hal_i2c_select_slave_device(HAL_DEV_I2C_Id Id, HAL_I2C_ADDR_Type AddrType,uint16_t Addr)
{
    if(HAL_I2C_ADDRTYPE_GENERAL_CALL == AddrType)
        return 1;
    switch(Id)
    {
        case HAL_DEV_I2C0:
            return CI2C0_SelectSlaveDevice(halI2c0DevicePtr, AddrType, Addr);
        case HAL_DEV_I2C1:
            return CI2C1_SelectSlaveDevice(halI2c1DevicePtr, AddrType, Addr);
        default:return 1;
    }
} 

/**
 * @brief :i2c single register write
 *
 * @param Client :HAL_I2C_CLIENT_Struct
 * @param RegAddr :reg addr
 * @param Msg :data to write
 * @param Len :data length
 * @param TimeoutMs :blocking time
 *
 * @return :0 is success;other is error
 */
uint16_t
hal_i2c_write_data_blocking(
        HAL_I2C_CLIENT_Struct *Client,
        uint8_t RegAddr,
        uint8_t *Msg,
        uint8_t Len,
        uint32_t TimeoutMs)
{
    HAL_I2C_ADDR_Type i2cAddrType;
    uint32_t u32TimeOut = 0;
    uint8_t u8SendBuf[HAL_I2C_SEND_NUM_MAX];

    if(Len > HAL_I2C_SEND_NUM_MAX -1)
    {
        return 1;
    }
    if(Client->bIs10bitAddr)
    {
        i2cAddrType = HAL_I2C_ADDRTYPE_10BITS;
    }else
    {
        i2cAddrType = HAL_I2C_ADDRTYPE_7BITS;
    }
    //copy msg
    u8SendBuf[0] = RegAddr;
    if(Len != 0)
    {
        memcpy(u8SendBuf+1, Msg, Len);
    }
    switch(Client->devI2cId)
    {
        case HAL_DEV_I2C0:
            {
                CI2C0_SelectSlaveDevice(halI2c0DevicePtr, i2cAddrType, Client->u16DevAddr);
                //send data
                do
                {
                    /* Master send data */
                    CI2C0_MasterSendBlock(halI2c0DevicePtr, u8SendBuf, (uint16_t)(Len+1), HAL_I2C_SEND_STOP);
                    u32TimeOut ++;
                    hal_systick_delay_ms(1);
                }while((CI2C0_MasterGetBlockSentStatus(halI2c0DevicePtr) == false) && (u32TimeOut < TimeoutMs));
            }
            break;
        case HAL_DEV_I2C1:
            {
                CI2C1_SelectSlaveDevice(halI2c1DevicePtr, i2cAddrType, Client->u16DevAddr);
                //send data
                do
                {
                    /* Master send data */
                    CI2C1_MasterSendBlock(halI2c1DevicePtr, u8SendBuf, (uint16_t)(Len+1), HAL_I2C_SEND_STOP);
                    u32TimeOut ++;
                    hal_systick_delay_ms(1);
                }while((CI2C1_MasterGetBlockSentStatus(halI2c1DevicePtr) == false) && (u32TimeOut < TimeoutMs));
            }
            break;
        default:return 2;
    }
    if(u32TimeOut >= TimeoutMs)
    {
        return 3;
    }
    return 0;
}

/**
 * @brief :i2c single register read
 *
 * @param Client :HAL_I2C_CLIENT_Struct
 * @param RegAddr :reg addr
 * @param Msg :storage value of read data
 * @param Len :data length
 * @param TimeoutMs :blocking time
 *
 * @return :0 is success;other is error
 */
uint16_t
hal_i2c_read_data_blocking(
        HAL_I2C_CLIENT_Struct *Client,
        uint8_t RegAddr,
        uint8_t *Msg,
        uint8_t Len,
        uint32_t TimeoutMs)
{
    HAL_I2C_ADDR_Type i2cAddrType;
    uint32_t u32TimeOut = 0;

    if(Client->bIs10bitAddr)
    {
        i2cAddrType = HAL_I2C_ADDRTYPE_10BITS;
    }else
    {
        i2cAddrType = HAL_I2C_ADDRTYPE_7BITS;
    }
    switch(Client->devI2cId)
    {
        case HAL_DEV_I2C0:
            {
                CI2C0_SelectSlaveDevice(halI2c0DevicePtr, i2cAddrType, Client->u16DevAddr);
                do
                {
                    /* Master send data */
                    CI2C0_MasterSendBlock(halI2c0DevicePtr, &RegAddr, (uint16_t)(1), HAL_I2C_SEND_STOP);
                    u32TimeOut ++;
                    hal_systick_delay_ms(1);
                }while((CI2C0_MasterGetBlockSentStatus(halI2c0DevicePtr) == false) && (u32TimeOut < TimeoutMs));
                if(u32TimeOut >= TimeoutMs)
                    return 1;
                u32TimeOut = 0;
                //recv data
                do{
                    /* Master recv data */
                    CI2C0_MasterReceiveBlock(halI2c0DevicePtr, Msg, (uint16_t)(Len), HAL_I2C_SEND_STOP);
                    u32TimeOut ++;
                    hal_systick_delay_ms(1);
                }while((CI2C0_MasterGetBlockReceivedStatus(halI2c0DevicePtr) == false) && (u32TimeOut < TimeoutMs));
                if(u32TimeOut >= TimeoutMs)
                    return 2;
            }
            break;
        case HAL_DEV_I2C1:
            {
                CI2C1_SelectSlaveDevice(halI2c1DevicePtr, i2cAddrType, Client->u16DevAddr);
                do
                {
                    /* Master send data */
                    CI2C1_MasterSendBlock(halI2c1DevicePtr, &RegAddr, (uint16_t)(1), HAL_I2C_SEND_STOP);
                    u32TimeOut ++;
                    hal_systick_delay_ms(1);
                }while((CI2C1_MasterGetBlockSentStatus(halI2c1DevicePtr) == false) && (u32TimeOut < TimeoutMs));
                if(u32TimeOut >= TimeoutMs)
                    return 1;
                u32TimeOut = 0;
                //recv data
                do{
                    /* Master recv data */
                    CI2C1_MasterReceiveBlock(halI2c1DevicePtr, Msg, (uint16_t)(Len), HAL_I2C_SEND_STOP);
                    u32TimeOut ++;
                    hal_systick_delay_ms(1);
                }while((CI2C1_MasterGetBlockReceivedStatus(halI2c1DevicePtr) == false) && (u32TimeOut < TimeoutMs));
                if(u32TimeOut >= TimeoutMs)
                    return 2;
            }
            break;
        default:return 3;
    }
    return 0;
}

/**
 * @brief :tx callback
 */
void
hal_i2c_tx_callback(HAL_I2C_CLIENT_Struct *Client)
{
	(void) Client;
}

/**
 * @brief :rx callback
 */
void
hal_i2c_rx_callback(HAL_I2C_CLIENT_Struct *Client)
{
	(void) Client;
}









