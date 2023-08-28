/*
 * hal_can.h
 *
 *  Created on: 2023Äê1ÔÂ30ÈÕ
 *      Author: xinlei.hu
 */

#ifndef HAL_CAN_HAL_CAN_H_
#define HAL_CAN_HAL_CAN_H_

#include "stdint.h"

#define CAN_DRIVER_MAX_DLC       (8u)
#define CAN_DRIVER_TX_NUM        (50u)
#define CAN_DRIVER_RX_NUM        (50u)


typedef enum {                         /*!< Type specifying the CAN frame type */
  HAL_CAN_DATA_FRAME,                  /*!< Data frame type received or transmitted */
  HAL_CAN_REMOTE_FRAME,                /*!< Remote frame type  */
  HAL_CAN_RESPONSE_FRAME               /*!< Response frame type - Tx buffer send data after receiving remote frame with the same ID */
} HAL_CAN_TFrameType;

typedef enum{
    HAL_CAN_ER_ERROR,
    HAL_CAN_ER_BUSOFF
} HAL_CAN_ER_TYPE;

typedef struct {
  uint32_t  u32MessageId;        /*!< Message ID */
  HAL_CAN_TFrameType halFrameType;        /*!< Type of the frame DATA/REMOTE */
  uint8_t *u8Data;                       /*!< Message data buffer */
  uint8_t  u8Length;                     /*!< Message length */
  uint16_t u16TimeStamp;                  /*!< Message time stamp */
  uint8_t  u8LocPriority;                /*!< Local Priority Tx Buffers */
} HAL_CAN_TFrame;

typedef struct{
    uint8_t u8MsgId;
    uint32_t u32CanTxCount;
    HAL_CAN_TFrame halCanTxFrame[CAN_DRIVER_TX_NUM];
} HAL_CAN_Tx_Struct;

typedef struct{
    uint8_t u8MsgId;
    uint32_t u32CanRxCount;
    HAL_CAN_TFrame halCanRxFrame[CAN_DRIVER_RX_NUM];
} HAL_CAN_Rx_Struct;

extern HAL_CAN_Tx_Struct gHalCanTxStruct;
extern HAL_CAN_Rx_Struct gHalCanRxStruct;
extern uint32_t gHalCanErrorCount;

extern uint16_t hal_can_init(void);
extern uint16_t hal_can_deinit(void);
extern uint16_t hal_can_transmit(uint32_t can_id, HAL_CAN_TFrameType type, uint8_t dlc, const uint8_t *pbuf);

extern void hal_can_error_callback(HAL_CAN_ER_TYPE u8CanErrorType);
extern void hal_can_tx_callback(uint8_t BufferIdx);
extern void hal_can_rx_callback(uint8_t BufferIdx);
#endif /* HAL_CAN_HAL_CAN_H_ */
