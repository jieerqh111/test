#ifndef __DEALRXPROCESSOR_H_
#define __DEALRXPROCESSOR_H_
#include "main.h"

extern u8 WinceCmdFrameStatus;
extern tSYSTEM_CONFIG currentRGBInfo, lastRGBInfo;

void DealWinceCmd(unsigned char *pdata,u16 len);
void EvtUART1RxProcessor(void);
void WinceCmdRxProcessor(u8 data);
void Updata(void);
u8 Light_Lever(void);
void DealProcessor(void);
///////////////////////////////////////////////////////
void ISP_DealUartRxData(u8 data);

void ISP_Tx_SayHi_Order(void);  
void ISP_Tx_Synchronized_Order(void);  
void ISP_Tx_Oscillator_Order(void);

void ISP_Rx_SayHi_Response(u8 data);
void ISP_Rx_Synchronized_Response(u8 data);
void ISP_Rx_Oscillator_Response(u8 data);

void ISP_pdealTx(void);
void ISP_pdealRx(u8 data);

#endif


