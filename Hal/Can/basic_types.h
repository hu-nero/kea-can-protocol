/*
 * basic_types.h
 *
 *  Created on: Aug 31, 2017
 *      Author: daniel
 */

#ifndef DEV_MCAL_SHAREDCAN_BASIC_TYPES_H_
#define DEV_MCAL_SHAREDCAN_BASIC_TYPES_H_

#include <stdint.h>
#include <stdbool.h>


typedef enum
{
    eErrorOk,
    eErrorNotOk,
    eErrorBusy,
    eErrorUninit,
    eErrorOutOfRange,
    eErrorSNA,
    eErrorMIA,
    eErrorTimeout,
    eErrorNullPtr,
    eErrorExists,
    eErrorNoExist
} TeErrorEnum;

#endif /* DEV_MCAL_SHAREDCAN_BASIC_TYPES_H_ */
