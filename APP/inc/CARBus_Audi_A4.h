#ifndef ___CARBUS__AUDI__A4__H_
#define ___CARBUS__AUDI__A4__H_
#include "main.h"


typedef struct AdcKeypad
{
	uint8_t CAN_N; //发送通道
	uint32_t ID;   //发送ID
	uint32_t IDE;  //数据格式
	uint8_t len;   //数据长度
	uint8_t PressInfor[8]; //按下的信息
	uint8_t LongPressInfor[8]; //长按的信息
	uint8_t releaseInfor[8]; //释放的信息	
}AdcKeypad;



void EKey_InfoChange(void); //EKEY检测应用函数
void ADCKey_InfoChange(void); //ADC按键检测应用函数
void Key_InfoChange(void); //自锁开关检测应用函数
void ADD_Data_To_CANxTxFifo_APP(uint8_t CAN_N);//周期性发送数据
void CAN_Info_Init(void);//默认的一些数据的初始化，例如默认P档 


void Gear_InfoChange(void); //档位
void Angle_InfoChange(void); //方向盘角度
void Left_InfoChange(void); //左键
void Right_InfoChange(void); //右键
void Sure_InfoChange(void); //确定键
void Back_InfoChange(void); //返回键
void Home_InfoChange(void); //主页键
void Up_InfoChange(void); //上键
void Down_InfoChange(void); //下键
void CAN1Keep_InfoChange(void); //CAN1 发送一组固定数据
void CAN2Keep_InfoChange(void); //CAN2 发送一组固定数据


void ACC_InfoChange(void);//ACC
void Radar_InfoChange(void);//Radar
void Standby_InfoChange(void); //待机
void Speed_InfoChange(void);//车速
void Turn_InfoChange(void);//转向灯


#endif
