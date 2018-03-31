#include "DealRxProcessor.h"	

//u8 WinceCmdFrameStatus;
//tSYSTEM_CONFIG  currentRGBInfo, lastRGBInfo, clearRGBInfo;

#if 1 //调试ISP
void (*pdealTx)(void);
void (*pdealRx)(u8 data);
u8 dealStatus = 0;
///////
void ISP_pdealTx(void)
{
    if(0 == dealStatus) {pdealTx = ISP_Tx_SayHi_Order;}  
    if(1 == dealStatus) {pdealTx = ISP_Tx_Synchronized_Order;}  
    if(2 == dealStatus) {pdealTx = ISP_Tx_Oscillator_Order;}  
    if(3 == dealStatus) {pdealTx = NULL;} 
    
    pdealTx();        
}
void ISP_pdealRx(u8 data)
{
    if(0 == dealStatus) {pdealRx = ISP_Rx_SayHi_Response;} 
    if(1 == dealStatus) {pdealRx = ISP_Rx_Synchronized_Response;}  
    if(2 == dealStatus) {pdealRx = ISP_Rx_Oscillator_Response;}  
    if(3 == dealStatus) {pdealRx = NULL;}
  
    pdealRx(data);        
}
///////
void ISP_Tx_SayHi_Order(void) //发送同步字符'?'   
{            
    UART3_send_str("?\r\n"); 
} 
void ISP_Tx_Synchronized_Order(void)
{   
    UART3_send_str("Synchronized\r\n"); 
} 
void ISP_Tx_Oscillator_Order(void)
{   
    UART3_send_str("12000\r\n"); 
} 
///////
void ISP_Rx_SayHi_Response(u8 data)
{
	static u8 status=0;
	
	switch(status)
	{
		case 0:
		{
			if(data == 'S') {status = 1;}
			else {status = 0;}
			break;		
		}
		case 1:
		{
			if(data == 'y') {status = 2;}
			else {status = 0;}
			break;		
		}
		case 2:
		{
			if(data == 'n') {status = 3;}
			else {status = 0;}
			break;		
		}
		case 3:
		{
			if(data == 'c') {status = 4;}
			else {status = 0;}
			break;		
		}
		case 4:
		{
			if(data == 'h') {status = 5;}
			else {status = 0;}
			break;		
		}
 		case 5:
		{
			if(data == 'r') {status = 6;}
			else {status = 0;}
			break;		
		}
		case 6:
		{
			if(data == 'o') {status = 7;}
			else {status = 0;}
			break;		
		}
		case 7:
		{
			if(data == 'n') {status = 8;}
			else {status = 0;}
			break;		
		}
		case 8:
		{
			if(data == 'i') {status = 9;}
			else {status = 0;}
			break;		
		}
 		case 9:
		{
			if(data == 'z') {status = 10;}
			else {status = 0;}
			break;		
		}
 		case 10:
		{
			if(data == 'e') {status = 11;}
			else {status = 0;}
			break;		
		}
 		case 11:
		{
			if(data == 'd') {status = 12;}
			else {status = 0;}                           
			break;		
		}  
 		case 12:
		{
			if((data == '\n')||(data == '\r')) {dealStatus = 1; printf("\r\nRecevce 'Synchronized'");}
			else {} 
            status = 0;                
			break;		
		}         
		default:
		{
			status=0;
			break;
		}
	}
}						

void ISP_Rx_Synchronized_Response(u8 data)
{
	static u8 status=0;
    
	switch(status)
	{
		case 0:
		{
			if(data == 'S') {status = 1;}
			else {status = 0;}
			break;		
		}
		case 1:
		{
			if(data == 'y') {status = 2;}
			else {status = 0;}
			break;		
		}
		case 2:
		{
			if(data == 'n') {status = 3;}
			else {status = 0;}
			break;		
		}
		case 3:
		{
			if(data == 'c') {status = 4;}
			else {status = 0;}
			break;		
		}
		case 4:
		{
			if(data == 'h') {status = 5;}
			else {status = 0;}
			break;		
		}
 		case 5:
		{
			if(data == 'r') {status = 6;}
			else {status = 0;}
			break;		
		}
		case 6:
		{
			if(data == 'o') {status = 7;}
			else {status = 0;}
			break;		
		}
		case 7:
		{
			if(data == 'n') {status = 8;}
			else {status = 0;}
			break;		
		}
		case 8:
		{
			if(data == 'i') {status = 9;}
			else {status = 0;}
			break;		
		}
 		case 9:
		{
			if(data == 'z') {status = 10;}
			else {status = 0;}
			break;		
		}
 		case 10:
		{
			if(data == 'e') {status = 11;}
			else {status = 0;}
			break;		
		}
 		case 11:
		{
			if(data == 'd') {status = 12;}
			else {status = 0;}            
			break;		
		}   
 		case 12:
		{
			if((data == '\n')||(data == '\r')) {status = 13;}
			else {status = 0;}            
			break;		
		}      
 		case 13:
		{
			if(data == 'O') {status = 14;}
			else {status = 0;}            
			break;		
		}      
 		case 14:
		{
			if(data == 'K') {status = 15;}
			else {status = 0;}                             
			break;		
		} 
 		case 15:
		{
			if((data == '\n')||(data == '\r')) {dealStatus = 2; printf("\r\nRecevce 'Synchronized OK'");}
			else {} 
            status = 0;                
			break;		
		}        
		default:
		{
			status=0;
			break;
		}
	}
}

void ISP_Rx_Oscillator_Response(u8 data)
{
	static u8 status=0;
	
	switch(status)
	{
		case 0:
		{
			if(data == '1') {status = 1;}
			else {status = 0;}
			break;		
		}
		case 1:
		{
			if(data == '2') {status = 2;}
			else {status = 0;}
			break;		
		}
		case 2:
		{
			if(data == '0') {status = 3;}
			else {status = 0;}
			break;		
		}
		case 3:
		{
			if(data == '0') {status = 4;}
			else {status = 0;}
			break;		
		}
		case 4:
		{
			if(data == '0') {status = 5;}
			else {status = 0;}
			break;		
		}
 		case 5:
		{
			if((data == '\n')||(data == '\r')) {status = 6;}
			else {status = 0;}
			break;		
		}
		case 6:
		{
			if(data == 'O') {status = 7;}
			else {status = 0;}
			break;		
		}
		case 7:
		{
			if(data == 'K') {status = 8;}
			else {status = 0;}
			break;		
		}
		case 8:
		{
			if((data == '\n')||(data == '\r')) {dealStatus = 3; printf("\r\nRecevce 'Oscillator OK'");}
			else {}
            status = 0;                
			break;		
		}      
		default:
		{
			status=0;
			break;
		}

	}
}


#endif 








#if 0
char PrintfToWince(unsigned char *pdata,u8 len)
{
	u8 checksum = 0;
	u8 i = 0;
//	u8 senddata[12];
//	if((len+5) > 12){
//		return 0xff;
//	}
//	senddata[0] = 0xFF;senddata[1] = 0x55;senddata[2] = len+2;
	PrintfChar(0xFF);
	PrintfChar(0x55);
	PrintfChar(len+1);
	checksum = (len+1) ;
	while (i<len) {
		checksum += pdata[i];
		PrintfChar(pdata[i]);
		i++;
	}
//	senddata[i+4] = checksum;
	PrintfChar(checksum);
//	UART1Send(senddata,len+5);
	return 0;
}

void DealWinceCmd(unsigned char *pdata,u16 len)
{
	switch(pdata[0])
	{
		case 0x01:	//初始化，发送参数给上位机
//			currentRGBInfo.Connect=pdata[1];
            pdata[0]=0x02;pdata[1] = currentRGBInfo.Switch;
            PrintfToWince(pdata,2);
            pdata[0]=0x03;pdata[1] = currentRGBInfo.RGB_Radio;
            PrintfToWince(pdata,2);
            pdata[0] = 0x04;
            pdata[1] = currentRGBInfo.FL_Info.R_Value;
            pdata[2] = currentRGBInfo.FL_Info.G_Value;
            pdata[3] = currentRGBInfo.FL_Info.B_Value;
            pdata[4] = currentRGBInfo.FR_Info.R_Value;
            pdata[5] = currentRGBInfo.FR_Info.G_Value;
            pdata[6] = currentRGBInfo.FR_Info.B_Value;
            pdata[7] = currentRGBInfo.RL_Info.R_Value;
            pdata[8] = currentRGBInfo.RL_Info.G_Value;
            pdata[9] = currentRGBInfo.RL_Info.B_Value;
            pdata[10] = currentRGBInfo.RR_Info.R_Value;
            pdata[11] = currentRGBInfo.RR_Info.G_Value;
            pdata[12] = currentRGBInfo.RR_Info.B_Value;       
            PrintfToWince(pdata,13);
			break;
		case 0x02:	//开关控制
            currentRGBInfo.Switch=pdata[1];
            PrintfToWince(pdata,2);
			break;
		case 0x03:	//亮度调节
			currentRGBInfo.RGB_Radio=pdata[1];
            PrintfToWince(pdata,2);
			break;
		case 0x04:	//调节参数
//			currentRGBInfo.Switch=0;
            currentRGBInfo.FL_Info.R_Value = pdata[1];
            currentRGBInfo.FL_Info.G_Value = pdata[2];
            currentRGBInfo.FL_Info.B_Value = pdata[3];
            currentRGBInfo.FR_Info.R_Value = pdata[4];
            currentRGBInfo.FR_Info.G_Value = pdata[5];
            currentRGBInfo.FR_Info.B_Value = pdata[6];
            currentRGBInfo.RL_Info.R_Value = pdata[7];
            currentRGBInfo.RL_Info.G_Value = pdata[8];
            currentRGBInfo.RL_Info.B_Value = pdata[9];
            currentRGBInfo.RR_Info.R_Value = pdata[10];
            currentRGBInfo.RR_Info.G_Value = pdata[11];
            currentRGBInfo.RR_Info.B_Value = pdata[12];          
            PrintfToWince(pdata,13);
			break;		
		default:
			break;			
	}
}


#define recMaxLen	256
void WinceCmdRxProcessor(u8 data)			//对串口接收的数据分析协议 帧头、校验和 更新标准等
{
	static unsigned char checksum;
	static u16 FrameLen;
	static u16 FrameLenMax;
	static unsigned char ReceiveReg[recMaxLen];

	switch (WinceCmdFrameStatus)
	{
		case 0:
			if (data == 0xff) 
				WinceCmdFrameStatus = 1;
			else
				printf("\r\nBad syc_1!!! \r\n  data=%d",data);
			break;
		case 1:
			if (data == 0x55) {
				WinceCmdFrameStatus = 2;
			}
			else{
				WinceCmdFrameStatus = 0;
				printf("\r\nBad syc_2!!! \r\n  data=%d",data);
			}
			break;
		case 2:			//length
			WinceCmdFrameStatus = 3;
			checksum = FrameLenMax = data;//接收长度
			FrameLen = 0;
			break;	
		case 3:			//commamd+parameter	
			if (FrameLen < (FrameLenMax-1))//还未接收够指定长度的数据
			{
				ReceiveReg[FrameLen] = data;
				checksum += data;
				FrameLen ++;
				if(FrameLen == recMaxLen){
					return;
				}
			}
			else//接收够指定长度的数据
			{
				if (data == checksum)
				{
					ReceiveReg[FrameLen] = 0;		
//				printf("rx");
					DealWinceCmd(ReceiveReg,FrameLen);			//接收的数据处理	
				}
				else
				{
					printf("\r\nBad CRC!!! \r\n checksum=%d",checksum);
				}
				WinceCmdFrameStatus = 0;
			}
			break;
		default:
			WinceCmdFrameStatus = 0;
			break;
	}
}
#endif
