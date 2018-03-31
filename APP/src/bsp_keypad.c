#include "bsp_keypad.h"

/* GPIO宏定义 */
#define PortNum_0       (0)
#define PortNum_1 		(1)
#define PortNum_2 		(2)
#define PortNum_3		(3)
#define PortNum_4       (4)

#define GPIO_INPUT      (0)
#define GPIO_OUTPUT     (1)

#define GPIO_PIN_EK1_MASK				(0x1<<8)	//EK1脚
#define GPIO_PIN_EK2_MASK				(0x1<<9)	//EK2脚

#define GPIO_PIN_STANDBY_MASK 			(0x1<<0)	//待机
#define GPIO_PIN_AIR_DISPLAY_MASK 	    (0x1<<1)	//空调显示
#define GPIO_PIN_REV_MASK 				(0x1<<4)	//倒车
#define GPIO_PIN_LEFT_BOOR_MASK 		(0x1<<8)	//左前门
#define GPIO_PIN_BT_MASK				(0x1<<9)	//蓝牙
#define GPIO_PIN_ILL_MASK 				(0x1<<10)	//大灯
#define GPIO_PIN_SCREEN_MASK			(0x1<<14)	//屏幕开关
#define GPIO_PIN_ACC_MASK 				(0x1<<15)	//ACC

typedef enum
{
    IO_ACC = 0,
	IO_Screen,
	IO_ILL,
	IO_BT,
	IO_LEFT_DOOR,
	IO_REV,
	IO_Air_Display,
	IO_Standby
}enumIODef;

//初始化各个按键的状态信息
FlyKeypad KeyStandby   = {0, 0, 0, 0, IO_Standby, 4};//待机
FlyKeypad KeyAir_Display   = {0, 0, 0, 0, IO_Air_Display, 8};//空调
FlyKeypad KeyREV       = {0, 0, 0, 0, IO_REV, 3};//倒车
FlyKeypad KeyLEFT_DOOR = {0, 0, 0, 0, IO_LEFT_DOOR, 7};//左前门
FlyKeypad KeyBT        = {0, 0, 0, 0, IO_BT,6}; //蓝牙
FlyKeypad KeyILL       = {0, 0, 0, 0, IO_ILL, 2};//大灯
FlyKeypad KeyScreen    = {0, 0, 0, 0, IO_Screen,5};// 屏开关
FlyKeypad KeyACC       = {0, 0, 0, 0, IO_ACC,1}; //ACC


//读取引脚的电平状态
uint8_t IO_Read(int8_t IOEnum)
{
	uint8_t port;
	uint32_t Mask;
    
	switch(IOEnum)
	{
		case IO_ACC:		port = PortNum_1; Mask = GPIO_PIN_ACC_MASK;
            break;
		case IO_Screen:		port = PortNum_1; Mask = GPIO_PIN_SCREEN_MASK;
            break;
		case IO_ILL:		port = PortNum_1; Mask = GPIO_PIN_ILL_MASK;
            break;
		case IO_BT:			port = PortNum_1; Mask = GPIO_PIN_BT_MASK;
            break;
		case IO_LEFT_DOOR:	port = PortNum_1; Mask = GPIO_PIN_LEFT_BOOR_MASK;
            break;
		case IO_REV:		port = PortNum_1; Mask = GPIO_PIN_REV_MASK;
            break;
		case IO_Air_Display:	port = PortNum_1; Mask = GPIO_PIN_AIR_DISPLAY_MASK;
            break;
		case IO_Standby:	port = PortNum_1; Mask = GPIO_PIN_STANDBY_MASK;
            break; 
	}
	return (GPIO_ReadValue(port)&(Mask))==(Mask);
}
/*****************************************************************************
**函数名称:    Appliction_Keypad_Init: 按键初始化 
**函数功能:	   设置按键为输入
**入口参数:   
**返回参数:
******************************************************************************/
void Appliction_Keypad_Init()
{
	uint8_t Keypad[8] = {0,1,4,8,9,10,14,15};
    uint8_t i, arraysize;
    
    arraysize = NELEMENTS(Keypad);
	for(i = 0; i < arraysize; i++){
        GPIO_SetDir(PortNum_1, (1<<Keypad[i]), GPIO_INPUT); //自锁开关
    }
    
	GPIO_SetDir(PortNum_0, GPIO_PIN_EK1_MASK, GPIO_INPUT);	//p0.8 ek1
	GPIO_SetDir(PortNum_0, GPIO_PIN_EK2_MASK, GPIO_INPUT);	//p0.9 ek2
}
/*****************************************************************************
**函数名称:    Key_One_Detected
**函数功能:    检测单个按键
**入口参数:   
**返回参数:
******************************************************************************/
void Key_One_Detected(FlyKeypad *const Keypad)
{
    if(Keypad->FirstFlag != 0xFF)						//状态标志是否置位
	{
        if(IO_Read((Keypad->PIN_MASK)) != Keypad->Step)	//判断状态是否改变
		{									
			Keypad->FirstFlag = 0xFF;					//设立状态变化标志
			ResetUserTimer(&Keypad->FirstTime);			//设立再读时间
		}			
	}
	else if(ReadUserTimer(&Keypad->FirstTime) > 50)		//50毫秒后再次读取IO状态
	{	
		if(IO_Read((Keypad->PIN_MASK)) != Keypad->Step) //第二次查看状态是否改变
		{
			Keypad->Step = IO_Read((Keypad->PIN_MASK)); //同步状态
			if(Keypad->Step){
                printf("\r\n%02X Keypad Down", Keypad->number);
            }
			else{
                printf("\r\n%02X Keypad Up", Keypad->number);
            }
		}	
		Keypad->FirstFlag = 0x00;	
	}	
}
/*****************************************************************************
**函数名称:    Key_Detected
**函数功能:	   检测按键
**入口参数:   
**返回参数:
******************************************************************************/
void Key_Detected(void)
{	
	/*******************检测按键*******************************/
	Key_One_Detected(&KeyACC);			//ACC按键--ACC
	Key_One_Detected(&KeyILL);			//大灯按键--转向灯
	Key_One_Detected(&KeyREV);			//倒车按键--车速
	Key_One_Detected(&KeyStandby);	    //待机按键--待机
	Key_One_Detected(&KeyBT);			//蓝牙按键--雷达
    
	Key_One_Detected(&KeyScreen);		//屏开关按键
	Key_One_Detected(&KeyLEFT_DOOR);    //左前门按键
	Key_One_Detected(&KeyAir_Display);  //空调显示按键

}


