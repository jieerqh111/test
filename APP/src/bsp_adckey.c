#include "bsp_adckey.h"


volatile uint32_t ADCKeyFlag;
uint32_t ADCTickCnt;
static FlagStatus ADC_GetFlag = SET;
static uint16_t tempAIN1Value=0,tempAIN2Value=0,tempAIN3Value=0,tempAIN4Value=0;
static uint16_t AIN1Value,AIN2Value,AIN3Value,AIN4Value;
static uint32_t TotoalAIN1Value,TotoalAIN2Value,TotoalAIN3Value,TotoalAIN4Value;


void ADC_Channel_Init(void)
{
	/* Initialize ADC ----------------------------------------------------*/
	/*
	 * Init ADC pin connect
 	 * select P0.25 as AD0.2--ADC[2]--AIN3
	 * select P0.26 as AD0.3--ADC[3]--AIN2
 	 * select P0.3  as AD0.6--ADC[6]--AIN1
	 * select P0.2  as AD0.7--ADC[7]--AIN4
	 */
    PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 25;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);

   	PinCfg.Funcnum = 1;
	PinCfg.Pinnum = 26;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Funcnum = 2;
    PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0;
    
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Funcnum = 2;
 	PinCfg.Pinnum = 3;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	/* ADC conversion rate = 200Khz */
	ADC_Init(LPC_ADC, 200000);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_2,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_3,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_6,ENABLE);
    ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_7,ENABLE);
    
 	ADC_BurstCmd(LPC_ADC,ENABLE);	// burst模式
}
//读取ADC
static uint32_t ADC_GetValue(uint8_t channel)
{
	 uint32_t adc_value = 0;
    
	 adc_value = ADC_ChannelGetData(LPC_ADC,channel);
 	 return adc_value;
}
//比较按键值,计数
static uint8_t CalculatetemptempAIN1Value(uint16_t adc_value)//对应AIN1, 第4行按键
{
    if((adc_value > 670) && (adc_value < 760)){//第1个按键 716
		TotoalAIN1Value += 100; return SET;
	}
   	else if((adc_value > 1290) && (adc_value < 1360)){//第2个按键 1324
		TotoalAIN1Value += 200; return SET;
	}
   	else if((adc_value > 1880) && (adc_value < 1950)){//第3个按键 1915
		TotoalAIN1Value += 300; return SET;
	}
    else if((adc_value > 2460) && (adc_value < 2540)){//第4个按键 2502
		TotoalAIN1Value += 400; return SET;
	}
   	else if((adc_value > 3070) && (adc_value < 3155)){//第5个按键 3113
		TotoalAIN1Value += 500; return SET;
	}
	else if((adc_value > 120) && (adc_value < 200)){//第6个按键 160
		TotoalAIN1Value += 600; return SET;
	}
    return RESET;
}	
static uint8_t CalculatetemptempAIN2Value(uint16_t adc_value)//对应AIN2, 第2行按键
{
    if((adc_value > 1870) && (adc_value < 1955)){//第1个按键 1912
		TotoalAIN2Value += 100; return SET;
	}
   	else if((adc_value > 1280) && (adc_value < 1360)){//第2个按键 1321 
		TotoalAIN2Value += 200; return SET;
	}
   	else if((adc_value > 665) && (adc_value < 745)){//第3个按键 704 
		TotoalAIN2Value += 300; return SET;
	}
    else if((adc_value > 2465) && (adc_value < 2545)){//第4个按键 2503 
		TotoalAIN2Value += 400; return SET;
	}
   	else if((adc_value > 3070) && (adc_value < 3150)){//第5个按键 3111 
		TotoalAIN2Value += 500; return SET;
	}
	else if((adc_value > 115) && (adc_value < 200)){//第6个按键 157
		TotoalAIN2Value += 600; return SET;
	}
    return RESET;
}
static uint8_t CalculatetemptempAIN3Value(uint16_t adc_value)//对应AIN3, 第1行按键
{
    if((adc_value > 670) && (adc_value < 755)){//第1个按键 713
		TotoalAIN3Value += 100; return SET;
	}
   	else if((adc_value > 1280) && (adc_value < 1360)){//第2个按键 1320
		TotoalAIN3Value += 200; return SET;
	}
   	else if((adc_value > 1875) && (adc_value < 1955)){//第3个按键 1916
		TotoalAIN3Value += 300; return SET;
	}
    else if((adc_value > 2450) && (adc_value < 2530)){//第4个按键 2492 
		TotoalAIN3Value += 400; return SET;
	}
   	else if((adc_value > 3070) && (adc_value < 3155)){//第5个按键 3113
		TotoalAIN3Value += 500; return SET;
	}
	else if((adc_value > 120) && (adc_value < 205)){//第6个按键 163
		TotoalAIN3Value += 600; return SET;
	}
    return RESET;
}	
static uint8_t CalculatetemptempAIN4Value(uint16_t adc_value)//对应AIN4, 第3行按键
{
    if((adc_value > 1875) && (adc_value < 1955)){//第1个按键 1915
		TotoalAIN4Value += 100; return SET;
	}
   	else if((adc_value > 1280) && (adc_value < 1360)){//第2个按键 1320
		TotoalAIN4Value += 200; return SET;
	}
   	else if((adc_value > 670) && (adc_value < 750)){//第3个按键 710
		TotoalAIN4Value += 300; return SET;
	}
    else if((adc_value > 2460) && (adc_value < 2540)){//第4个按键 2500
		TotoalAIN4Value += 400; return SET;
	}
   	else if((adc_value > 3570) && (adc_value < 3650)){//第5个按键 3608
		TotoalAIN4Value += 500; return SET;
	}
	else if((adc_value > 3070) && (adc_value < 3155)){//第6个按键 3112
		TotoalAIN4Value += 600; return SET;
	}
    return RESET;
}	
//计算出对应的按键
static uint8_t CalculateKeyValue1(uint16_t value)//对应AIN1, 第4行按键
{
    uint8_t keyflag=0;
    if(((value >= 100)&&(value <= 100))){//第1个按键
        keyflag |= 0x01;
        printf("\r\nButton: 4-1 ");
	}
   	else if(((value >= 200)&&(value <= 200))){//第2个按键
        keyflag |= 0x02;
        printf("\r\nButton: 4-2 ");
	}
   	else if(((value >= 300)&&(value <= 300))){//第3个按键
        keyflag |= 0x04;
        printf("\r\nButton: 4-3 ");
	}
	else if(((value >= 400)&&(value <=400))){//第4个按键
        keyflag |= 0x08;
        printf("\r\nButton: 4-4 ");
	}
   	else if(((value >= 500)&&(value <= 500))){//第5个按键
        keyflag |= 0x10;
        printf("\r\nButton: 4-5 ");
	}
	 else if(((value >= 600)&&(value <= 600))){//第6个按键
        keyflag |= 0x20;
        printf("\r\nButton: 4-6 ");
	}
	return keyflag;
}
static uint8_t CalculateKeyValue2(uint16_t value)//对应AIN2, 第2行按键
{
    uint8_t keyflag=0;
    if(((value >= 100)&&(value <= 100))){//第1个按键
        keyflag |= 0x01;
        printf("\r\nButton: 2-1 ");
	}
   	else if(((value >= 200)&&(value <= 200))){//第2个按键
        keyflag |= 0x02;
        printf("\r\nButton: 2-2 ");
	}
   	else if(((value >= 300)&&(value <= 300))){//第3个按键
        keyflag |= 0x04;
        printf("\r\nButton: 2-3 ");
	}
	else if(((value >= 400)&&(value <=400 ))){//第4个按键
        keyflag |= 0x08;
        printf("\r\nButton: 2-4 ");
	}
   	else if(((value >= 500)&&(value <= 500))){//第5个按键
        keyflag |= 0x10;
        printf("\r\nButton: 2-5 ");
	}
	 else if(((value >= 600)&&(value <= 600))){//第6个按键
        keyflag |= 0x20;
        printf("\r\nButton: 2-6 ");
	}
	return keyflag;	
}	
static uint8_t CalculateKeyValue3(uint16_t value)//对应AIN3, 第1行按键
{
    uint8_t keyflag=0;
    if(((value >= 100)&&(value <= 100))){//第1个按键
        keyflag |= 0x01;
        printf("\r\nButton: 1-1 ");
	}
   	else if(((value >= 200)&&(value <= 200))){//第2个按键
        keyflag |= 0x02;
        printf("\r\nButton: 1-2 ");
	}
   	else if(((value >= 300)&&(value <= 300))){//第3个按键
        keyflag |= 0x04;
        printf("\r\nButton: 1-3 ");
	}
	else if(((value >= 400)&&(value <= 400))){//第4个按键
        keyflag |= 0x08;
        printf("\r\nButton: 1-4 ");
	}
   	else if(((value >= 500)&&(value <=500))){//第5个按键
        keyflag |= 0x10;
        printf("\r\nButton: 1-5 ");
	}
	 else if(((value>=600)&&(value<=600))){//第6个按键
        keyflag |= 0x20;
        printf("\r\nButton: 1-6 ");
	}
	return keyflag;
}	
static uint8_t CalculateKeyValue4(uint16_t value)//对应AIN4, 第3行按键
{
    uint8_t keyflag=0;
    if(((value >= 100)&&(value <= 100))){//第1个按键
        keyflag |= 0x01;
        printf("\r\nButton: 3-1 ");
	}
   	else if(((value >= 200)&&(value <= 200))){//第2个按键
        keyflag |= 0x02;
        printf("\r\nButton: 3-2 ");
	}
   	else if(((value >= 300)&&(value <= 300))){//第3个按键
        keyflag |= 0x04;
        printf("\r\nButton: 3-3 ");
	}
	else if(((value >= 400)&&(value <=400 ))){//第4个按键
        keyflag |= 0x08;
        printf("\r\nButton: 3-4 ");
	}
   	else if(((value >= 500)&&(value <= 500))){//第5个按键
        keyflag |= 0x10;
        printf("\r\nButton: 3-5 ");
	}
	 else if(((value >= 600)&&(value <= 600))){//第6个按键
        keyflag |= 0x20;
        printf("\r\nButton: 3-6 ");
	}
	return keyflag;	
}	
//返回按键标记变量，通过判断变量的位，进行识别不同的按键的状态
uint32_t ADCKey_Detected(void)
{
    uint32_t tempKey;
    uint8_t tempTestFlag1, tempTestFlag2, tempTestFlag3, tempTestFlag4;
	static uint16_t count=0;

    if(ReadUserTimer(&ADCTickCnt) > ADC_GETTIME)
    {
        ResetUserTimer(&ADCTickCnt);
		 
		tempAIN1Value=ADC_GetValue(ADC_CHANNEL_6);
        tempAIN2Value=ADC_GetValue(ADC_CHANNEL_3);
        tempAIN3Value=ADC_GetValue(ADC_CHANNEL_2);		 
        tempAIN4Value=ADC_GetValue(ADC_CHANNEL_7);
	 
        if((tempAIN1Value>4000)&&(tempAIN2Value>4000)&&(tempAIN3Value>4000)&&(tempAIN4Value>4000))
        {            
            ADC_GetFlag = SET; 
            
            TotoalAIN1Value = 0;
            TotoalAIN2Value = 0;
            TotoalAIN3Value = 0;
            TotoalAIN4Value = 0;
            count = 0;
            ADCKeyFlag = 0;            
        }
        else
        {    
            if(ADC_GetFlag == SET)
            {  
                tempTestFlag1 = CalculatetemptempAIN1Value(tempAIN1Value);
                tempTestFlag2 = CalculatetemptempAIN2Value(tempAIN2Value);
                tempTestFlag3 = CalculatetemptempAIN3Value(tempAIN3Value);
                tempTestFlag4 = CalculatetemptempAIN4Value(tempAIN4Value);
 
                if((RESET==tempTestFlag1)&&(RESET==tempTestFlag2)&&(RESET==tempTestFlag3)&&(RESET==tempTestFlag4)) {
                    count = 0;
                } 
                else{count++;}
            }

            if(count >= ADC_GETCOUNT)
            {//计算平均值
                ADC_GetFlag = RESET; 
                
                AIN1Value = TotoalAIN1Value/ADC_GETCOUNT;
                tempKey = CalculateKeyValue1(AIN1Value);
                ADCKeyFlag &= 0xffffff00;
                ADCKeyFlag |= (tempKey<<0);
                        
                AIN2Value = TotoalAIN2Value/ADC_GETCOUNT;
                tempKey = CalculateKeyValue2(AIN2Value);
                ADCKeyFlag &= 0xffff00ff;
                ADCKeyFlag |= (tempKey<<8);
                    
                AIN3Value = TotoalAIN3Value/ADC_GETCOUNT; 
                tempKey = CalculateKeyValue3(AIN3Value);
                ADCKeyFlag &= 0xff00ffff;
                ADCKeyFlag |= (tempKey<<16);
                        
                AIN4Value = TotoalAIN4Value/ADC_GETCOUNT;
                tempKey = CalculateKeyValue4(AIN4Value);
                ADCKeyFlag &= 0x00ffffff;
                ADCKeyFlag |= (tempKey<<24);
                    
                TotoalAIN1Value = 0;
                TotoalAIN2Value = 0;
                TotoalAIN3Value = 0;
                TotoalAIN4Value = 0;
                count = 0;
            }
        }
	}
    return ADCKeyFlag;
}

