/*
 * service_ntc.h
 *
 *  Created on: 2023Äê9ÔÂ19ÈÕ
 *      Author: xinlei.hu
 */

#ifndef SERVICES_NTC_SERVICE_NTC_H_
#define SERVICES_NTC_SERVICE_NTC_H_
#include <stdint.h>

#define SERVICES_NTC_CH_TEST            0u
#define SERVICES_NTC_CH_0               0u
#define SERVICES_NTC_CH_1               1u

#define SERVICES_NTC_R0_R1_58K      58050u
#define SERVICES_NTC_R0_R1_13K      13570u
#define SERVICES_NTC_R0_R1_2_45K     2450u
#define SERVICES_NTC_R0_R1_2_97K     2970u

#define SERVICES_NTC_R1_56K         56670u
#define SERVICES_NTC_R1_12K         12020u
#define SERVICES_NTC_R1_2_09K        2090u
#define SERVICES_NTC_R1_2_57K        2570u

#define SERVICES_NTC_R0_56K         56740u
#define SERVICES_NTC_R0_12K         12120u
#define SERVICES_NTC_R0_2_10K        2100u
#define SERVICES_NTC_R0_2_68K        2680u


extern void ntc_service_io_init(void);
extern void ntc_service_io_reset(void);
extern uint16_t ntc_service_r_ctrl(uint16_t NTC_R, uint8_t Ch, uint16_t *RValue, uint8_t *ChState);


#endif /* SERVICES_NTC_SERVICE_NTC_H_ */
