#ifndef _BSP_KEYPAD_H
#define _BSP_KEYPAD_H
#include "main.h"

#define KEYUPSTEP 					0
#define KEYDOWNSTEP 				1
#define IsKeyPutDowm(x)     (KEYDOWNSTEP==x.Step)   //判断ACC等自锁按键是否按下

typedef struct  
{
	uint8_t  Flag;		 //按键的标志//暂时没有使用到
	uint8_t  Step;		 //按键电平的状态, 可以用过判断这个变量是否为1，来判断按键是否被按下
	uint32_t FirstTime;  //第一次改变状态的时间
	uint8_t  FirstFlag;  //计算延时的标志
	const uint32_t PIN_MASK;//引脚
	const uint8_t number;//按键ID
}FlyKeypad;

extern FlyKeypad KeyAir_Display;
extern FlyKeypad KeyREV;
extern FlyKeypad KeyLEFT_DOOR;
extern FlyKeypad KeyBT;
extern FlyKeypad KeyILL;
extern FlyKeypad KeyScreen; 
extern FlyKeypad KeyACC;		
extern FlyKeypad KeyStandby;
void Appliction_Keypad_Init(void);
void Key_Detected(void);


#endif
