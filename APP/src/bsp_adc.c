#include "bsp_adc.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_gpdma.h"

uint32_t ADCTickCnt;
static FlagStatus ADC_GetFlag = SET;

uint32_t ADCKeyFlag;
static uint16_t AIN1Value,AIN2Value,AIN3Value,AIN4Value;
static uint16_t tempAIN1Value=0,tempAIN2Value=0,tempAIN3Value=0,tempAIN4Value=0;

static uint16_t AINValue[ADC_CHANNELCOUNT][ADC_GETCOUNT] = {0};
static uint8_t  AINnumber[4] = {0};
static uint8_t  ADC_CHANNEL[4]={ADC_CHANNEL_2,ADC_CHANNEL_3,ADC_CHANNEL_6,ADC_CHANNEL_7};
uint32_t TotoalAINValue[4]={1,0,0,0};//AIN3,AIN2,AIN1,AIN4

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

	PinCfg.Pinnum = 26;
	PINSEL_ConfigPin(&PinCfg);
	
 	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);
    
 	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);

	/* ADC conversion rate = 200Khz */
	ADC_Init(LPC_ADC, 200000);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN2,DISABLE);    
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_2,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_3,ENABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_6,ENABLE);
    ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_7,ENABLE);
    
 	ADC_BurstCmd(LPC_ADC,ENABLE);	// burst模式
}

//返回AD值
uint32_t ADC_GetValue(uint8_t channel)
{
	 uint32_t adc_value = 0;
    
//     //Wait conversion complete
//     while (!(ADC_ChannelGetStatus(LPC_ADC,channel,ADC_DATA_DONE)));
	 adc_value = ADC_ChannelGetData(LPC_ADC,channel);
 	 return adc_value;
}

uint8_t CalculateKeyValue1(uint16_t adc_value)//对应AIN1
{
    uint8_t keyflag=0;
    if((adc_value > 600) && (adc_value < 800)){//第1个按键
		keyflag |=0x01;
	}
   	else if((adc_value > 1200) && (adc_value < 1400)){//第2个按键
		keyflag |=0x02;
	}
   	else if((adc_value > 1800) && (adc_value < 2000)){//第3个按键
		keyflag |=0x04;
	}
     else if((adc_value > 2400) && (adc_value < 2600)){//第4个按键
		keyflag |=0x08;
	}
   	else if((adc_value > 3000) && (adc_value < 3200)){//第5个按键
		keyflag |=0x10;
	}
	else if((adc_value > 50) && (adc_value < 250)){//第6个按键
		keyflag |=0x20;
	}
    return keyflag;
}	
uint8_t CalculateKeyValue2(uint16_t adc_value)//对应AIN2
{
    uint8_t keyflag=0;
    if((adc_value > 600) && (adc_value < 800)){//第1个按键
		keyflag |=0x01;
	}
   	else if((adc_value > 1200) && (adc_value < 1400)){//第2个按键
		keyflag |=0x02;
	}
   	else if((adc_value > 1800) && (adc_value < 2000)){//第3个按键
		keyflag |=0x04;
	}
     else if((adc_value > 2400) && (adc_value < 2600)){//第4个按键
		keyflag |=0x08;
	}
   	else if((adc_value > 3000) && (adc_value < 3200)){//第5个按键
		keyflag |=0x10;
	}
	else if((adc_value > 50) && (adc_value < 250)){//第6个按键
		keyflag |=0x20;
	}
    return keyflag;
}	

uint8_t CalculateKeyValue3(uint16_t adc_value)//对应AIN3
{
    uint8_t keyflag=0;
    if((adc_value > 600) && (adc_value < 800)){//第1个按键
		keyflag |=0x01;
        _DBG("第1个按键: \r\n");
	}
   	else if((adc_value > 1200) && (adc_value < 1400)){//第2个按键
		keyflag |=0x02;
        _DBG("第2个按键: \r\n");
	}
   	else if((adc_value > 1800) && (adc_value < 2000)){//第3个按键
		keyflag |=0x04;
         _DBG("第3个按键: \r\n");
	}
     else if((adc_value > 2400) && (adc_value < 2600)){//第4个按键
		keyflag |=0x08;
	}
   	else if((adc_value > 3000) && (adc_value < 3200)){//第5个按键
		keyflag |=0x10;
	}
	else if((adc_value > 50) && (adc_value < 250)){//第6个按键
		keyflag |=0x20;
	}
    return keyflag;
}	
uint8_t CalculateKeyValue4(uint16_t adc_value)//对应AIN4
{
    uint8_t keyflag=0;
    if((adc_value > 600) && (adc_value < 800)){//第1个按键
		keyflag |=0x01;
	}
   	else if((adc_value > 1200) && (adc_value < 1400)){//第2个按键
		keyflag |=0x02;
	}
   	else if((adc_value > 1800) && (adc_value < 2000)){//第3个按键
		keyflag |=0x04;
	}
     else if((adc_value > 2400) && (adc_value < 2600)){//第4个按键
		keyflag |=0x08;
	}
   	else if((adc_value > 3000) && (adc_value < 3200)){//第5个按键
		keyflag |=0x10;
	}
	else if((adc_value > 50) && (adc_value < 250)){//第6个按键
		keyflag |=0x20;
	}
    return keyflag;
}	

void ADC_KeyDetected(void)
{
    uint8_t i, j;
    uint32_t temp;
    uint32_t MAX_value[ADC_CHANNELCOUNT] = {0};     
    uint32_t MIN_value[ADC_CHANNELCOUNT] = {0};     
    
    if(ReadUserTimer(&ADCTickCnt) > 10)
    {
        /* 
         * 判断采集数据大小, 若adc_value < 4000, 则有按键按下;		
         * 采集5次数据, 并做处理
         */           
		for(i=0; i < ADC_CHANNELCOUNT; i++)  //4个通道
		{
            temp = ADC_GetValue(ADC_CHANNEL[i]);           
			AINValue[i][AINnumber[i]] = temp;  //把各自通道的ADC数据采集各自的BUF里面
            
            
//			AINValue[i][AINnumber[i]] = ADC_GetValue(ADC_CHANNEL[i]);  //把各自通道的ADC数据采集各自的BUF里面
        
            if(AINValue[i][AINnumber[i]] > 3500) //认为是常态
            {
                AINnumber[i] = 0; 
//                ADC_GetFlag = SET;
            }
            else
            {           
//                        _DBG("AINValue[0]: ");
//                        _DBG_("");                        
//                        _DBD16(AINnumber[0]);
//                        _DBG_("");                        
//                        _DBD16(AINValue[0][AINnumber[i]]);
//                        _DBG_("");                        
                
                if(ADC_GetFlag == SET)
                {
                    AINnumber[i]++; 
                    if(AINnumber[i] >= ADC_GETCOUNT)
                    {                       
                       
//                        ADC_GetFlag = RESET; //等待按键弹起才能继续再次进行接收数据从而判断按键                       
                        /*************************计算总值*******************************************/
                        for(i=0; i<ADC_CHANNELCOUNT; i++)  //4个通道
                        {
                            for(j=0; j < ADC_GETCOUNT; j++)  
                            {
                                TotoalAINValue[i] += AINValue[i][j];
                            }
                        }  
                        /**************************计算出最大值/最小值**************************************/
                        for(i=0; i<ADC_CHANNELCOUNT; i++)  //最大值
                        {
                            for(j=0; j < ADC_GETCOUNT; j++)  
                            {
                                MAX_value[i] = MAX(MAX_value[i], AINValue[i][j]);
                            }
                        }                        
                        for(i=0; i<ADC_CHANNELCOUNT; i++)  //最小值
                        {
                            for(j=0; j < ADC_GETCOUNT; j++)  
                            {
                                MIN_value[i] = MAX(MIN_value[i], AINValue[i][j]);
                            }
                        }                        
                        /**************************减去最大值/最小值**************************************/
                        for(i=0; i<ADC_CHANNELCOUNT; i++)  
                        {
                            for(j=0; j < ADC_GETCOUNT; j++)  
                            {
                                TotoalAINValue[i] = TotoalAINValue[i] - MAX_value[i] - MIN_value[i];
                            }
                        }                                               
                        /**************************计算平均值**********************************************/                        
                        AIN3Value = TotoalAINValue[0]/(ADC_GETCOUNT - 2);
//                        AIN2Value = TotoalAINValue[1]/(ADC_GETCOUNT - 2);
//                        AIN1Value = TotoalAINValue[2]/(ADC_GETCOUNT - 2);
//                        AIN4Value = TotoalAINValue[3]/(ADC_GETCOUNT - 2);     
//                        _DBG("AIN3Value: ");
//                        _DBD32(AIN3Value);
//                        _DBG_("");                        
                        
                        /**************************按键状态值**********************************************/                                               
//                        tempAIN1Value = CalculateKeyValue1(AIN1Value);
//                        tempAIN2Value = CalculateKeyValue2(AIN2Value);
                        tempAIN3Value = CalculateKeyValue3(AIN3Value);
//                        tempAIN4Value = CalculateKeyValue4(AIN4Value);                        
                    }
                }
            }
        }    
        ResetUserTimer(&ADCTickCnt);   
	}
}

void EvtAdcsProcessor(void)
{
    ADC_KeyDetected();
    
	ADCKeyFlag&=0xffffff00;
	ADCKeyFlag|=(tempAIN1Value<<0);
		
	ADCKeyFlag&=0xffff00ff;
	ADCKeyFlag|=(tempAIN2Value<<8);
		
	ADCKeyFlag&=0xff00ffff;
	ADCKeyFlag|=(tempAIN3Value<<16);
		
	ADCKeyFlag&=0x00ffffff;
	ADCKeyFlag|=(tempAIN4Value<<24);
    
//    _DBG("ADCKeyFlag: \r\n");
//    _DBH32(ADCKeyFlag);
//    _DBG("ADCKeyFlag: \r\n");
//    _DBH32(ADCKeyFlag);
}


//uint32_t Get_MAX(uint32_t  *buffer, uint32_t *value)
//{
//    uint8_t i, length = 0;
//    uint32_t max_value = 0;
//    
//    length = NELEMENTS(buffer);
//    
//    for(; i<length; i++ )
//    {
//        max_value = MAX(max_value, buffer[i]);         
//    }
//    
//    *value = max_value;
//    return *value;
//}   
//    
//uint32_t Get_MIN(uint32_t  *buffer)
//{
//    uint8_t i, length;
//    uint32_t min_value;
//    
//    length = NELEMENTS(&buffer);
//    
//    for(; i<length; i++ )
//    {
//        adc_max_min_value = MIN(min_value, buffer[i]);       
//    }
//    
//    return adc_max_min_value;
//}     


