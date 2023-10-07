/*
 * board_init.c
 *
 *  Created on: 2023Äê8ÔÂ29ÈÕ
 *      Author: xinlei.hu
 */
#include "board_init.h"
#include "Hal.h"

_bootcfg_information gBootInfo;
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
    hal_can_init(eCanPort_0, gBootInfo.CANBaudMode);

}

void
boot_read(void)
{
	uint8_t *pstr,*pdtr;
	pstr=(uint8_t*)CALI_BASE_ADDR;
	pdtr=(uint8_t*)(&gBootInfo);
	for(uint16_t i=0;i<(sizeof(_bootcfg_information)-2);i++)
	{
		*(pdtr++)=*(pstr++);
	}
}
