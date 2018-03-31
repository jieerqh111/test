#include "CARBus_Audi_A4.h"

/*****宏定义ID、声明数组、声明CAN结构体*****/
/*****宏定义ID***********/
//轻触按键：
#define CarEKey_ID      0x17F8F073  
#define CarLeft_ID      0x17F8F073  
#define CarRight_ID     0x17F8F073  
#define CarSure_ID      0x17F8F073  
#define CarBack_ID      0x17F8F073  
#define CarHome_ID      0x17F8F073 
#define CarUp_ID        0x17F8F073 
#define CarDown_ID      0x17F8F073 
#define CarGear_ID      0x3BE  //A4
#define CarAngle_ID     0x086  //A4
#define CarTime1_ID     0x17331110 //同步时间：时-分-秒
#define CarTime2_ID     0x17331110 //同步时间：年-月-日
//自锁开关：
#define CarACC_ID       0x3C0  
#define CarSpeed_ID     0x30B  
#define CarTurn_ID      0x366  
#define CarRADAR1_ID    0x17330A10  
#define CarRADAR2_ID    0x17330A10  
#define CarRADAR3_ID    0x17330A10  

//固定信息，保证CAN1/CAN2不休眠:
#define CarCAN1_ID      0x01
#define CarCAN2_ID      0x02    


/******声明数组**********/
//轻触按键：
uint8_t CarEKeyData[8]   =  {0x05,0x48,0x03,0x00,0x00,0x00,0xAA,0xAA};//旋钮
uint8_t CarLeftData[8]   =  {0x03,0x38,0x0F,0x00,0x00,0xAA,0xAA,0xAA};//左键
uint8_t CarRightData[8]  =  {0x03,0x38,0x5C,0x00,0x00,0xAA,0xAA,0xAA};//右键
uint8_t CarSureData[8]   =  {0x04,0x30,0x01,0x00,0x00,0xAA,0xAA,0xAA};//确定键
uint8_t CarBackData[8]   =  {0x04,0x30,0x0F,0x00,0x00,0xAA,0xAA,0xAA};//返回键
uint8_t CarHomeData[8]   =  {0x04,0x30,0x1A,0x00,0x00,0xAA,0xAA,0xAA};//主页键
uint8_t CarUpData[8]     =  {0x03,0x38,0x0A,0x00,0x00,0xAA,0xAA,0xAA};//上键
uint8_t CarDownData[8]   =  {0x03,0x38,0x0B,0x00,0x00,0xAA,0xAA,0xAA};//下键
uint8_t CarGearData[8]   =  {0X3E,0X09,0X97,0X51,0XC8,0X00,0X00,0X80};//档位
uint8_t CarAngleData[8]  =  {0xD5,0x0C,0x00,0x12,0x00,0xAA,0x0C,0x00};//方向盘
uint8_t CarTime1Data[8]  =  {0x44,0x51,0x0A,0x0A,0x0A,0x00,0x00,0x00};//时间：时-分-秒
uint8_t CarTime2Data[8]  =  {0x44,0x50,0x11,0x08,0x09,0x00,0x00,0x00};//时间：年-月-日
//自锁开关：
uint8_t CarACCData[4]    =  {0x92,0x0f,0x00,0x00};//ACC
uint8_t CarSpeedData[8]  =  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//Speed
uint8_t CarTurnData[8]   =  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//Turn
uint8_t CarRadar1Data[4] =  {0x52,0x95,0x00,0x00};//Radar1	
uint8_t CarRadar2Data[8] =  {0x42,0x93,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//Radar2	
uint8_t CarRadar3Data[8] =  {0x42,0x92,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//Radar3	
//固定信息，保证CAN1/CAN2不休眠:
uint8_t CarCAN1Data[8] =  {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//CAN1 Channel
uint8_t CarCAN2Data[8] =  {0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//CAN2 Channel


/******声明CAN结构体**********/
//轻触按键：
CAN_MSG_Type CarEKey_MSG;
CAN_MSG_Type CarLeft_MSG;
CAN_MSG_Type CarRight_MSG;
CAN_MSG_Type CarSure_MSG;
CAN_MSG_Type CarBack_MSG;
CAN_MSG_Type CarHome_MSG;
CAN_MSG_Type CarUp_MSG;
CAN_MSG_Type CarDown_MSG;
CAN_MSG_Type CarGear_MSG;
CAN_MSG_Type CarAngle_MSG;
CAN_MSG_Type CarTime1_MSG;
CAN_MSG_Type CarTime2_MSG;
//自锁开关：
CAN_MSG_Type CarACC_MSG;
CAN_MSG_Type CarSpeed_MSG;
CAN_MSG_Type CarTurn_MSG;
CAN_MSG_Type CarRadar1_MSG;
CAN_MSG_Type CarRadar2_MSG;
CAN_MSG_Type CarRadar3_MSG;
//固定信息，保证CAN1/CAN2不休眠:
CAN_MSG_Type CarCAN1_MSG;
CAN_MSG_Type CarCAN2_MSG;



//休眠和待机变量
Bool ACCFirstFlag = TRUE;
Bool ACCSecondFlag = TRUE;
Bool StandbyFlag = TRUE;


/*****************************************************************************
**函数名称:    ADCKeyInfoSend
**函数功能:	   轻触按键检测应用函数
**入口参数:   
**返回参数:
******************************************************************************/
void ADCKey_InfoChange(void)
{
    Gear_InfoChange();//档位
    Angle_InfoChange();//方向盘角度
    Left_InfoChange();//左键
    Right_InfoChange();//右键
    Sure_InfoChange();//确定键
    Back_InfoChange();//返回键
    Home_InfoChange();//主页键
    Up_InfoChange(); //上键
    Down_InfoChange(); //下键
}
/*****************************************************************************
**函数名称:    KeyInfoSend
**函数功能:	   自锁开关检测应用函数
**入口参数:   
**返回参数:
******************************************************************************/
void Key_InfoChange(void)
{
    ACC_InfoChange();//ACC
    Turn_InfoChange();//转向灯
    Speed_InfoChange();//车速
    Standby_InfoChange();//待机
    Radar_InfoChange();//雷达


}

/*******************************CarEKey_ID*******************************************
**函数名称:	 EKeyInfoSend
**函数功能:	 检测按键EK
**入口参数:
**返回参数:
******************************************************************************/
void EKey_InfoChange()
{
	static uint8_t flag = 0;		
	static uint8_t ek1_step = 1;
	static uint8_t ek2_step = 1;
	static uint32_t ekIntervalTime;
	static uint8_t left = 0xff,right = 0x1;
    
	if(ek1_step != (GPIO_ReadValue(0)&(0x300))>>8) //如果有旋转
	{	
        ek2_step = ek1_step; //存放当前状态
		ek1_step = (GPIO_ReadValue(0)&(0x300))>>8; //获取最新状态
        
		/********************比较两个状态得出左旋还是右旋********************/
        if(((ek2_step == 3)&&(ek1_step == 1))||((ek2_step == 0)&&(ek1_step == 2))) //左旋
		{
			if(flag != 2)
			{
				CarEKeyData[3] = left--; //左旋一下减1	                
			}
			flag=1; //标志位
		}
		else if(((ek2_step == 3)&&(ek1_step == 2))||((ek2_step == 0)&&(ek1_step == 1))) //右旋
		{
			if(flag != 1)
			{
				CarEKeyData[3] = right++; //右旋一下加一 
			}
			flag = 2; //标志位
		}	
	}
    
	if(ReadUserTimer(&ekIntervalTime) > T_1MS*200) //如果200ms内没有另一次旋转
	{
		ResetUserTimer(&ekIntervalTime);
		if((flag == 1)||(flag == 2))
		{	
			flag = 0;	
			left = 0xff;
            right = 0x1;
            
            Appliction_CanData_Init(&CarEKey_MSG, CarEKeyData, 8, CarEKey_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarEKey_MSG);//将要发送的数据放到发送FIFO里等待发送
		}
	}
}

/***********************CarGear_ID*****************************/
void Gear_InfoChange(void) //1-1、1-2、1-3、1-4按键
{          
    static bool PGear = FALSE;
    static bool RGear = FALSE;
    static bool NGear = FALSE;
    static bool DGear = FALSE;

    /* 1-1 按键按下  P档 */
    if((ADCKeyFlag>>16)&(0x01))
    {
        if(PGear == FALSE)
        {
            CarGearData[2] = 0X97;
            
            Appliction_CanData_Init(&CarGear_MSG, CarGearData, 8, CarGear_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarGear_MSG);//将要发送的数据放到发送FIFO里等待发送
            
            PGear = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象  
        }
    }
    /* 1-2 按键按下  R档 */
    else if((ADCKeyFlag>>16)&(0x02))
    {
        if(RGear == FALSE)
        {
            CarGearData[2] = 0XD7;
            
            Appliction_CanData_Init(&CarGear_MSG, CarGearData, 8, CarGear_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarGear_MSG);//将要发送的数据放到发送FIFO里等待发送
            
            RGear = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象  
        }
    }
    /* 1-3 按键按下  N档 */
    else if((ADCKeyFlag>>16)&(0x04))
    {
        if(NGear == FALSE)
        {
            CarGearData[2] = 0X07;
            
            Appliction_CanData_Init(&CarGear_MSG, CarGearData, 8, CarGear_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarGear_MSG);//将要发送的数据放到发送FIFO里等待发送
            
            NGear = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象  
        }
    }
    /* 1-4 按键按下  D档 */
    else if((ADCKeyFlag>>16)&(0x08))
    {
        if(DGear == FALSE)
        {
            CarGearData[2] = 0XB7;
            
            Appliction_CanData_Init(&CarGear_MSG, CarGearData, 8, CarGear_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarGear_MSG);//将要发送的数据放到发送FIFO里等待发送
            
            DGear = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象  
        }
    }
    else //没有按键被按下，则重置标志，等待下次按下按键后可以发送按下的数据
    {
        PGear = FALSE;
        RGear = FALSE;
        NGear = FALSE;
        DGear = FALSE;
    }   

}

/***********************CarAngle_ID***************************************/
void Angle_InfoChange(void) //1-5、1-6按键
{      
    static bool angleLeftFlag = FALSE;
    static bool angleRightFlag = FALSE;
    
    /* 1-5 按键按下  方向盘左转 */
    if((ADCKeyFlag>>16)&(0x10))
    {
        if(angleLeftFlag == FALSE)
        {
            CarAngleData[3] -= 0x20; //角度-
            CarAngleData[2] = CarAngleData[3]; //赋值

            Appliction_CanData_Init(&CarAngle_MSG, CarAngleData, 8, CarAngle_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarAngle_MSG);//将要发送的数据放到发送FIFO里等待发送  
                    
            angleLeftFlag = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象           
        }
    }
    /* 1-6 按键按下  方向盘右转  */
    else if((ADCKeyFlag>>16)&(0x20))
    {
        if(angleRightFlag == FALSE)
        {

            CarAngleData[3] += 0x20; //角度+
            CarAngleData[2] = CarAngleData[3]; //赋值

            Appliction_CanData_Init(&CarAngle_MSG, CarAngleData, 8, CarAngle_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarAngle_MSG);//将要发送的数据放到发送FIFO里等待发送  
        
            angleRightFlag = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象           
        }
    }
     /* 没有方向盘按键按下*/   
    else
    {
        angleLeftFlag = FALSE; //标志，等待下次按下按键后可以发送按下的数据
        angleRightFlag = FALSE; //标志，等待下次按下按键后可以发送按下的数据
    }   
}
/***********************CarLeft_ID*****************************/
void Left_InfoChange(void) //3-4按键
{
    static bool flag = FALSE;

    if((ADCKeyFlag>>24)&(0x08))
    {
        if(flag == FALSE)
        {
            CarLeftData[3] = 0x07;
            Appliction_CanData_Init(&CarLeft_MSG, CarLeftData, 8, CarLeft_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarLeft_MSG);//将要发送的数据放到发送FIFO里等待发送
                       
            flag = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象           
        }            
    }
    else
    {
//        CarLeftData[3] = 0x00;
        
        flag = FALSE; //标志，等待下次按下按键后可以发送按下的数据
    }
}
/***********************CarRight_ID*****************************/
void Right_InfoChange(void) //3-6按键
{
    static bool flag = FALSE;
    
    if((ADCKeyFlag>>24)&(0x20))
    {
        if(flag == FALSE)
        {
            CarRightData[3] = 0x03;
            Appliction_CanData_Init(&CarRight_MSG, CarRightData, 8, CarRight_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarRight_MSG);//将要发送的数据放到发送FIFO里等待发送
            
            flag = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象           
        }
    }
    else
    {
//        CarRightData[3] = 0x00;

        flag = FALSE; //标志，等待下次按下按键后可以发送按下的数据
    }
}
/***********************CarSure_ID*****************************/
void Sure_InfoChange(void) //3-5按键
{
    static bool flag = FALSE;

    if((ADCKeyFlag>>24)&(0x10))
    {
        if(flag == FALSE)
        {
            CarSureData[3] = 0x01;
            Appliction_CanData_Init(&CarSure_MSG, CarSureData, 8, CarSure_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarSure_MSG);//将要发送的数据放到发送FIFO里等待发送

            flag = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象           
        }
    }
    else
    {
//        CarSureData[3] = 0x00;
        
        flag = FALSE; //标志，等待下次按下按键后可以发送按下的数据
    }
}
/***********************CarBack_ID*****************************/
void Back_InfoChange(void) //3-1按键
{
    static bool flag = FALSE;

    if((ADCKeyFlag>>24)&(0x01))
    {
        if(flag == FALSE)
        {
            CarBackData[3] = 0x01;
            Appliction_CanData_Init(&CarBack_MSG, CarBackData, 8, CarBack_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarBack_MSG);//将要发送的数据放到发送FIFO里等待发送

            flag = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象           
        }
    }
    else
    {
//        CarBackData[3] = 0x00;
        
        flag = FALSE; //标志，等待下次按下按键后可以发送按下的数据
    }
}
/***********************CarHome_ID*****************************/
void Home_InfoChange(void) //3-2按键
{
    static bool flag = FALSE;

    if((ADCKeyFlag>>24)&(0x02))
    {
        if(flag == FALSE)
        {
            CarHomeData[3] = 0x01;
            Appliction_CanData_Init(&CarHome_MSG, CarHomeData, 8, CarHome_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarHome_MSG);//将要发送的数据放到发送FIFO里等待发送

            flag = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象           
        }
    }
    else
    {
//        CarHomeData[3] = 0x00;
        
        flag = FALSE; //标志，等待下次按下按键后可以发送按下的数据
    }   
}
/***********************CarUp_ID*****************************/
void Up_InfoChange(void) //2-5按键
{
    static bool flag = FALSE;

    if((ADCKeyFlag>>8)&(0x10))
    {
        if(flag == FALSE)
        {
            CarUpData[3] = 0x01;
            Appliction_CanData_Init(&CarUp_MSG, CarUpData, 8, CarUp_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarUp_MSG);//将要发送的数据放到发送FIFO里等待发送
                        
            flag = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象           
        }
    }
    else
    {
//        CarUpData[3] = 0x00;

        flag = FALSE; //标志，等待下次按下按键后可以发送按下的数据
    }   
}
/***********************CarDown_ID*****************************/
void Down_InfoChange(void) //4-5按键
{
    static bool flag = FALSE;

    if((ADCKeyFlag)&(0x10))
    {
        if(flag == FALSE)
        {
            CarDownData[3] = 0x05;
            Appliction_CanData_Init(&CarDown_MSG, CarDownData, 8, CarDown_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarDown_MSG);//将要发送的数据放到发送FIFO里等待发送

            flag = TRUE; //标志，不再重复发送按下的数据，否则会出现按下一次却执行几次的现象           
        }
    }
    else
    {
//        CarDownData[3] = 0x00;

        flag = FALSE; //标志，等待下次按下按键后可以发送按下的数据
    }   
}

//自锁开关：
/***********************ACC*****************************/
void ACC_InfoChange(void)
{
	static uint32_t InternalTime;
	static uint32_t ACCCloseTime;
        
	if(ReadUserTimer(&InternalTime) > T_100MS*5)//间隔发送
	{		
		ResetUserTimer(&InternalTime);		    //重设时间
		if(IsKeyPutDowm(KeyACC))
		{
			CarACCData[2]=0x03;
            
            ACCFirstFlag = TRUE; //不开始计算ACC关闭时间	 
            ACCSecondFlag = TRUE; //允许发送CAN数据
		}
		else
		{
			CarACCData[2]=0x00;
            
            if((ACCFirstFlag==TRUE) && (ACCSecondFlag==TRUE)) 
            {
                ResetUserTimer(&ACCCloseTime);	//开始计算ACC关闭时间	   
                ACCFirstFlag = FALSE; //设立条件，不让计时重设
            }  
            if(ReadUserTimer(&ACCCloseTime) > 90000) //90S后不允许发送CAN数据
            {
                ACCSecondFlag = FALSE; //不允许发送CAN数据
            }

		}
        Appliction_CanData_Init(&CarACC_MSG, CarACCData, 8, CarACC_ID, CAN_ID_STD);   
//        ADD_data_to_CANxTxFifo(CAN_3, CarACC_MSG);//将要发送的数据放到发送FIFO里等待发送
	}	
}

/***********************Radar*****************************/
void Radar_InfoChange(void)
{
	static uint32_t InternalTime;
    
    if(ReadUserTimer(&InternalTime)>T_100MS*5)//间隔发送
    {		
        ResetUserTimer(&InternalTime);		  //重设时间
        if(IsKeyPutDowm(KeyBT))
        {
            CarRadar1Data[2]=0x01; 
 
            CarRadar2Data[2]=0x20;                        
            CarRadar2Data[3]=0x20;
            CarRadar2Data[4]=0x20;
            CarRadar2Data[5]=0x20;

            CarRadar3Data[2]=0x20;            
            CarRadar3Data[3]=0x20;
            CarRadar3Data[4]=0x20;
            CarRadar3Data[5]=0x20;
        }
        else
        {
            CarRadar1Data[2]=0x00;
 
            CarRadar2Data[2]=0xFF;            
            CarRadar2Data[3]=0xFF;
            CarRadar2Data[4]=0xFF;
            CarRadar2Data[5]=0xFF;

            CarRadar3Data[2]=0xFF;            
            CarRadar3Data[3]=0xFF;
            CarRadar3Data[4]=0xFF;
            CarRadar3Data[5]=0xFF;
        }
      
        Appliction_CanData_Init(&CarRadar1_MSG, CarRadar1Data, 4, CarRADAR1_ID, CAN_ID_EXT);   
//        ADD_data_to_CANxTxFifo(CAN_3, CarRadar1_MSG);//将要发送的数据放到发送FIFO里等待发送

        Appliction_CanData_Init(&CarRadar2_MSG, CarRadar2Data, 8, CarRADAR2_ID, CAN_ID_EXT);   
//        ADD_data_to_CANxTxFifo(CAN_3, CarRadar2_MSG);//将要发送的数据放到发送FIFO里等待发送

        Appliction_CanData_Init(&CarRadar3_MSG, CarRadar3Data, 8, CarRADAR3_ID, CAN_ID_EXT);   
//        ADD_data_to_CANxTxFifo(CAN_3, CarRadar3_MSG);//将要发送的数据放到发送FIFO里等待发送
    }
}

/***********************Standby*****************************/
void Standby_InfoChange(void)
{
	static uint32_t InternalTime;
        
	if(ReadUserTimer(&InternalTime) > T_100MS*5)//间隔发送
	{		
		ResetUserTimer(&InternalTime);		    //重设时间
		if(IsKeyPutDowm(KeyStandby))
		{
            StandbyFlag = TRUE; //不允许发送CAN数据
		}
		else
		{
            StandbyFlag = FALSE; //允许发送CAN数据
		}
	}	
}
/***********************CarSpeed_ID*****************************/
void Speed_InfoChange(void)
{
	static uint32_t InternalTime;
        
	if(ReadUserTimer(&InternalTime) > T_100MS*5)//间隔发送
	{		
		ResetUserTimer(&InternalTime);		    //重设时间
        
		if(IsKeyPutDowm(KeyREV))
		{
            CarSpeedData[3] = 0x50;
		}
		else
		{
            CarSpeedData[3] = 0x00;  
		}
        
        Appliction_CanData_Init(&CarSpeed_MSG, CarSpeedData, 8, CarSpeed_ID, CAN_ID_STD);   
//        ADD_data_to_CANxTxFifo(CAN_3, CarSpeed_MSG);//将要发送的数据放到发送FIFO里等待发送
	}	
}
/***********************CarTurn_ID*****************************/
void Turn_InfoChange(void)
{
	static uint32_t InternalTime;
        
	if(ReadUserTimer(&InternalTime) > T_100MS*5)//间隔发送
	{		
		ResetUserTimer(&InternalTime);		    //重设时间
        
		if(IsKeyPutDowm(KeyILL))
		{
            CarTurnData[2] = 0x00;  
            CarTurnData[3] = 0x01; //RTurn
		}
		else
		{
            CarTurnData[2] = 0x00;  
            CarTurnData[3] = 0x00; //TurnOFF
		}
        
        Appliction_CanData_Init(&CarTurn_MSG, CarTurnData, 8, CarTurn_ID, CAN_ID_STD);   
//        ADD_data_to_CANxTxFifo(CAN_3, CarTurn_MSG);//将要发送的数据放到发送FIFO里等待发送
	}	
}

///***********************CAN1 发送一组固定数据*****************************/
//void CAN1Keep_InfoChange(void) 
//{
//    Appliction_CanData_Init(&CarCAN1_MSG, CarCAN1Data, 8, CarCAN1_ID, CAN_ID_STD);//CAN1发送一组固定数据   
//    ADD_data_to_CANxTxFifo(CAN_1, CarCAN1_MSG);//将要发送的数据放到发送FIFO里等待发送
//}
///***********************CAN2 发送一组固定数据*****************************/
//void CAN2Keep_InfoChange(void) 
//{
//    Appliction_CanData_Init(&CarCAN2_MSG, CarCAN2Data, 8, CarCAN2_ID, CAN_ID_STD);//CAN2发送一组固定数据   
//    ADD_data_to_CANxTxFifo(CAN_2, CarCAN2_MSG);//将要发送的数据放到发送FIFO里等待发送
//}

/***********************ADD_Data_To_CANxTxFifo_APP*****************************/
void ADD_Data_To_CANxTxFifo_APP(uint8_t CAN_N)
{
	static uint32_t InternalCAN1Time;
	static uint32_t InternalCAN2Time;
	static uint32_t InternalCAN3Time;

    switch(CAN_N)
    { 
        case CAN_1: //添加数据到CAN1 TxFIFO
            if(ReadUserTimer(&InternalCAN1Time) > T_100MS*2)//间隔200ms发送
            {		
                ADD_data_to_CANxTxFifo(CAN_1, CarCAN1_MSG);//CAN1发送一组固定数据  

                ResetUserTimer(&InternalCAN1Time);		  
            }	
            break;
        case CAN_2: //添加数据到CAN2 TxFIFO
            if(ReadUserTimer(&InternalCAN2Time) > T_100MS*2)//间隔200ms发送
            {		
                ADD_data_to_CANxTxFifo(CAN_2, CarCAN2_MSG);//CAN2发送一组固定数据  
                
                ResetUserTimer(&InternalCAN2Time);		  
            }	
            break;
        case CAN_3: //添加数据到CAN3 TxFIFO
            if(ReadUserTimer(&InternalCAN3Time) > T_100MS*2)//间隔200ms发送
            {	
                ADD_data_to_CANxTxFifo(CAN_3, CarGear_MSG);
                ADD_data_to_CANxTxFifo(CAN_3, CarAngle_MSG);
                ADD_data_to_CANxTxFifo(CAN_3, CarTime1_MSG);
                ADD_data_to_CANxTxFifo(CAN_3, CarTime2_MSG);
                ADD_data_to_CANxTxFifo(CAN_3, CarACC_MSG);
                ADD_data_to_CANxTxFifo(CAN_3, CarTurn_MSG);
                ADD_data_to_CANxTxFifo(CAN_3, CarSpeed_MSG);
                ADD_data_to_CANxTxFifo(CAN_3, CarRadar1_MSG);
                ADD_data_to_CANxTxFifo(CAN_3, CarRadar2_MSG);
                ADD_data_to_CANxTxFifo(CAN_3, CarRadar3_MSG);
                
                ResetUserTimer(&InternalCAN3Time);		  
            }
            break;
        default:
            break;
    }		  
}

/***********************CAN_Info_Init：默认的一些数据放在这里初始化*****************************/
void CAN_Info_Init(void) 
{
    Appliction_CanData_Init(&CarCAN1_MSG, CarCAN1Data, 8, CarCAN1_ID, CAN_ID_STD);//CAN1发送一组固定数据   
    Appliction_CanData_Init(&CarCAN2_MSG, CarCAN2Data, 8, CarCAN2_ID, CAN_ID_STD);//CAN2发送一组固定数据   

    Appliction_CanData_Init(&CarTime1_MSG, CarTime1Data, 8, CarTime1_ID, CAN_ID_EXT);//同步时间
    Appliction_CanData_Init(&CarTime2_MSG, CarTime2Data, 8, CarTime2_ID, CAN_ID_EXT);//同步时间

    Appliction_CanData_Init(&CarGear_MSG, CarGearData, 8, CarGear_ID, CAN_ID_STD);//默认P档
}


