#include "bsp_mcp2515.h"


#define MCP2515_REST 		GPIO_ClearValue(MCP2515_REST_PORT, MCP2515_REST_MASK);	GPIO_SetValue(MCP2515_REST_PORT,MCP2515_REST_MASK)
#define SPI_CS_LOW          GPIO_ClearValue(SPI_PORT, SPI_PIN_SSEL_MASK)
#define SPI_CS_HI 			GPIO_SetValue(SPI_PORT,SPI_PIN_SSEL_MASK);
#define SPI_SPIF_WAIT 		while(!SPI_CheckStatus(SPI_GetStatus(LPC_SPI), SPI_STAT_SPIF))
#define MCP2515ISINTRT      (((GPIO_ReadValue(MCP2515_CAN_INTER_POST))&(0x1<<4))==0)	
#define SPI_SPIF_SEBDANDWAIT(x)     SPI_SendData(LPC_SPI, x);SPI_SPIF_WAIT	
extern struct msg
{
	CAN_MSG_Type CAN_Msg;
	unsigned char can;
	unsigned char flag;
}rxbuff[20];
CAN_MSG_Type ALLCAN_RxMsg[100];
unsigned char Rxflag[100];
/*********************************************************************************************************
* �������ƣ�SPI_WriteAddOneData
* �������ܣ�дMCP2515һ���Ĵ���������
* ��ڲ�����
* ���ڲ������Ĵ�������
*********************************************************************************************************/
void SPI_WriteAddOneData( uint8_t addr, uint8_t data)
{  
    SPI_CS_LOW;//CS�͵�ƽѡ��
	
	SPI_SendData(LPC_SPI, MCP2515_Write);//����дָ��
	SPI_SPIF_WAIT;//�ȴ��������
	
	SPI_SendData(LPC_SPI, addr);//д��д�ĵ�ַ
	SPI_SPIF_WAIT;//�ȴ��������
	
	SPI_SendData(LPC_SPI, data);//д������
	SPI_SPIF_WAIT;//�ȴ��������
	
	SPI_CS_HI;//CS�ߵ�ƽ����
			
}
/*********************************************************************************************************
* �������ƣ�SPI_ReadAddOneData
* �������ܣ���ȡMCP2515һ���Ĵ���������
* ��ڲ�����
* ���ڲ������Ĵ�������
*********************************************************************************************************/
uint8_t SPI_ReadAddOneData( uint8_t addr)
{  
			
	SPI_CS_LOW;//CS�͵�ƽѡ��
	
	SPI_SendData(LPC_SPI, MCP2515_Read);//���Ͷ�ָ��
	SPI_SPIF_WAIT;//�ȴ��������
	
	SPI_SendData(LPC_SPI, addr);//д���ȡ��ַ
	SPI_SPIF_WAIT;//�ȴ��������
	
	SPI_SendData(LPC_SPI, 0xff);//������Чָ��	
	SPI_SPIF_WAIT;//�ȴ��������
    
	SPI_CS_HI;//CS�ߵ�ƽ����
	SPI_ReceiveData(LPC_SPI);
    
	return SPI_ReceiveData(LPC_SPI);//�ó����յ�ָ��
}
/*********************************************************************************************************
* �������ƣ�SPI_Port_Init
* �������ܣ�SPI_MCP2515�˿ڳ�ʼ��
* ��ڲ�����
* ���ڲ�����
*********************************************************************************************************/
void Port_Init()
{
	/*����SPI�������� ��ʼ��CANREST��SPI_PIN_SSEL�˿�Ϊ���*/
	set_PinFunc(SPI_PORT, SPI_PIN_SCK, SPI_PIN_func);//SPI_SCK
	set_PinFunc(SPI_PORT, SPI_PIN_MISO, SPI_PIN_func);//SPI_MISO
	set_PinFunc(SPI_PORT, SPI_PIN_MOSI, SPI_PIN_func);//SPI_MOSI
	GPIO_SetDir(MCP2515_CAN_INTER_POST, MCP2515_CAN_INTER_MASK, GPIO_Input);//MCP2515_CAN_INIT
	GPIO_SetDir(SPI_PORT, SPI_PIN_SSEL_MASK, GPIO_Output);	//SPI_SSEL
	GPIO_SetDir(MCP2515_REST_PORT, MCP2515_REST_MASK, GPIO_Output);//MCP2515_REST
	/************************************************************/
	
}
/*********************************************************************************************************
* �������ƣ�SPI_Init
* �������ܣ�SPI��ʼ��
* ��ڲ�����
* ���ڲ�����
*********************************************************************************************************/
void SPI_INit()
{
	SPI_CFG_Type SPI_ConfigStruct;
	SPI_ConfigStruct.Databit=SPI_DATABIT_8;//8λ���ݴ���
	SPI_ConfigStruct.CPHA=SPI_CPHA_FIRST;/* second clock edge */
	SPI_ConfigStruct.CPOL=SPI_CPOL_HI;//�ߵ�ƽ
	SPI_ConfigStruct.Mode=SPI_MASTER_MODE;//��ģʽ
	SPI_ConfigStruct.DataOrder=SPI_DATA_MSB_FIRST;//: MSB first
    SPI_ConfigStruct.ClockRate =    2000000;	
	SPI_Init(LPC_SPI, &SPI_ConfigStruct);	//
	
}
/*********************************************************************************************************
* �������ƣ�MCP2515_Init
* �������ܣ�MCP2515��ʼ��
* ��ڲ�����
* ���ڲ�����
*********************************************************************************************************/

void MCP2515_Init()
{
	unsigned int i;	
	MCP2515_REST;		    //MCP2515�ϵ縴λ
	for(i=0;i<0xfffff;i++)  //��λ��ʱ
				;
	/********���ñ�����Ϊ500K  ʱ��16MHZ/1   16Tq   500k  ************/
	SPI_WriteAddOneData(MCP2515_CNF1,0);		//SJW=1 Tq=1/4Mhz
	SPI_WriteAddOneData(MCP2515_CNF2,0xb1);	//BTLMODE=1 SAM=0 PHSEG1=6+1 PRSEG=1+1
	SPI_WriteAddOneData(MCP2515_CNF3,0x5);//PHSEG2=5+1
	/**************************************************************/
	SPI_WriteAddOneData(MCP2515_CAN_CTRL,0x1<<3);//��������ģʽ
	SPI_WriteAddOneData(MCP2515_TXB1_CTRL,0x3);//����TXB1�������ȼ����
		/********���ý��ռĴ�����MCP2515�ж�ʹ��  ************/
	SPI_WriteAddOneData(MCP2515_RXB0CTRL,0X64);//�ر��˲��� ����ʹ��
	SPI_WriteAddOneData(MCP2515_RXB1CTRL,0x60);//�ر��˲���
	SPI_WriteAddOneData(MCP2515_CANINTE,0x3);		//�������ж�ʹ��
	NVIC_SetPriority(EINT3_IRQn, 2);       
	NVIC_EnableIRQ(EINT3_IRQn);
	LPC_GPIOINT->IO2IntEnF = MCP2515_CAN_INTER_MASK;                                     /* P2.10�½����ж�ʹ��          */
    LPC_GPIOINT->IO2IntClr |= MCP2515_CAN_INTER_MASK;
}

void Appliction_SPI_To_MCP2515_Init(void)
{
	Port_Init();//�˿����ų�ʼ��
	SPI_INit();	//SPI��ʼ��
	MCP2515_Init();//MCP2515��ʼ��
}
/*********************************************************************************************************
* �������ƣ�MCP2515_CAN_TXB1CTRLStatus
* �������ܣ���ȡTXB1��CTRL״̬
* ��ڲ�����
* ���ڲ�����
*********************************************************************************************************/
uint8_t MCP2515_CAN_TXB1CTRLStatus(uint8_t MASK)
{
	return (SPI_ReadAddOneData(MCP2515_TXB1_CTRL)&MASK);
	
}
/*********************************************************************************************************
* �������ƣ�MCP2515_CAN_TXB2CTRLStatus
* �������ܣ���ȡTXB2��CTRL״̬
* ��ڲ�����
* ���ڲ�����
*********************************************************************************************************/
uint8_t MCP2515_CAN_TXB2CTRLStatus(uint8_t MASK)
{
	return (SPI_ReadAddOneData(MCP2515_TXB2_CTRL)&MASK);
	
}

/*********************************************************************************************************
* �������ƣ�MCP2515_CAN_TXB3CTRLStatus
* �������ܣ���ȡTXB2��CTRL״̬
* ��ڲ�����
* ���ڲ�����
*********************************************************************************************************/
uint8_t MCP2515_CAN_TXB3CTRLStatus(uint8_t MASK)
{
	return (SPI_ReadAddOneData(MCP2515_TXB3_CTRL)&MASK);
	
}
/*********************************************************************************************************
* �������ƣ�MCP2515_CAN_SendMsg
* �������ܣ�����CAN����
* ��ڲ�����
* ���ڲ�����
*********************************************************************************************************/
uint8_t MCP2515_CAN_SendMsg (CAN_MSG_Type * CAN_Msg)
{
	uint8_t tmp;
	
	#if(CANSEND_Select == CANSENDON)
	/* ���������æµ�ȴ� */
//	printf("%x\r\n",MCP2515_CAN_TXB2CTRLStatus(MCP2515_TXB_CTRL_TXREQ));
while(1)
{
	if (!MCP2515_CAN_TXB1CTRLStatus(MCP2515_TXB_CTRL_TXREQ))
		break;
	if (!MCP2515_CAN_TXB2CTRLStatus(MCP2515_TXB_CTRL_TXREQ))
		break;		
	if (!MCP2515_CAN_TXB3CTRLStatus(MCP2515_TXB_CTRL_TXREQ))
		break;	
}
	/**************************������1����CAN���� *****************************************/
	
	if (!MCP2515_CAN_TXB1CTRLStatus(MCP2515_TXB_CTRL_TXREQ))
	{
		SPI_CS_LOW;//CS�͵�ƽѡ��
		SPI_SPIF_SEBDANDWAIT(0x40);//װ��ָ��	
		if(STD_ID_FORMAT == CAN_Msg->format)//�����11λ��׼֡
		{
			tmp = ((CAN_Msg->id &0x7f8)>>3);
			SPI_SPIF_SEBDANDWAIT(tmp);					
			tmp = ((CAN_Msg->id &0x7)<<5);
			SPI_SPIF_SEBDANDWAIT(tmp);
			SPI_SPIF_SEBDANDWAIT(0x0);	
			SPI_SPIF_SEBDANDWAIT(0x0);		
		}
		else
		{//��չ֡
			tmp = ((CAN_Msg->id &0x1fe00000)>>21);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &(0x7<<18))>>13);
			tmp |= ((CAN_Msg->id &(0x3<<16))>>16)|(0x1<<3);//��չ֡
			SPI_SPIF_SEBDANDWAIT(tmp);		
			tmp = ((CAN_Msg->id &0xff00)>>8);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &0xff));
			SPI_SPIF_SEBDANDWAIT(tmp);				
		}
		tmp = (CAN_Msg->len &0xf);
		if(REMOTE_FRAME==CAN_Msg->type)//Զ��֡
			tmp|=(0x1<<6);
		SPI_SPIF_SEBDANDWAIT(tmp);		
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[0]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[1]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[2]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[3]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[0]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[1]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[2]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[3]);
		SPI_CS_HI;//CS�ߵ�ƽ����
		SPI_WriteAddOneData(MCP2515_TXB1_CTRL,MCP2515_TXB_send);
	
		return SUCCESS;
	}
		/**************************************************************************************/
		/**************************������2����CAN���� *****************************************/
	else if(!MCP2515_CAN_TXB2CTRLStatus(MCP2515_TXB_CTRL_TXREQ))
	{
		
		SPI_CS_LOW;//CS�͵�ƽѡ��
		SPI_SPIF_SEBDANDWAIT(0x42);//װ��ָ��	
		if(STD_ID_FORMAT == CAN_Msg->format)//�����11λ��׼֡
		{
			tmp = ((CAN_Msg->id &0x7f8)>>3);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &0x7)<<5);
			SPI_SPIF_SEBDANDWAIT(tmp);
			SPI_SPIF_SEBDANDWAIT(0x0);	
			SPI_SPIF_SEBDANDWAIT(0x0);		
		}
		else
		{//��չ֡
			tmp = ((CAN_Msg->id &0x1fe00000)>>21);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &(0x7<<18))>>13);
			tmp |= ((CAN_Msg->id &(0x3<<16))>>16)|(0x1<<3);//��չ֡
			SPI_SPIF_SEBDANDWAIT(tmp);		
			tmp = ((CAN_Msg->id &0xff00)>>8);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &0xff));
			SPI_SPIF_SEBDANDWAIT(tmp);				
		}
		tmp = (CAN_Msg->len &0xf);
		if(REMOTE_FRAME==CAN_Msg->type)//Զ��֡
			tmp|=(0x1<<6);
		SPI_SPIF_SEBDANDWAIT(tmp);		
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[0]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[1]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[2]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[3]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[0]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[1]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[2]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[3]);
		SPI_CS_HI;//CS�ߵ�ƽ����
		SPI_WriteAddOneData(MCP2515_TXB2_CTRL,MCP2515_TXB_send);
			
		 return SUCCESS;
	}	
	/**************************������3����CAN���� *****************************************/
	else if (!MCP2515_CAN_TXB3CTRLStatus(MCP2515_TXB_CTRL_TXREQ))
	{
		SPI_CS_LOW;//CS�͵�ƽѡ��
		SPI_SPIF_SEBDANDWAIT(0x44);//װ��ָ��	
		if(STD_ID_FORMAT == CAN_Msg->format)//�����11λ��׼֡
		{
			tmp = ((CAN_Msg->id &0x7f8)>>3);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &0x7)<<5);
			SPI_SPIF_SEBDANDWAIT(tmp);
			SPI_SPIF_SEBDANDWAIT(0x0);	
			SPI_SPIF_SEBDANDWAIT(0x0);		
		}
		else
		{//��չ֡
			tmp = ((CAN_Msg->id &0x1fe00000)>>21);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &(0x7<<18))>>13);
			tmp |= ((CAN_Msg->id &(0x3<<16))>>16)|(0x1<<3);//��չ֡
			SPI_SPIF_SEBDANDWAIT(tmp);		
			tmp = ((CAN_Msg->id &0xff00)>>8);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &0xff));
			SPI_SPIF_SEBDANDWAIT(tmp);				
		}
		tmp = (CAN_Msg->len &0xf);
		if(REMOTE_FRAME==CAN_Msg->type)//Զ��֡
            tmp|=(0x1<<6);
		SPI_SPIF_SEBDANDWAIT(tmp);		
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[0]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[1]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[2]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataA[3]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[0]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[1]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[2]);
		SPI_SPIF_SEBDANDWAIT(CAN_Msg->dataB[3]);
		SPI_CS_HI;//CS�ߵ�ƽ����
		SPI_WriteAddOneData(MCP2515_TXB3_CTRL,MCP2515_TXB_send);
			
		 return SUCCESS;
	}
	/**************************************************************************************/
	else
	{
		return ERROR;
	}
	
	
#endif
	
}	
	/*********************************************************************************************************
* �������ƣ�SPI_CAN_IntHandler
* �������ܣ�MCP2515�жϴ�����    
* ��ڲ�����
* ���ڲ�����
*********************************************************************************************************/

void SPI_CAN_IntHandler(CAN_MSG_Type *LPCTxMsg)
{
	unsigned char tmp_intf,tmp;

//	if(MCP2515ISINTRT)//�Ƿ�����ж�MCP2515
	if(MCP2515ISINTRT)
	{				
		tmp_intf=SPI_ReadAddOneData(0x2c);//��ȡ�жϱ�־״̬
		//printf("");
		//************************���ջ���Ĵ���0********************************************//
		if(tmp_intf&0x1)							
		{	
		
			tmp = SPI_ReadAddOneData(MCP2515_RXB0DLC);	//��ȡ�����ݳ���
			LPCTxMsg->len = tmp &0xf;
		
			tmp = SPI_ReadAddOneData(MCP2515_RXB0SIDL);//ȡ��SIDL
			if((tmp&(0x1<<4)))													//�����Զ��֡
				LPCTxMsg->type=REMOTE_FRAME;							
			
			if((tmp&(0x1<<3)))													//�������չ֡
			{
				LPCTxMsg->format=EXT_ID_FORMAT;						//��չ֡��־
			
			tmp = SPI_ReadAddOneData(MCP2515_RXB0EID0);//��չ֡��8λ
				LPCTxMsg->id=tmp;
			tmp = SPI_ReadAddOneData(MCP2515_RXB0EID8);//��չ֡7~15λ
				LPCTxMsg->id|=(tmp<<8);
			tmp = SPI_ReadAddOneData(MCP2515_RXB0SIDL);	//��չ֡16~20λ
				LPCTxMsg->id|=((tmp&0x3)<<16);
				LPCTxMsg->id|=((tmp&0xc0)<<13);
			tmp = SPI_ReadAddOneData(MCP2515_RXB0SIDH);	//��չ֡21~20λ
				LPCTxMsg->id|=((tmp)<<21);			
			}
			else																			//��׼֡
			{			
					tmp = SPI_ReadAddOneData(MCP2515_RXB0SIDL);	//��׼֡��3λ
						LPCTxMsg->id = (tmp&0xe0)>>5;
					tmp = SPI_ReadAddOneData(MCP2515_RXB0SIDH);//	//��׼֡��8λ
						LPCTxMsg->id |= (tmp)<<3;				
			}
			/******************			ȡ������*************************/
			LPCTxMsg->dataA[0] =  SPI_ReadAddOneData(MCP2515_RXB0DM0);
			LPCTxMsg->dataA[1] =  SPI_ReadAddOneData(MCP2515_RXB0DM1);			
			LPCTxMsg->dataA[2] =  SPI_ReadAddOneData(MCP2515_RXB0DM2);
			LPCTxMsg->dataA[3] =  SPI_ReadAddOneData(MCP2515_RXB0DM3);
			LPCTxMsg->dataB[0] =  SPI_ReadAddOneData(MCP2515_RXB0DM4);
			LPCTxMsg->dataB[1] =  SPI_ReadAddOneData(MCP2515_RXB0DM5);
			LPCTxMsg->dataB[2] =  SPI_ReadAddOneData(MCP2515_RXB0DM6);
			LPCTxMsg->dataB[3] =  SPI_ReadAddOneData(MCP2515_RXB0DM7);
			
			
			tmp_intf &=~(0x1);																	//���MCP2515�жϱ�־
			SPI_WriteAddOneData(MCP2515_CANINTF,tmp_intf);
		//	SPI_WriteAddOneData( 0x2c, 0)	;

		}
		else
		{
		//************************���ջ���Ĵ���1********************************************//
            if(tmp_intf&0x2)																			//���ջ���Ĵ���1
            {	
                tmp = SPI_ReadAddOneData(MCP2515_RXB1DLC);
                LPCTxMsg->len = tmp &0xf;
                
                tmp = SPI_ReadAddOneData(MCP2515_RXB1SIDL);
                if((tmp&(0x1<<4)))
                    LPCTxMsg->type=REMOTE_FRAME;
                
                if((tmp&(0x1<<3)))
                {
                    LPCTxMsg->format=EXT_ID_FORMAT;
                    
                tmp = SPI_ReadAddOneData(MCP2515_RXB1EID0);
                    LPCTxMsg->id=tmp;
                tmp = SPI_ReadAddOneData(MCP2515_RXB1EID8);
                    LPCTxMsg->id|=(tmp<<8);
                tmp = SPI_ReadAddOneData(MCP2515_RXB1SIDL);	
                    LPCTxMsg->id|=((tmp&0x3)<<16);
                    LPCTxMsg->id|=((tmp&0xc0)<<13);
                tmp = SPI_ReadAddOneData(MCP2515_RXB1SIDH);	
                    LPCTxMsg->id|=((tmp)<<21);			
                }
                else
                {				
                    tmp = SPI_ReadAddOneData(MCP2515_RXB1SIDL);
                        LPCTxMsg->id = (tmp&0x3);
                    tmp = SPI_ReadAddOneData(MCP2515_RXB1SIDH);
                        LPCTxMsg->id |= (tmp)<<3;				
                }
                LPCTxMsg->dataA[0] =  SPI_ReadAddOneData(MCP2515_RXB1DM0);
                LPCTxMsg->dataA[1] =  SPI_ReadAddOneData(MCP2515_RXB1DM1);			
                LPCTxMsg->dataA[2] =  SPI_ReadAddOneData(MCP2515_RXB1DM2);
                LPCTxMsg->dataA[3] =  SPI_ReadAddOneData(MCP2515_RXB1DM3);
                LPCTxMsg->dataB[0] =  SPI_ReadAddOneData(MCP2515_RXB1DM4);
                LPCTxMsg->dataB[1] =  SPI_ReadAddOneData(MCP2515_RXB1DM5);
                LPCTxMsg->dataB[2] =  SPI_ReadAddOneData(MCP2515_RXB1DM6);
                LPCTxMsg->dataB[3] =  SPI_ReadAddOneData(MCP2515_RXB1DM7);
                
                tmp_intf &=~(0x2);
                SPI_WriteAddOneData(MCP2515_CANINTF,tmp_intf);
            //	SPI_WriteAddOneData( 0x2c, 0)	;
            }		
        }
    }
}
/*********************************************************************************************************
* �������ƣ�EINT3_IRQHandler
* �������ܣ��ⲿ�жϺ���
* ��ڲ�����
* ���ڲ������Ĵ�������
*********************************************************************************************************/
void EINT3_IRQHandler (void)
{
	CAN_MSG_Type RxBuff;
	if(LPC_GPIOINT->IO2IntStatF&(0x1<<4))
	{
		SPI_CAN_IntHandler (&RxBuff);//�ó�����
		FIFO_PUT_ONE(CAN3RxFifo,RxBuff);										
        LPC_GPIOINT->IO2IntClr |= MCP2515_CAN_INTER_MASK;//����ⲿ�ж�
	}
}

