#include "bsp_uart.h"

#define  UART3EN  0 
#if UART3EN
volatile uint8_t  UART3Buffer[BUFSIZE];
volatile uint32_t UART3RxFifoPush = 0, UART3RxFifoPull = 0;

/*********************************************************************//**
 * @brief		UART3 Init 
 * @param[in]	None
 * @return 		None
 **********************************************************************/

void UART3_Init(uint32_t Baudrate)
{
	// UART Configuration structure variable
	UART_CFG_Type UART_InitStruct;
	// UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UART_FIFOInitStruct;
	// Pin configuration for UART3
	PINSEL_CFG_Type PinCfg;
    
	/*
	 * Initialize UART3 pin connect
	 */	
	PinCfg.Funcnum = 3;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 28;
	PinCfg.Portnum = 4;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 29;
	PINSEL_ConfigPin(&PinCfg);

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 115200
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_InitStruct.Baud_rate = 115200;
	UART_InitStruct.Databits  = UART_DATABIT_8;
	UART_InitStruct.Parity    = UART_PARITY_NONE;
	UART_InitStruct.Stopbits  = UART_STOPBIT_1;    
	// Initialize UART3 peripheral with given to corresponding parameter
	UART_Init(LPC_UART3, &UART_InitStruct);

	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOInitStruct.FIFO_DMAMode = DISABLE;
	UART_FIFOInitStruct.FIFO_Level   = UART_FIFO_TRGLEV0;
	UART_FIFOInitStruct.FIFO_ResetRxBuf = ENABLE;
	UART_FIFOInitStruct.FIFO_ResetTxBuf = ENABLE;
	// Initialize FIFO for UART3 peripheral
	UART_FIFOConfig(LPC_UART3, &UART_FIFOInitStruct);

	// Enable UART Transmit
	UART_TxCmd(LPC_UART3, ENABLE);
    
    /* Enable UART Rx interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_RBR, ENABLE);
	/* Enable UART line status interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART3, UART_INTCFG_RLS, ENABLE);
    
    /* preemption = 1, sub-priority =  3*/
    NVIC_SetPriority(UART3_IRQn, 3);
	/* Enable Interrupt for UART0 channel */
    NVIC_EnableIRQ(UART3_IRQn);

//    UARTPuts(LPC_UART3, "LPC_UART3 is Open!");
}

/*********************************************************************//**
 * @brief		UART3 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void UART3_IRQHandler(void)
{
	uint32_t intsrc, tmp, tmp1;

	/* Determine the interrupt source */
	intsrc = UART_GetIntId(LPC_UART3);
	tmp = intsrc & UART_IIR_INTID_MASK;

	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS){

		// Check line status
		tmp1 = UART_GetLineStatus(LPC_UART3);
		// Mask out the Receive Ready and Transmit Holding empty status
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		// If any error exist
		if (tmp1) {
            UARTPuts(LPC_UART3, "LPC_UART3 IRQ error!");   
		}
	}

	// Receive Data Available or Character time-out
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){                           
			UART3_IntReceive();
	}

}

void UART3_IntReceive(void)
{
	uint8_t tmpc;
	uint32_t rLen;

	while(1){
		// Call UART read function in UART driver
		rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART3, &tmpc, 1, NONE_BLOCKING);
		// If data received
		if (rLen){
			/* Check if buffer is more space
			 * If no more space, remaining character will be trimmed out
			 */
				UART3Buffer[UART3RxFifoPush] = tmpc;
				UART3RxFifoPush=(UART3RxFifoPush+1)&(BUFSIZE-1);
		}
		// no more data
		else {
			break;
		}
	}
}

void EvtUART3RxProcessor(void)
{
	uint8_t data;

	while(UART3RxFifoPull != UART3RxFifoPush)
	{
        data = UART3Buffer[UART3RxFifoPull];
		UART3RxFifoPull=(UART3RxFifoPull+1)&(BUFSIZE-1);

		UARTPutHex (LPC_UART3, data);
    }
}
#endif
#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

/*
 * @}
 */
