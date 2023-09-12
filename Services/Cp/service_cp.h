/*
 * service_cp.h
 *
 *  Created on: 2023Äê9ÔÂ11ÈÕ
 *      Author: xinlei.hu
 */

#ifndef SERVICES_CP_SERVICE_CP_H_
#define SERVICES_CP_SERVICE_CP_H_
#include <stdint.h>

#define SERVICE_CP_PARAMETER_CHSIZE     2u


typedef enum
{
    SERVICE_CP_SIMULATE_DUTY = 0,
    SERVICE_CP_SIMULATE_FREQ,
    SERVICE_CP_SIMULATE_MASK
} SERVICE_CP_SIMULATE_Enum;


extern uint16_t cp_service_ctrl(uint8_t Duty, uint16_t Freq, uint8_t Mask, uint8_t *RealDuty, uint16_t *RealFreq);

#endif /* SERVICES_CP_SERVICE_CP_H_ */
