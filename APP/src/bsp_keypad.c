#include "bsp_keypad.h"

/* GPIO�궨�� */
#define PortNum_0       (0)
#define PortNum_1 		(1)
#define PortNum_2 		(2)
#define PortNum_3		(3)
#define PortNum_4       (4)

#define GPIO_INPUT      (0)
#define GPIO_OUTPUT     (1)

#define GPIO_PIN_EK1_MASK				(0x1<<8)	//EK1��
#define GPIO_PIN_EK2_MASK				(0x1<<9)	//EK2��

#define GPIO_PIN_STANDBY_MASK 			(0x1<<0)	//����
#define GPIO_PIN_AIR_DISPLAY_MASK 	    (0x1<<1)	//�յ���ʾ
#define GPIO_PIN_REV_MASK 				(0x1<<4)	//����
#define GPIO_PIN_LEFT_BOOR_MASK 		(0x1<<8)	//��ǰ��
#define GPIO_PIN_BT_MASK				(0x1<<9)	//����
#define GPIO_PIN_ILL_MASK 				(0x1<<10)	//���
#define GPIO_PIN_SCREEN_MASK			(0x1<<14)	//��Ļ����
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

//��ʼ������������״̬��Ϣ
FlyKeypad KeyStandby   = {0, 0, 0, 0, IO_Standby, 4};//����
FlyKeypad KeyAir_Display   = {0, 0, 0, 0, IO_Air_Display, 8};//�յ�
FlyKeypad KeyREV       = {0, 0, 0, 0, IO_REV, 3};//����
FlyKeypad KeyLEFT_DOOR = {0, 0, 0, 0, IO_LEFT_DOOR, 7};//��ǰ��
FlyKeypad KeyBT        = {0, 0, 0, 0, IO_BT,6}; //����
FlyKeypad KeyILL       = {0, 0, 0, 0, IO_ILL, 2};//���
FlyKeypad KeyScreen    = {0, 0, 0, 0, IO_Screen,5};// ������
FlyKeypad KeyACC       = {0, 0, 0, 0, IO_ACC,1}; //ACC


//��ȡ���ŵĵ�ƽ״̬
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
**��������:    Appliction_Keypad_Init: ������ʼ�� 
**��������:	   ���ð���Ϊ����
**��ڲ���:   
**���ز���:
******************************************************************************/
void Appliction_Keypad_Init()
{
	uint8_t Keypad[8] = {0,1,4,8,9,10,14,15};
    uint8_t i, arraysize;
    
    arraysize = NELEMENTS(Keypad);
	for(i = 0; i < arraysize; i++){
        GPIO_SetDir(PortNum_1, (1<<Keypad[i]), GPIO_INPUT); //��������
    }
    
	GPIO_SetDir(PortNum_0, GPIO_PIN_EK1_MASK, GPIO_INPUT);	//p0.8 ek1
	GPIO_SetDir(PortNum_0, GPIO_PIN_EK2_MASK, GPIO_INPUT);	//p0.9 ek2
}
/*****************************************************************************
**��������:    Key_One_Detected
**��������:    ��ⵥ������
**��ڲ���:   
**���ز���:
******************************************************************************/
void Key_One_Detected(FlyKeypad *const Keypad)
{
    if(Keypad->FirstFlag != 0xFF)						//״̬��־�Ƿ���λ
	{
        if(IO_Read((Keypad->PIN_MASK)) != Keypad->Step)	//�ж�״̬�Ƿ�ı�
		{									
			Keypad->FirstFlag = 0xFF;					//����״̬�仯��־
			ResetUserTimer(&Keypad->FirstTime);			//�����ٶ�ʱ��
		}			
	}
	else if(ReadUserTimer(&Keypad->FirstTime) > 50)		//50������ٴζ�ȡIO״̬
	{	
		if(IO_Read((Keypad->PIN_MASK)) != Keypad->Step) //�ڶ��β鿴״̬�Ƿ�ı�
		{
			Keypad->Step = IO_Read((Keypad->PIN_MASK)); //ͬ��״̬
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
**��������:    Key_Detected
**��������:	   ��ⰴ��
**��ڲ���:   
**���ز���:
******************************************************************************/
void Key_Detected(void)
{	
	/*******************��ⰴ��*******************************/
	Key_One_Detected(&KeyACC);			//ACC����--ACC
	Key_One_Detected(&KeyILL);			//��ư���--ת���
	Key_One_Detected(&KeyREV);			//��������--����
	Key_One_Detected(&KeyStandby);	    //��������--����
	Key_One_Detected(&KeyBT);			//��������--�״�
    
	Key_One_Detected(&KeyScreen);		//�����ذ���
	Key_One_Detected(&KeyLEFT_DOOR);    //��ǰ�Ű���
	Key_One_Detected(&KeyAir_Display);  //�յ���ʾ����

}


