#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H
#include "main.h"


void GPIO_Init(void);   
void GPIO_Toggle(void);   
uint32_t GPIO_ReadPinStatus(uint8_t Portnum);   
void EvtGPIO_ReadPinStatus(uint32_t PinValue);   





#endif /* _BSP_GPIO_H */

