/*
 * board_config.h
 *
 *  Created on: 2023Äê8ÔÂ31ÈÕ
 *      Author: xinlei.hu
 */

#ifndef BOARD_BOARD_CONFIG_H_
#define BOARD_BOARD_CONFIG_H_
#include <stdint.h>

extern uint8_t SoftsVer[];

#define CAT_NUM_BIT(a, bit)             (((a) >> (bit)) & 1u)
#define SET_NUM_BIT(a, bit)             ((a) |= (1u << (bit)))
#define RESET_NUM_BIT(a, bit)           ((a) &= ~(1u << (bit)))



#endif /* BOARD_BOARD_CONFIG_H_ */
