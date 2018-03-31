#ifndef _BSP_UART_H
#define _BSP_UART_H
#include "lpc_types.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "bsp_systick.h"


#define BUFSIZE     256

void UART3_Init(uint32_t Baudrate);
void UART3_IntReceive(void);
void EvtUART3RxProcessor(void);



#endif /* _bsp_uart_H */

