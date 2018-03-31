#include "Appliction_WDT.h"


void Appliction_WDOG_Init(unsigned long outtime)
{
	
	WDT_Init(WDT_CLKSRC_IRC, WDT_MODE_RESET);//RC oscillator 
	WDT_Start(outtime);//

}
void Appliction_WDOG_Start(unsigned long outtime)
{
	WDT_Start(outtime);//
}



