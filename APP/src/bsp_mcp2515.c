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
* 函数名称：SPI_WriteAddOneData
* 函数功能：写MCP2515一个寄存器的数据
* 入口参数：
* 出口参数：寄存器数据
*********************************************************************************************************/
void SPI_WriteAddOneData( uint8_t addr, uint8_t data)
{  
    SPI_CS_LOW;//CS低电平选中
	
	SPI_SendData(LPC_SPI, MCP2515_Write);//发送写指令
	SPI_SPIF_WAIT;//等待传输完成
	
	SPI_SendData(LPC_SPI, addr);//写入写的地址
	SPI_SPIF_WAIT;//等待传输完成
	
	SPI_SendData(LPC_SPI, data);//写入数据
	SPI_SPIF_WAIT;//等待传输完成
	
	SPI_CS_HI;//CS高电平拉高
			
}
/*********************************************************************************************************
* 函数名称：SPI_ReadAddOneData
* 函数功能：读取MCP2515一个寄存器的数据
* 入口参数：
* 出口参数：寄存器数据
*********************************************************************************************************/
uint8_t SPI_ReadAddOneData( uint8_t addr)
{  
			
	SPI_CS_LOW;//CS低电平选中
	
	SPI_SendData(LPC_SPI, MCP2515_Read);//发送读指令
	SPI_SPIF_WAIT;//等待传输完成
	
	SPI_SendData(LPC_SPI, addr);//写入读取地址
	SPI_SPIF_WAIT;//等待传输完成
	
	SPI_SendData(LPC_SPI, 0xff);//发送无效指令	
	SPI_SPIF_WAIT;//等待传输完成
    
	SPI_CS_HI;//CS高电平拉高
	SPI_ReceiveData(LPC_SPI);
    
	return SPI_ReceiveData(LPC_SPI);//拿出接收的指令
}
/*********************************************************************************************************
* 函数名称：SPI_Port_Init
* 函数功能：SPI_MCP2515端口初始化
* 入口参数：
* 出口参数：
*********************************************************************************************************/
void Port_Init()
{
	/*设置SPI功能引脚 初始化CANREST，SPI_PIN_SSEL端口为输出*/
	set_PinFunc(SPI_PORT, SPI_PIN_SCK, SPI_PIN_func);//SPI_SCK
	set_PinFunc(SPI_PORT, SPI_PIN_MISO, SPI_PIN_func);//SPI_MISO
	set_PinFunc(SPI_PORT, SPI_PIN_MOSI, SPI_PIN_func);//SPI_MOSI
	GPIO_SetDir(MCP2515_CAN_INTER_POST, MCP2515_CAN_INTER_MASK, GPIO_Input);//MCP2515_CAN_INIT
	GPIO_SetDir(SPI_PORT, SPI_PIN_SSEL_MASK, GPIO_Output);	//SPI_SSEL
	GPIO_SetDir(MCP2515_REST_PORT, MCP2515_REST_MASK, GPIO_Output);//MCP2515_REST
	/************************************************************/
	
}
/*********************************************************************************************************
* 函数名称：SPI_Init
* 函数功能：SPI初始化
* 入口参数：
* 出口参数：
*********************************************************************************************************/
void SPI_INit()
{
	SPI_CFG_Type SPI_ConfigStruct;
	SPI_ConfigStruct.Databit=SPI_DATABIT_8;//8位数据传输
	SPI_ConfigStruct.CPHA=SPI_CPHA_FIRST;/* second clock edge */
	SPI_ConfigStruct.CPOL=SPI_CPOL_HI;//高电平
	SPI_ConfigStruct.Mode=SPI_MASTER_MODE;//主模式
	SPI_ConfigStruct.DataOrder=SPI_DATA_MSB_FIRST;//: MSB first
    SPI_ConfigStruct.ClockRate =    2000000;	
	SPI_Init(LPC_SPI, &SPI_ConfigStruct);	//
	
}
/*********************************************************************************************************
* 函数名称：MCP2515_Init
* 函数功能：MCP2515初始化
* 入口参数：
* 出口参数：
*********************************************************************************************************/

void MCP2515_Init()
{
	unsigned int i;	
	MCP2515_REST;		    //MCP2515上电复位
	for(i=0;i<0xfffff;i++)  //复位延时
				;
	/********配置比特率为500K  时钟16MHZ/1   16Tq   500k  ************/
	SPI_WriteAddOneData(MCP2515_CNF1,0);		//SJW=1 Tq=1/4Mhz
	SPI_WriteAddOneData(MCP2515_CNF2,0xb1);	//BTLMODE=1 SAM=0 PHSEG1=6+1 PRSEG=1+1
	SPI_WriteAddOneData(MCP2515_CNF3,0x5);//PHSEG2=5+1
	/**************************************************************/
	SPI_WriteAddOneData(MCP2515_CAN_CTRL,0x1<<3);//正常工作模式
	SPI_WriteAddOneData(MCP2515_TXB1_CTRL,0x3);//设置TXB1发送优先级最高
		/********配置接收寄存器，MCP2515中断使能  ************/
	SPI_WriteAddOneData(MCP2515_RXB0CTRL,0X64);//关闭滤波器 滚存使能
	SPI_WriteAddOneData(MCP2515_RXB1CTRL,0x60);//关闭滤波器
	SPI_WriteAddOneData(MCP2515_CANINTE,0x3);		//接收器中断使能
	NVIC_SetPriority(EINT3_IRQn, 2);       
	NVIC_EnableIRQ(EINT3_IRQn);
	LPC_GPIOINT->IO2IntEnF = MCP2515_CAN_INTER_MASK;                                     /* P2.10下降沿中断使能          */
    LPC_GPIOINT->IO2IntClr |= MCP2515_CAN_INTER_MASK;
}

void Appliction_SPI_To_MCP2515_Init(void)
{
	Port_Init();//端口引脚初始化
	SPI_INit();	//SPI初始化
	MCP2515_Init();//MCP2515初始化
}
/*********************************************************************************************************
* 函数名称：MCP2515_CAN_TXB1CTRLStatus
* 函数功能：获取TXB1的CTRL状态
* 入口参数：
* 出口参数：
*********************************************************************************************************/
uint8_t MCP2515_CAN_TXB1CTRLStatus(uint8_t MASK)
{
	return (SPI_ReadAddOneData(MCP2515_TXB1_CTRL)&MASK);
	
}
/*********************************************************************************************************
* 函数名称：MCP2515_CAN_TXB2CTRLStatus
* 函数功能：获取TXB2的CTRL状态
* 入口参数：
* 出口参数：
*********************************************************************************************************/
uint8_t MCP2515_CAN_TXB2CTRLStatus(uint8_t MASK)
{
	return (SPI_ReadAddOneData(MCP2515_TXB2_CTRL)&MASK);
	
}

/*********************************************************************************************************
* 函数名称：MCP2515_CAN_TXB3CTRLStatus
* 函数功能：获取TXB2的CTRL状态
* 入口参数：
* 出口参数：
*********************************************************************************************************/
uint8_t MCP2515_CAN_TXB3CTRLStatus(uint8_t MASK)
{
	return (SPI_ReadAddOneData(MCP2515_TXB3_CTRL)&MASK);
	
}
/*********************************************************************************************************
* 函数名称：MCP2515_CAN_SendMsg
* 函数功能：发送CAN数据
* 入口参数：
* 出口参数：
*********************************************************************************************************/
uint8_t MCP2515_CAN_SendMsg (CAN_MSG_Type * CAN_Msg)
{
	uint8_t tmp;
	
	#if(CANSEND_Select == CANSENDON)
	/* 如果发送器忙碌等待 */
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
	/**************************发送器1发送CAN数据 *****************************************/
	
	if (!MCP2515_CAN_TXB1CTRLStatus(MCP2515_TXB_CTRL_TXREQ))
	{
		SPI_CS_LOW;//CS低电平选中
		SPI_SPIF_SEBDANDWAIT(0x40);//装载指令	
		if(STD_ID_FORMAT == CAN_Msg->format)//如果是11位标准帧
		{
			tmp = ((CAN_Msg->id &0x7f8)>>3);
			SPI_SPIF_SEBDANDWAIT(tmp);					
			tmp = ((CAN_Msg->id &0x7)<<5);
			SPI_SPIF_SEBDANDWAIT(tmp);
			SPI_SPIF_SEBDANDWAIT(0x0);	
			SPI_SPIF_SEBDANDWAIT(0x0);		
		}
		else
		{//扩展帧
			tmp = ((CAN_Msg->id &0x1fe00000)>>21);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &(0x7<<18))>>13);
			tmp |= ((CAN_Msg->id &(0x3<<16))>>16)|(0x1<<3);//扩展帧
			SPI_SPIF_SEBDANDWAIT(tmp);		
			tmp = ((CAN_Msg->id &0xff00)>>8);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &0xff));
			SPI_SPIF_SEBDANDWAIT(tmp);				
		}
		tmp = (CAN_Msg->len &0xf);
		if(REMOTE_FRAME==CAN_Msg->type)//远程帧
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
		SPI_CS_HI;//CS高电平拉高
		SPI_WriteAddOneData(MCP2515_TXB1_CTRL,MCP2515_TXB_send);
	
		return SUCCESS;
	}
		/**************************************************************************************/
		/**************************发送器2发送CAN数据 *****************************************/
	else if(!MCP2515_CAN_TXB2CTRLStatus(MCP2515_TXB_CTRL_TXREQ))
	{
		
		SPI_CS_LOW;//CS低电平选中
		SPI_SPIF_SEBDANDWAIT(0x42);//装载指令	
		if(STD_ID_FORMAT == CAN_Msg->format)//如果是11位标准帧
		{
			tmp = ((CAN_Msg->id &0x7f8)>>3);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &0x7)<<5);
			SPI_SPIF_SEBDANDWAIT(tmp);
			SPI_SPIF_SEBDANDWAIT(0x0);	
			SPI_SPIF_SEBDANDWAIT(0x0);		
		}
		else
		{//扩展帧
			tmp = ((CAN_Msg->id &0x1fe00000)>>21);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &(0x7<<18))>>13);
			tmp |= ((CAN_Msg->id &(0x3<<16))>>16)|(0x1<<3);//扩展帧
			SPI_SPIF_SEBDANDWAIT(tmp);		
			tmp = ((CAN_Msg->id &0xff00)>>8);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &0xff));
			SPI_SPIF_SEBDANDWAIT(tmp);				
		}
		tmp = (CAN_Msg->len &0xf);
		if(REMOTE_FRAME==CAN_Msg->type)//远程帧
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
		SPI_CS_HI;//CS高电平拉高
		SPI_WriteAddOneData(MCP2515_TXB2_CTRL,MCP2515_TXB_send);
			
		 return SUCCESS;
	}	
	/**************************发送器3发送CAN数据 *****************************************/
	else if (!MCP2515_CAN_TXB3CTRLStatus(MCP2515_TXB_CTRL_TXREQ))
	{
		SPI_CS_LOW;//CS低电平选中
		SPI_SPIF_SEBDANDWAIT(0x44);//装载指令	
		if(STD_ID_FORMAT == CAN_Msg->format)//如果是11位标准帧
		{
			tmp = ((CAN_Msg->id &0x7f8)>>3);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &0x7)<<5);
			SPI_SPIF_SEBDANDWAIT(tmp);
			SPI_SPIF_SEBDANDWAIT(0x0);	
			SPI_SPIF_SEBDANDWAIT(0x0);		
		}
		else
		{//扩展帧
			tmp = ((CAN_Msg->id &0x1fe00000)>>21);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &(0x7<<18))>>13);
			tmp |= ((CAN_Msg->id &(0x3<<16))>>16)|(0x1<<3);//扩展帧
			SPI_SPIF_SEBDANDWAIT(tmp);		
			tmp = ((CAN_Msg->id &0xff00)>>8);
			SPI_SPIF_SEBDANDWAIT(tmp);
			tmp = ((CAN_Msg->id &0xff));
			SPI_SPIF_SEBDANDWAIT(tmp);				
		}
		tmp = (CAN_Msg->len &0xf);
		if(REMOTE_FRAME==CAN_Msg->type)//远程帧
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
		SPI_CS_HI;//CS高电平拉高
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
* 函数名称：SPI_CAN_IntHandler
* 函数功能：MCP2515中断处理函数    
* 入口参数：
* 出口参数：
*********************************************************************************************************/

void SPI_CAN_IntHandler(CAN_MSG_Type *LPCTxMsg)
{
	unsigned char tmp_intf,tmp;

//	if(MCP2515ISINTRT)//是否产生中断MCP2515
	if(MCP2515ISINTRT)
	{				
		tmp_intf=SPI_ReadAddOneData(0x2c);//获取中断标志状态
		//printf("");
		//************************接收缓冲寄存器0********************************************//
		if(tmp_intf&0x1)							
		{	
		
			tmp = SPI_ReadAddOneData(MCP2515_RXB0DLC);	//获取报数据长度
			LPCTxMsg->len = tmp &0xf;
		
			tmp = SPI_ReadAddOneData(MCP2515_RXB0SIDL);//取出SIDL
			if((tmp&(0x1<<4)))													//如果是远程帧
				LPCTxMsg->type=REMOTE_FRAME;							
			
			if((tmp&(0x1<<3)))													//如果是扩展帧
			{
				LPCTxMsg->format=EXT_ID_FORMAT;						//扩展帧标志
			
			tmp = SPI_ReadAddOneData(MCP2515_RXB0EID0);//扩展帧低8位
				LPCTxMsg->id=tmp;
			tmp = SPI_ReadAddOneData(MCP2515_RXB0EID8);//扩展帧7~15位
				LPCTxMsg->id|=(tmp<<8);
			tmp = SPI_ReadAddOneData(MCP2515_RXB0SIDL);	//扩展帧16~20位
				LPCTxMsg->id|=((tmp&0x3)<<16);
				LPCTxMsg->id|=((tmp&0xc0)<<13);
			tmp = SPI_ReadAddOneData(MCP2515_RXB0SIDH);	//扩展帧21~20位
				LPCTxMsg->id|=((tmp)<<21);			
			}
			else																			//标准帧
			{			
					tmp = SPI_ReadAddOneData(MCP2515_RXB0SIDL);	//标准帧低3位
						LPCTxMsg->id = (tmp&0xe0)>>5;
					tmp = SPI_ReadAddOneData(MCP2515_RXB0SIDH);//	//标准帧高8位
						LPCTxMsg->id |= (tmp)<<3;				
			}
			/******************			取出数据*************************/
			LPCTxMsg->dataA[0] =  SPI_ReadAddOneData(MCP2515_RXB0DM0);
			LPCTxMsg->dataA[1] =  SPI_ReadAddOneData(MCP2515_RXB0DM1);			
			LPCTxMsg->dataA[2] =  SPI_ReadAddOneData(MCP2515_RXB0DM2);
			LPCTxMsg->dataA[3] =  SPI_ReadAddOneData(MCP2515_RXB0DM3);
			LPCTxMsg->dataB[0] =  SPI_ReadAddOneData(MCP2515_RXB0DM4);
			LPCTxMsg->dataB[1] =  SPI_ReadAddOneData(MCP2515_RXB0DM5);
			LPCTxMsg->dataB[2] =  SPI_ReadAddOneData(MCP2515_RXB0DM6);
			LPCTxMsg->dataB[3] =  SPI_ReadAddOneData(MCP2515_RXB0DM7);
			
			
			tmp_intf &=~(0x1);																	//清除MCP2515中断标志
			SPI_WriteAddOneData(MCP2515_CANINTF,tmp_intf);
		//	SPI_WriteAddOneData( 0x2c, 0)	;

		}
		else
		{
		//************************接收缓冲寄存器1********************************************//
            if(tmp_intf&0x2)																			//接收缓冲寄存器1
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
* 函数名称：EINT3_IRQHandler
* 函数功能：外部中断函数
* 入口参数：
* 出口参数：寄存器数据
*********************************************************************************************************/
void EINT3_IRQHandler (void)
{
	CAN_MSG_Type RxBuff;
	if(LPC_GPIOINT->IO2IntStatF&(0x1<<4))
	{
		SPI_CAN_IntHandler (&RxBuff);//拿出数据
		FIFO_PUT_ONE(CAN3RxFifo,RxBuff);										
        LPC_GPIOINT->IO2IntClr |= MCP2515_CAN_INTER_MASK;//清除外部中断
	}
}

