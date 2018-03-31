#ifndef _BSP_CAN_H
#define _BSP_CAN_H
#include "main.h"

/**********宏定义发送CAN数据函数**************************************************************/
#define CAN_1 								1   //选择CAN1通道
#define CAN_2 								2   //选择CAN2通道
#define CAN_3 							    3   //选择CAN3通道
//#define CAN1_SendMsg(LPCTxMsg, buf, dlc, id, ID_type)     Appliction_CanData_Init(LPCTxMsg, buf, dlc, id, ID_type); \
//                                                              Appliction_Can_Transmit(LPC_CAN1,LPCTxMsg);
//#define CAN2_SendMsg(LPCTxMsg, buf, dlc, id, ID_type)     Appliction_CanData_Init(LPCTxMsg, buf, dlc, id, ID_type); \
//                                                              Appliction_Can_Transmit(LPC_CAN2,LPCTxMsg);
//#define SPI_CAN_SendMsg(LPCTxMsg, buf, dlc, id, ID_type)  Appliction_CanData_Init(LPCTxMsg, buf, dlc, id, ID_type); \
                                                              MCP2515_CAN_SendMsg(LPCTxMsg);
#define CANInit_SendMsg(channel, LPCTxMsg, buf, dlc, id, ID_type)  Appliction_CanData_Init(LPCTxMsg, buf, dlc, id, ID_type); \
                                                                       Can_Transmit(channel, LPCTxMsg);
                                                                       
/**********Set FIFO**************************************************************/
#define CAN1RxFifoSize     128
#define CAN2RxFifoSize     128
#define CAN3RxFifoSize     128
#define CAN1TxFifoSize     128
#define CAN2TxFifoSize     128
#define CAN3TxFifoSize     128

EXPORT_FIFO_API(CAN1RxFifo, CAN1RxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN1Rx FIFO
EXPORT_FIFO_API(CAN1TxFifo, CAN1TxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN1Tx FIFO
EXPORT_FIFO_API(CAN2TxFifo, CAN2TxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN2Tx FIFO
EXPORT_FIFO_API(CAN2RxFifo, CAN2RxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN2Rx FIFO
EXPORT_FIFO_API(CAN3RxFifo, CAN3RxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN3Rx FIFO
EXPORT_FIFO_API(CAN3TxFifo, CAN3TxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN3Tx FIFO

/******************************************************************************/
#define CAN_ID_STD                 ((uint32_t)0x00000000)  /*!< Standard Id */
#define CAN_ID_EXT                 ((uint32_t)0x00000004)  /*!< Extended Id */
#define CAN_RTR_DATA               ((uint32_t)0x00000000)  /*!< Data frame */
#define CAN_RTR_REMOTE             ((uint32_t)0x00000002)  /*!< Remote frame */
/*****************************************************************************/

void Appliction_Can_Init(void);
void Appliction_CanData_Init(CAN_MSG_Type * LPCTxMsg,uint8_t *buf,uint8_t dlc,uint32_t id,uint32_t ID_type);
void Appliction_Can_Transmit(LPC_CAN_TypeDef *CANx,CAN_MSG_Type *CAN_Msg);
void Can_Transmit(uint8_t CAN_N,CAN_MSG_Type * CAN_MSG);
void ADD_data_to_CANxTxFifo(uint8_t CAN_N, CAN_MSG_Type TxMessage);
void Send_CANxTxFifo_data(uint8_t CAN_N);

#endif







