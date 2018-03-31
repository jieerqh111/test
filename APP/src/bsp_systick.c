#include "bsp_systick.h"

/* SysTick Counter */
volatile unsigned long SysTickCnt;

/*********************************************************************//**
 * @brief		SysTick handler sub-routine (1ms)
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SysTick_Handler (void) 
{
    SysTickCnt++;
}

/*********************************************************************//**
 * @brief		SysTick handler Init (1ms)
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SysTick_Init(void)
{
    SysTick_Config(SystemCoreClock/1000 - 1); /* Generate interrupt each 1 ms   */
}

/*********************************************************************//**
 * @brief		Delay function
 * @param[in]	tick - number milisecond of delay time
 * @return 		None
 **********************************************************************/
void Delay_ms(__IO uint32_t tick) 
{
    unsigned long systickcnt;
    
    systickcnt = SysTickCnt;
    while ((SysTickCnt - systickcnt) < tick);
}


void SysTickDeInit(void)
{
    SysTick->CTRL = 0;
}


void ResetUserTimer(uint32_t *timer)	//赋值成当前时间
{
	*timer = SysTickCnt; 
}

uint32_t ReadSysTimer(void)				//读取系统时间
{
	return  SysTickCnt;
}

uint32_t ReadUserTimer(uint32_t *timer)	//以一个时间为参考点，度过了多长时间
{
	return (SysTickCnt - *timer);
}

uint32_t ReadUserTimerINT(uint32_t *timer)//在中断中使用
{
	return (SysTickCnt - *timer);
}

void SetUserTimer(uint32_t *timer,uint32_t T)
{
	*timer = SysTickCnt + T;
}
