/*
 * service_di.h
 *
 *  Created on: 2023Äê9ÔÂ20ÈÕ
 *      Author: xinlei.hu
 */
#ifndef SERVICES_DI_SERVICE_DI_H_
#define SERVICES_DI_SERVICE_DI_H_
#include <stdint.h>


extern void di_service_input_io_init(void);
extern uint16_t di_service_input_io_read(uint32_t *DI_0, uint32_t *DI_1);


#endif /* SERVICES_DI_SERVICE_DI_H_ */
