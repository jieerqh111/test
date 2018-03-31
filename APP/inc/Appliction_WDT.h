#ifndef __APPLICTION_WDT_H__
#define __APPLICTION_WDT_H__
#include "main.h"

#define  T_1_S      0xf4240

void Appliction_WDOG_Init(unsigned long outtime);
void Appliction_WDOG_Start(unsigned long outtime);


#endif




