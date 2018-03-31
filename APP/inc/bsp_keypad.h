#ifndef _BSP_KEYPAD_H
#define _BSP_KEYPAD_H
#include "main.h"

#define KEYUPSTEP 					0
#define KEYDOWNSTEP 				1
#define IsKeyPutDowm(x)     (KEYDOWNSTEP==x.Step)   //�ж�ACC�����������Ƿ���

typedef struct  
{
	uint8_t  Flag;		 //�����ı�־//��ʱû��ʹ�õ�
	uint8_t  Step;		 //������ƽ��״̬, �����ù��ж���������Ƿ�Ϊ1�����жϰ����Ƿ񱻰���
	uint32_t FirstTime;  //��һ�θı�״̬��ʱ��
	uint8_t  FirstFlag;  //������ʱ�ı�־
	const uint32_t PIN_MASK;//����
	const uint8_t number;//����ID
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
