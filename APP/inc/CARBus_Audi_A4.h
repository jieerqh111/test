#ifndef ___CARBUS__AUDI__A4__H_
#define ___CARBUS__AUDI__A4__H_
#include "main.h"


typedef struct AdcKeypad
{
	uint8_t CAN_N; //����ͨ��
	uint32_t ID;   //����ID
	uint32_t IDE;  //���ݸ�ʽ
	uint8_t len;   //���ݳ���
	uint8_t PressInfor[8]; //���µ���Ϣ
	uint8_t LongPressInfor[8]; //��������Ϣ
	uint8_t releaseInfor[8]; //�ͷŵ���Ϣ	
}AdcKeypad;



void EKey_InfoChange(void); //EKEY���Ӧ�ú���
void ADCKey_InfoChange(void); //ADC�������Ӧ�ú���
void Key_InfoChange(void); //�������ؼ��Ӧ�ú���
void ADD_Data_To_CANxTxFifo_APP(uint8_t CAN_N);//�����Է�������
void CAN_Info_Init(void);//Ĭ�ϵ�һЩ���ݵĳ�ʼ��������Ĭ��P�� 


void Gear_InfoChange(void); //��λ
void Angle_InfoChange(void); //�����̽Ƕ�
void Left_InfoChange(void); //���
void Right_InfoChange(void); //�Ҽ�
void Sure_InfoChange(void); //ȷ����
void Back_InfoChange(void); //���ؼ�
void Home_InfoChange(void); //��ҳ��
void Up_InfoChange(void); //�ϼ�
void Down_InfoChange(void); //�¼�
void CAN1Keep_InfoChange(void); //CAN1 ����һ��̶�����
void CAN2Keep_InfoChange(void); //CAN2 ����һ��̶�����


void ACC_InfoChange(void);//ACC
void Radar_InfoChange(void);//Radar
void Standby_InfoChange(void); //����
void Speed_InfoChange(void);//����
void Turn_InfoChange(void);//ת���


#endif
