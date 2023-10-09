/*
 * board_init.c
 *
 *  Created on: 2023Äê8ÔÂ29ÈÕ
 *      Author: xinlei.hu
 */
#include "board_init.h"
#include <string.h>
#include "Hal.h"

_calibratecfg_info gBootInfo;
extern void boot_read(void);

void
board_init(void)
{
    //systick init
    hal_systick_init();
    //gpio init
    hal_gpio_init();
    //flash init
    hal_flash_init(HAL_DEV_FLASH0);
    //boot read
    boot_read();
    //can init
    hal_can_init(eCanPort_0, gBootInfo.u32CanBaudValue);

}

void
boot_read(void)
{
	memcpy(&gBootInfo, (uint8_t*)CALI_BASE_ADDR, sizeof(_calibratecfg_info));
    if (gBootInfo.u32Flag != CALI_CFG_FLAG)
    {
        gBootInfo.u32Flag = CALI_CFG_FLAG;
        gBootInfo.u32CanBaudValue = CAN_BAUD_500;
        gBootInfo.u32CanRequestId = CAN_DEFAULT_ID_REQUEST;
        gBootInfo.u32CanResponseId = CAN_DEFAULT_ID_RESPONSE;

        //write flash
        hal_flash_erase(HAL_DEV_FLASH0, CALI_BASE_ADDR);
        uint8_t len = sizeof(gBootInfo);
        uint32_t addr = CALI_BASE_ADDR;
        uint8_t *ptr = (uint8_t*)&gBootInfo;
        while (len)
        {
            if (len >= FLASHOPERATEUINT)
            {
                len -= FLASHOPERATEUINT;
                hal_flash_write(HAL_DEV_FLASH0, ptr, addr, FLASHOPERATEUINT);
                ptr += FLASHOPERATEUINT;
                addr += FLASHOPERATEUINT;
            }
            else
            {
                hal_flash_write(HAL_DEV_FLASH0, ptr, addr, len);
                len = 0;
            }
        }
    }
}










