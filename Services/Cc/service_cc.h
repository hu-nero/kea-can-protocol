/*
 * service_cc.h
 *
 *  Created on: 2023Äê9ÔÂ19ÈÕ
 *      Author: xinlei.hu
 */

#ifndef SERVICES_CC_SERVICE_CC_H_
#define SERVICES_CC_SERVICE_CC_H_
#include <stdint.h>



extern void cc_service_io_init(void);
extern void cc_service_io_reset(void);
extern uint16_t cc_service_r_ctrl(uint16_t CC_R, uint8_t CC2, uint8_t Mask, uint16_t *RValue, uint8_t *CC2State);

#endif /* SERVICES_CC_SERVICE_CC_H_ */
