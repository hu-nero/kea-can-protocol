/*
 * service_switch_ctrl.h
 *
 *  Created on: 2023Äê9ÔÂ20ÈÕ
 *      Author: xinlei.hu
 */

#ifndef SERVICES_SWITCH_CTRL_SERVICE_SWITCH_CTRL_H_
#define SERVICES_SWITCH_CTRL_SERVICE_SWITCH_CTRL_H_
#include <stdint.h>


extern void switch_ctrl_service_output_io_init(void);
extern uint16_t switch_ctrl_service_output_ctrl(uint8_t BitState, uint8_t Mask, uint32_t *Value);


#endif /* SERVICES_SWITCH_CTRL_SERVICE_SWITCH_CTRL_H_ */
