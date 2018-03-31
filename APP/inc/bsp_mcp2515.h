#ifndef _BSP_MCP2515_H
#define _BSP_MCP2515_H
#include "main.h"

#define GPIO_Output 1
#define GPIO_Input 	0
#define LPC_SPI_CAN ((LPC_CAN_TypeDef *) 0x1)
	/*SPI���ź궨��*/
#define SPI_PORT 						(0)
#define SPI_PIN_MOSI 				    (18)
#define SPI_PIN_MISO 					(17)
#define SPI_PIN_SSEL 					(16)
#define SPI_PIN_SCK 					(15)
#define SPI_PIN_func 					(3)
#define SPI_PIN_SSEL_MASK 				(0x1<<16)
/*MCP2515_REST_PORT���ź궨��*/
#define MCP2515_REST_PORT 				(2)
#define MCP2515_REST_MASK 				(0x1<<3)
#define MCP2515_CAN_INTER_POST			(2)
#define MCP2515_CAN_INTER_MASK	   	    (0x1<<4)

/*****************************************************/
#define MCP2515_TXB_CTRL_TXREQ          (0x1<<3)
/*MCP251ָ�*/
#define MCP2515_Read 					0x3
#define MCP2515_Write 					0x2
#define MCP2515_TXB_send 				0xb
#define MCP2515_CANSTAT 				0x0e

#define MCP2515_CNF1 					0x2a
#define MCP2515_CNF2 					0x29
#define MCP2515_CNF3 					0x28
#define MCP2515_CAN_CTRL 				0xf

#define MCP2515_TXB1_BASE				0x30
#define MCP2515_TXB1_CTRL 			    MCP2515_TXB1_BASE+0
#define MCP2515_TXB1_SIDH 			    MCP2515_TXB1_BASE+1
#define MCP2515_TXB1_SIDL 			    MCP2515_TXB1_BASE+2
#define MCP2515_TXB1_EID8 			    MCP2515_TXB1_BASE+3
#define MCP2515_TXB1_EID0 			    MCP2515_TXB1_BASE+4
#define MCP2515_TXB1_DLL 				MCP2515_TXB1_BASE+5
#define MCP2515_TXB1_D0 				MCP2515_TXB1_BASE+6
#define MCP2515_TXB1_D1 				MCP2515_TXB1_BASE+7
#define MCP2515_TXB1_D2 				MCP2515_TXB1_BASE+8
#define MCP2515_TXB1_D3 				MCP2515_TXB1_BASE+9
#define MCP2515_TXB1_D4 				MCP2515_TXB1_BASE+10
#define MCP2515_TXB1_D5 				MCP2515_TXB1_BASE+11
#define MCP2515_TXB1_D6 				MCP2515_TXB1_BASE+12
#define MCP2515_TXB1_D7 				MCP2515_TXB1_BASE+13

#define MCP2515_TXB2_BASE				0x40
#define MCP2515_TXB2_CTRL 			    MCP2515_TXB2_BASE+0
#define MCP2515_TXB2_SIDH 		    	MCP2515_TXB2_BASE+1
#define MCP2515_TXB2_SIDL 			    MCP2515_TXB2_BASE+2
#define MCP2515_TXB2_EID8 			    MCP2515_TXB2_BASE+3
#define MCP2515_TXB2_EID0 		    	MCP2515_TXB2_BASE+4
#define MCP2515_TXB2_DLL 				MCP2515_TXB2_BASE+5
#define MCP2515_TXB2_D0 				MCP2515_TXB2_BASE+6
#define MCP2515_TXB2_D1 				MCP2515_TXB2_BASE+7
#define MCP2515_TXB2_D2 				MCP2515_TXB2_BASE+8
#define MCP2515_TXB2_D3 				MCP2515_TXB2_BASE+9
#define MCP2515_TXB2_D4 				MCP2515_TXB2_BASE+10
#define MCP2515_TXB2_D5 				MCP2515_TXB2_BASE+11
#define MCP2515_TXB2_D6 				MCP2515_TXB2_BASE+12
#define MCP2515_TXB2_D7 				MCP2515_TXB2_BASE+13

#define MCP2515_TXB3_BASE				0x50
#define MCP2515_TXB3_CTRL 			    MCP2515_TXB3_BASE+0
#define MCP2515_TXB3_SIDH 			    MCP2515_TXB3_BASE+1
#define MCP2515_TXB3_SIDL 		    	MCP2515_TXB3_BASE+2
#define MCP2515_TXB3_EID8 		    	MCP2515_TXB3_BASE+3
#define MCP2515_TXB3_EID0 			    MCP2515_TXB3_BASE+4
#define MCP2515_TXB3_DLL 				MCP2515_TXB3_BASE+5
#define MCP2515_TXB3_D0 				MCP2515_TXB3_BASE+6
#define MCP2515_TXB3_D1 				MCP2515_TXB3_BASE+7
#define MCP2515_TXB3_D2 				MCP2515_TXB3_BASE+8
#define MCP2515_TXB3_D3 				MCP2515_TXB3_BASE+9
#define MCP2515_TXB3_D4 				MCP2515_TXB3_BASE+10
#define MCP2515_TXB3_D5 				MCP2515_TXB3_BASE+11
#define MCP2515_TXB3_D6 				MCP2515_TXB3_BASE+12
#define MCP2515_TXB3_D7 				MCP2515_TXB3_BASE+13



#define MCP2515_RXB1CTRL  			     0X70
#define MCP2515_CANINTE 	 			 0x2b
#define MCP2515_CANINTF 	 			 0x2c

#define MCP2515_RXB0CTRL  			     0X60
#define MCP2515_RXB0SIDH                 0x61
#define MCP2515_RXB0SIDL                 0x62
#define MCP2515_RXB0EID8                 0x63
#define MCP2515_RXB0EID0                 0x64
#define MCP2515_RXB0DLC               	 0x65
#define MCP2515_RXB0DM0                	 0x66
#define MCP2515_RXB0DM1              	 0x67
#define MCP2515_RXB0DM2             	 0x68
#define MCP2515_RXB0DM3               	 0x69
#define MCP2515_RXB0DM4               	 0x6a
#define MCP2515_RXB0DM5       	         0x6b
#define MCP2515_RXB0DM6       	         0x6c
#define MCP2515_RXB0DM7       	         0x6d

#define MCP2515_RXB1CTRL  			     0X70
#define MCP2515_RXB1SIDH                 0x71
#define MCP2515_RXB1SIDL                 0x72
#define MCP2515_RXB1EID8                 0x73
#define MCP2515_RXB1EID0                 0x74
#define MCP2515_RXB1DLC        	         0x75
#define MCP2515_RXB1DM0        	         0x76
#define MCP2515_RXB1DM1       	         0x77
#define MCP2515_RXB1DM2       	         0x78
#define MCP2515_RXB1DM3       	         0x79
#define MCP2515_RXB1DM4       	         0x7a
#define MCP2515_RXB1DM5       	         0x7b
#define MCP2515_RXB1DM6       	         0x7c
#define MCP2515_RXB1DM7       	         0x7d
#define MCP2515_TX_TEC 					 0x1c



void SPI_CAN_IntHandler(CAN_MSG_Type *CAN_Msg);

void SPI_WriteAddOneData(const uint8_t addr,const uint8_t data);

uint8_t SPI_ReadAddOneData(const uint8_t addr);

void Appliction_SPI_To_MCP2515_Init(void);
uint8_t MCP2515_CAN_SendMsg (CAN_MSG_Type * CAN_Msg);

#endif

