#include "bsp_can.h"

//���ߺʹ�������
extern Bool ACCSecondFlag;
extern Bool StandbyFlag;

NEW_FIFO(CAN1RxFifo, CAN1RxFifoSize, CAN_MSG_Type, uint32_t);//����CAN1Rx FIFO
NEW_FIFO(CAN1TxFifo, CAN1TxFifoSize, CAN_MSG_Type, uint32_t);//����CAN1Tx FIFO
NEW_FIFO(CAN2TxFifo, CAN2TxFifoSize, CAN_MSG_Type, uint32_t);//����CAN2Tx FIFO
NEW_FIFO(CAN2RxFifo, CAN2RxFifoSize, CAN_MSG_Type, uint32_t);//����CAN2Rx FIFO
NEW_FIFO(CAN3RxFifo, CAN3RxFifoSize, CAN_MSG_Type, uint32_t);//����CAN3Rx FIFO
NEW_FIFO(CAN3TxFifo, CAN3TxFifoSize, CAN_MSG_Type, uint32_t);//����CAN3Tx FIFO

/*****************************************************************************
**��������:	  CAN_INIT
**��������:	  ��ʼ��canͨ��
**��ڲ���:
**���ز���:
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
    
	CAN_Init(LPC_CAN1, 500000);				 //��ʼ��1ͨ��	
	CAN_SetAFMode (LPC_CANAF,CAN_AccOff);	 //ģʽ
	CAN_Init(LPC_CAN2, 500000);				 //��ʼ��2ͨ��
	CAN_SetAFMode (LPC_CANAF,CAN_AccOff);	 //ģʽ
	CAN_IRQCmd(LPC_CAN1, CANINT_RIE, ENABLE);//CAN1�ж�ʹ��
	CAN_IRQCmd(LPC_CAN2, CANINT_RIE, ENABLE);//CAN2�ж�ʹ��
	NVIC_SetPriority(CAN_IRQn, 2);           //����CAN�ж����ȼ�
	NVIC_EnableIRQ(CAN_IRQn);				 //CAN�ж�ʹ��
 
    SPi_Can_3_Init();      //SPI_CAN��ʼ��
 
    FIFO_INIT(CAN1TxFifo); //��ʼ��FIFO
    FIFO_INIT(CAN2TxFifo);
    FIFO_INIT(CAN3TxFifo);
    FIFO_INIT(CAN1RxFifo);
    FIFO_INIT(CAN2RxFifo);
    FIFO_INIT(CAN3RxFifo);
}

/*****************************************************************************
**��������:	  Appliction_CanData_Init
**��������:	  ��can����ת���ɱ�׼����׼������
**��ڲ���:			
**���ز���:
******************************************************************************/
void Appliction_CanData_Init(CAN_MSG_Type * LPCTxMsg,uint8_t *buf,uint8_t dlc,uint32_t id,uint32_t ID_type)
{
	uint8_t i;
	LPCTxMsg->len = dlc;  //����
	if(ID_type == CAN_ID_STD)
	{
		LPCTxMsg->format = STD_ID_FORMAT;  //��׼֡
		LPCTxMsg->id = (id & 0x7fful);
	}
	else
	{
		LPCTxMsg->format= EXT_ID_FORMAT;  //��չ֡
		LPCTxMsg->id = (id & 0x1ffffffful);
	}

	LPCTxMsg->type = DATA_FRAME;	      //����֡

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
**��������:	  ADD_data_to_CANxTxFifo
**��������:	  ������ݵ�CANx���ͻ�����
**��ڲ���:			
**���ز���:
*******************************************************************************/
void ADD_data_to_CANxTxFifo(uint8_t CAN_N, CAN_MSG_Type TxMessage)
{	
    switch(CAN_N)
    {
        case 1://���浽CAN1TxFifo
            FIFO_PUT_ONE(CAN1TxFifo, TxMessage);
            break;
        case 2://���浽CAN2TxFifo
            FIFO_PUT_ONE(CAN2TxFifo, TxMessage);
            break;
        case 3://���浽CAN3TxFifo
            FIFO_PUT_ONE(CAN3TxFifo, TxMessage);
            break;
        default:
            break;
    }		
}

/*****************************************************************************
**��������:	  Appliction_Can_Transmit
**��������:	  ����can����
**��ڲ���:	   
**���ز���:
******************************************************************************/
void Appliction_Can_Transmit(LPC_CAN_TypeDef *CANx,CAN_MSG_Type *CAN_Msg)
{
//    while(CAN_GetCTRLStatus(CANx,CANCTRL_GLOBAL_STS)&(1<<5));//�ȴ�����������	

	NVIC_DisableIRQ(CAN_IRQn);
	CAN_SendMsg(CANx,CAN_Msg);//��������
	NVIC_EnableIRQ(CAN_IRQn);
}


/*****************************************************************************
**��������:	  Can_Transmit
**��������:	  ����CAN��Ϣ
**��ڲ���:	  ����CAN���ݵĽṹ��
**���ز���:  

extern Bool ACCSecondFlag;
extern Bool StandbyFlag;

******************************************************************************/
void Can_Transmit(uint8_t CAN_N, CAN_MSG_Type * CAN_MSG)
{
	static uint32_t IntervalTime;
	while(1)//���η���CAN���ݵļ��ʱ��Ϊ2ms
    {
        if(ReadUserTimer(&IntervalTime)>2)
        break;
    }
    ResetUserTimer(&IntervalTime);
   
//    switch(CAN_N)
//    {
//        case 1://ͨ��1����
//            Appliction_Can_Transmit(LPC_CAN1,CAN_MSG);
//            break;
//        case 2://ͨ��2����
//            Appliction_Can_Transmit(LPC_CAN2,CAN_MSG);
//            break;
//        case 3://ͨ��3����
//            MCP2515_CAN_SendMsg(CAN_MSG);
//            break;
//        default:
//            break;
//    }		
    
    if(StandbyFlag==FALSE) //������
    {
        if(ACCSecondFlag==TRUE) //ACC����
        {
            switch(CAN_N)
            {   
                case CAN_1://ͨ��1����
                    Appliction_Can_Transmit(LPC_CAN1,CAN_MSG);
                    break;
                case CAN_2://ͨ��2����
                    Appliction_Can_Transmit(LPC_CAN2,CAN_MSG);
                    break;
                case CAN_3://ͨ��3����
                    MCP2515_CAN_SendMsg(CAN_MSG);
                    break;
                default:
                    break;
            }		
        }
    }
}


/*******************************************************************************
**��������:	  Send_CANxTxFifo
**��������:	  ��CANxTxFifo��ȡ�����ݣ�����
**��ڲ���:			
**���ز���:
*******************************************************************************/
void Send_CANxTxFifo_data(uint8_t CAN_N)
{
	CAN_MSG_Type CAN_MSG;
    switch(CAN_N)
    {
        case CAN_1://����CAN1TxFifo�е�����
            if(FIFO_GET_ONE(CAN1TxFifo, &CAN_MSG)) 
            {
                Can_Transmit(CAN_1, &CAN_MSG);
            }
            break;
        case CAN_2://����CAN2TxFifo�е�����
            if(FIFO_GET_ONE(CAN2TxFifo, &CAN_MSG)) 
            {
                Can_Transmit(CAN_2, &CAN_MSG);
            }
            break;
        case CAN_3://����CAN3TxFifo�е�����
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
**��������:	  CAN_IRQHandler
**��������:	  �жϴ�����
**��ڲ���:			
**���ز���:
******************************************************************************/
void CAN_IRQHandler (void)
{				
	uint8_t CAN1IntStatus;
	uint8_t CAN2IntStatus;
    CAN_MSG_Type CAN1RXMsg;
    CAN_MSG_Type CAN2RXMsg;
    
	CAN1IntStatus = CAN_IntGetStatus(LPC_CAN1);
	CAN2IntStatus = CAN_IntGetStatus(LPC_CAN2);
    
	if((CAN1IntStatus>>0)&0x01) //bit0: �����ж�
	{
		CAN_ReceiveMsg(LPC_CAN1,&CAN1RXMsg);
    }
	if((CAN2IntStatus>>0)&0x01)
	{
		CAN_ReceiveMsg(LPC_CAN2,&CAN2RXMsg);
    }
}
