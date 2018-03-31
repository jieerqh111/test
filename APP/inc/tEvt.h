#ifndef _TEVT_H_
#define _TEVT_H_
#include "main.h"	


void EIC_IRQ_DisableINT_All(void);
void EIC_IRQ_EnableINT_All(void);

/* Disable global interrupts  */
#define _DI()  EIC_IRQ_DisableINT_All()      

/* Enable global interrupts */	
#define _EI()  EIC_IRQ_EnableINT_All()     
		
#endif

/******************* (C) COPYRIGHT 20081024 ileaon*****END OF FILE****/
