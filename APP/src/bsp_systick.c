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


void ResetUserTimer(uint32_t *timer)	//��ֵ�ɵ�ǰʱ��
{
	*timer = SysTickCnt; 
}

uint32_t ReadSysTimer(void)				//��ȡϵͳʱ��
{
	return  SysTickCnt;
}

uint32_t ReadUserTimer(uint32_t *timer)	//��һ��ʱ��Ϊ�ο��㣬�ȹ��˶೤ʱ��
{
	return (SysTickCnt - *timer);
}

uint32_t ReadUserTimerINT(uint32_t *timer)//���ж���ʹ��
{
	return (SysTickCnt - *timer);
}

void SetUserTimer(uint32_t *timer,uint32_t T)
{
	*timer = SysTickCnt + T;
}
