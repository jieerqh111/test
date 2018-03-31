#include "bsp_can.h"

//休眠和待机变量
extern Bool ACCSecondFlag;
extern Bool StandbyFlag;

NEW_FIFO(CAN1RxFifo, CAN1RxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN1Rx FIFO
NEW_FIFO(CAN1TxFifo, CAN1TxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN1Tx FIFO
NEW_FIFO(CAN2TxFifo, CAN2TxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN2Tx FIFO
NEW_FIFO(CAN2RxFifo, CAN2RxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN2Rx FIFO
NEW_FIFO(CAN3RxFifo, CAN3RxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN3Rx FIFO
NEW_FIFO(CAN3TxFifo, CAN3TxFifoSize, CAN_MSG_Type, uint32_t);//声明CAN3Tx FIFO

/*****************************************************************************
**函数名称:	  CAN_INIT
**函数功能:	  初始化can通道
**入口参数:
**返回参数:
******************************************************************************/
void Appliction_Can_Init(void)
{
	PINSEL_CFG_Type PinCfg;
	/* Pin configuration
	 * CAN1: select P0.0 as RD1. P0.1 as TD1
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 0;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);

	/* Pin configuration
	 * CAN2: select P2.7 as RD2; P2.8 as TD2
	 */
	PinCfg.Pinnum = 7;
	PinCfg.Portnum = 2;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 8;
	PINSEL_ConfigPin(&PinCfg);
    
	CAN_Init(LPC_CAN1, 500000);				 //初始化1通道	
	CAN_SetAFMode (LPC_CANAF,CAN_AccOff);	 //模式
	CAN_Init(LPC_CAN2, 500000);				 //初始化2通道
	CAN_SetAFMode (LPC_CANAF,CAN_AccOff);	 //模式
	CAN_IRQCmd(LPC_CAN1, CANINT_RIE, ENABLE);//CAN1中断使能
	CAN_IRQCmd(LPC_CAN2, CANINT_RIE, ENABLE);//CAN2中断使能
	NVIC_SetPriority(CAN_IRQn, 2);           //设置CAN中断优先级
	NVIC_EnableIRQ(CAN_IRQn);				 //CAN中断使能
 
    SPi_Can_3_Init();      //SPI_CAN初始化
 
    FIFO_INIT(CAN1TxFifo); //初始化FIFO
    FIFO_INIT(CAN2TxFifo);
    FIFO_INIT(CAN3TxFifo);
    FIFO_INIT(CAN1RxFifo);
    FIFO_INIT(CAN2RxFifo);
    FIFO_INIT(CAN3RxFifo);
}

/*****************************************************************************
**函数名称:	  Appliction_CanData_Init
**函数功能:	  把can数据转换成标准数据准备发送
**入口参数:			
**返回参数:
******************************************************************************/
void Appliction_CanData_Init(CAN_MSG_Type * LPCTxMsg,uint8_t *buf,uint8_t dlc,uint32_t id,uint32_t ID_type)
{
	uint8_t i;
	LPCTxMsg->len = dlc;  //长度
	if(ID_type == CAN_ID_STD)
	{
		LPCTxMsg->format = STD_ID_FORMAT;  //标准帧
		LPCTxMsg->id = (id & 0x7fful);
	}
	else
	{
		LPCTxMsg->format= EXT_ID_FORMAT;  //扩展帧
		LPCTxMsg->id = (id & 0x1ffffffful);
	}

	LPCTxMsg->type = DATA_FRAME;	      //数据帧

	for(i=0;(i<dlc)&&(i<4);i++)
	LPCTxMsg->dataA[i] = buf[i];
	for(;i<4;i++)	
	LPCTxMsg->dataA[i] = 0;
	for(;(i<dlc)&&(i<8);i++)
	LPCTxMsg->dataB[i-4] = buf[i];
	for(;i<8;i++)	
	LPCTxMsg->dataB[i-4] = 0;	   
}

/*******************************************************************************
**函数名称:	  ADD_data_to_CANxTxFifo
**函数功能:	  添加数据到CANx发送缓冲区
**入口参数:			
**返回参数:
*******************************************************************************/
void ADD_data_to_CANxTxFifo(uint8_t CAN_N, CAN_MSG_Type TxMessage)
{	
    switch(CAN_N)
    {
        case 1://储存到CAN1TxFifo
            FIFO_PUT_ONE(CAN1TxFifo, TxMessage);
            break;
        case 2://储存到CAN2TxFifo
            FIFO_PUT_ONE(CAN2TxFifo, TxMessage);
            break;
        case 3://储存到CAN3TxFifo
            FIFO_PUT_ONE(CAN3TxFifo, TxMessage);
            break;
        default:
            break;
    }		
}

/*****************************************************************************
**函数名称:	  Appliction_Can_Transmit
**函数功能:	  发送can数据
**入口参数:	   
**返回参数:
******************************************************************************/
void Appliction_Can_Transmit(LPC_CAN_TypeDef *CANx,CAN_MSG_Type *CAN_Msg)
{
//    while(CAN_GetCTRLStatus(CANx,CANCTRL_GLOBAL_STS)&(1<<5));//等待发送器空闲	

	NVIC_DisableIRQ(CAN_IRQn);
	CAN_SendMsg(CANx,CAN_Msg);//发送数据
	NVIC_EnableIRQ(CAN_IRQn);
}


/*****************************************************************************
**函数名称:	  Can_Transmit
**函数功能:	  发送CAN信息
**入口参数:	  发送CAN数据的结构体
**返回参数:  

extern Bool ACCSecondFlag;
extern Bool StandbyFlag;

******************************************************************************/
void Can_Transmit(uint8_t CAN_N, CAN_MSG_Type * CAN_MSG)
{
	static uint32_t IntervalTime;
	while(1)//两次发送CAN数据的间隔时间为2ms
    {
        if(ReadUserTimer(&IntervalTime)>2)
        break;
    }
    ResetUserTimer(&IntervalTime);
   
//    switch(CAN_N)
//    {
//        case 1://通道1发送
//            Appliction_Can_Transmit(LPC_CAN1,CAN_MSG);
//            break;
//        case 2://通道2发送
//            Appliction_Can_Transmit(LPC_CAN2,CAN_MSG);
//            break;
//        case 3://通道3发送
//            MCP2515_CAN_SendMsg(CAN_MSG);
//            break;
//        default:
//            break;
//    }		
    
    if(StandbyFlag==FALSE) //不待机
    {
        if(ACCSecondFlag==TRUE) //ACC按下
        {
            switch(CAN_N)
            {   
                case CAN_1://通道1发送
                    Appliction_Can_Transmit(LPC_CAN1,CAN_MSG);
                    break;
                case CAN_2://通道2发送
                    Appliction_Can_Transmit(LPC_CAN2,CAN_MSG);
                    break;
                case CAN_3://通道3发送
                    MCP2515_CAN_SendMsg(CAN_MSG);
                    break;
                default:
                    break;
            }		
        }
    }
}


/*******************************************************************************
**函数名称:	  Send_CANxTxFifo
**函数功能:	  从CANxTxFifo中取出数据，发送
**入口参数:			
**返回参数:
*******************************************************************************/
void Send_CANxTxFifo_data(uint8_t CAN_N)
{
	CAN_MSG_Type CAN_MSG;
    switch(CAN_N)
    {
        case CAN_1://发送CAN1TxFifo中的数据
            if(FIFO_GET_ONE(CAN1TxFifo, &CAN_MSG)) 
            {
                Can_Transmit(CAN_1, &CAN_MSG);
            }
            break;
        case CAN_2://发送CAN2TxFifo中的数据
            if(FIFO_GET_ONE(CAN2TxFifo, &CAN_MSG)) 
            {
                Can_Transmit(CAN_2, &CAN_MSG);
            }
            break;
        case CAN_3://发送CAN3TxFifo中的数据
            if(FIFO_GET_ONE(CAN3TxFifo, &CAN_MSG)) 
            {
                Can_Transmit(CAN_3, &CAN_MSG);
            }
            break;
        default:
            break;
    }		
}

/*****************************************************************************
**函数名称:	  CAN_IRQHandler
**函数功能:	  中断处理函数
**入口参数:			
**返回参数:
******************************************************************************/
void CAN_IRQHandler (void)
{				
	uint8_t CAN1IntStatus;
	uint8_t CAN2IntStatus;
    CAN_MSG_Type CAN1RXMsg;
    CAN_MSG_Type CAN2RXMsg;
    
	CAN1IntStatus = CAN_IntGetStatus(LPC_CAN1);
	CAN2IntStatus = CAN_IntGetStatus(LPC_CAN2);
    
	if((CAN1IntStatus>>0)&0x01) //bit0: 接收中断
	{
		CAN_ReceiveMsg(LPC_CAN1,&CAN1RXMsg);
    }
	if((CAN2IntStatus>>0)&0x01)
	{
		CAN_ReceiveMsg(LPC_CAN2,&CAN2RXMsg);
    }
}
