#ifndef _BSP_ADCKEY_H
#define _BSP_ADCKEY_H
#include "main.h"

#define ADC_GETTIME   10
#define ADC_GETCOUNT   5  

extern volatile uint32_t ADCKeyFlag;

void ADC_Channel_Init(void);//��ʼ��ADCͨ��
uint32_t ADCKey_Detected(void);//ADC����ɨ��



#endif /* _BSP_ADCKEY_H */

