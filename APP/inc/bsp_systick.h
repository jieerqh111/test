#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H
#include "main.h"

#define 	T_1MS		(1)
#define 	T_10MS		(10)
#define 	T_100MS		(100)
#define 	T_1S		(1000)
#define 	T_1MM		(T_1S*60)
#define    T_8MS         (T_1MS*8)
#define 	T_50MS        (T_1MS*50)
#define 	T_200MS       (T_1MS*200)
#define 	T_500MS       (T_1MS*500)
#define 	T_2S          (T_1MS*2000)
#define 	T_3S          (T_1MS*3000)
#define 	T_30S         (T_1S*30)


void SysTick_Init(void);
void SysTickDeInit(void);
void Delay_ms(__IO uint32_t nTime);
void SysTick_Handler(void);
void ResetUserTimer(uint32_t *timer);
uint32_t ReadSysTimer(void);	
uint32_t ReadUserTimer(uint32_t *timer);
uint32_t ReadUserTimerINT(uint32_t *timer);
void SetUserTimer(uint32_t *timer,uint32_t T);

#endif /* __BSP_SYSTICK_H */
