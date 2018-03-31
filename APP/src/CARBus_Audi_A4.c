#include "CARBus_Audi_A4.h"

/*****�궨��ID���������顢����CAN�ṹ��*****/
/*****�궨��ID***********/
//�ᴥ������
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
#define CarTime1_ID     0x17331110 //ͬ��ʱ�䣺ʱ-��-��
#define CarTime2_ID     0x17331110 //ͬ��ʱ�䣺��-��-��
//�������أ�
#define CarACC_ID       0x3C0  
#define CarSpeed_ID     0x30B  
#define CarTurn_ID      0x366  
#define CarRADAR1_ID    0x17330A10  
#define CarRADAR2_ID    0x17330A10  
#define CarRADAR3_ID    0x17330A10  

//�̶���Ϣ����֤CAN1/CAN2������:
#define CarCAN1_ID      0x01
#define CarCAN2_ID      0x02    


/******��������**********/
//�ᴥ������
uint8_t CarEKeyData[8]   =  {0x05,0x48,0x03,0x00,0x00,0x00,0xAA,0xAA};//��ť
uint8_t CarLeftData[8]   =  {0x03,0x38,0x0F,0x00,0x00,0xAA,0xAA,0xAA};//���
uint8_t CarRightData[8]  =  {0x03,0x38,0x5C,0x00,0x00,0xAA,0xAA,0xAA};//�Ҽ�
uint8_t CarSureData[8]   =  {0x04,0x30,0x01,0x00,0x00,0xAA,0xAA,0xAA};//ȷ����
uint8_t CarBackData[8]   =  {0x04,0x30,0x0F,0x00,0x00,0xAA,0xAA,0xAA};//���ؼ�
uint8_t CarHomeData[8]   =  {0x04,0x30,0x1A,0x00,0x00,0xAA,0xAA,0xAA};//��ҳ��
uint8_t CarUpData[8]     =  {0x03,0x38,0x0A,0x00,0x00,0xAA,0xAA,0xAA};//�ϼ�
uint8_t CarDownData[8]   =  {0x03,0x38,0x0B,0x00,0x00,0xAA,0xAA,0xAA};//�¼�
uint8_t CarGearData[8]   =  {0X3E,0X09,0X97,0X51,0XC8,0X00,0X00,0X80};//��λ
uint8_t CarAngleData[8]  =  {0xD5,0x0C,0x00,0x12,0x00,0xAA,0x0C,0x00};//������
uint8_t CarTime1Data[8]  =  {0x44,0x51,0x0A,0x0A,0x0A,0x00,0x00,0x00};//ʱ�䣺ʱ-��-��
uint8_t CarTime2Data[8]  =  {0x44,0x50,0x11,0x08,0x09,0x00,0x00,0x00};//ʱ�䣺��-��-��
//�������أ�
uint8_t CarACCData[4]    =  {0x92,0x0f,0x00,0x00};//ACC
uint8_t CarSpeedData[8]  =  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//Speed
uint8_t CarTurnData[8]   =  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//Turn
uint8_t CarRadar1Data[4] =  {0x52,0x95,0x00,0x00};//Radar1	
uint8_t CarRadar2Data[8] =  {0x42,0x93,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//Radar2	
uint8_t CarRadar3Data[8] =  {0x42,0x92,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//Radar3	
//�̶���Ϣ����֤CAN1/CAN2������:
uint8_t CarCAN1Data[8] =  {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//CAN1 Channel
uint8_t CarCAN2Data[8] =  {0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//CAN2 Channel


/******����CAN�ṹ��**********/
//�ᴥ������
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
//�������أ�
CAN_MSG_Type CarACC_MSG;
CAN_MSG_Type CarSpeed_MSG;
CAN_MSG_Type CarTurn_MSG;
CAN_MSG_Type CarRadar1_MSG;
CAN_MSG_Type CarRadar2_MSG;
CAN_MSG_Type CarRadar3_MSG;
//�̶���Ϣ����֤CAN1/CAN2������:
CAN_MSG_Type CarCAN1_MSG;
CAN_MSG_Type CarCAN2_MSG;



//���ߺʹ�������
Bool ACCFirstFlag = TRUE;
Bool ACCSecondFlag = TRUE;
Bool StandbyFlag = TRUE;


/*****************************************************************************
**��������:    ADCKeyInfoSend
**��������:	   �ᴥ�������Ӧ�ú���
**��ڲ���:   
**���ز���:
******************************************************************************/
void ADCKey_InfoChange(void)
{
    Gear_InfoChange();//��λ
    Angle_InfoChange();//�����̽Ƕ�
    Left_InfoChange();//���
    Right_InfoChange();//�Ҽ�
    Sure_InfoChange();//ȷ����
    Back_InfoChange();//���ؼ�
    Home_InfoChange();//��ҳ��
    Up_InfoChange(); //�ϼ�
    Down_InfoChange(); //�¼�
}
/*****************************************************************************
**��������:    KeyInfoSend
**��������:	   �������ؼ��Ӧ�ú���
**��ڲ���:   
**���ز���:
******************************************************************************/
void Key_InfoChange(void)
{
    ACC_InfoChange();//ACC
    Turn_InfoChange();//ת���
    Speed_InfoChange();//����
    Standby_InfoChange();//����
    Radar_InfoChange();//�״�


}

/*******************************CarEKey_ID*******************************************
**��������:	 EKeyInfoSend
**��������:	 ��ⰴ��EK
**��ڲ���:
**���ز���:
******************************************************************************/
void EKey_InfoChange()
{
	static uint8_t flag = 0;		
	static uint8_t ek1_step = 1;
	static uint8_t ek2_step = 1;
	static uint32_t ekIntervalTime;
	static uint8_t left = 0xff,right = 0x1;
    
	if(ek1_step != (GPIO_ReadValue(0)&(0x300))>>8) //�������ת
	{	
        ek2_step = ek1_step; //��ŵ�ǰ״̬
		ek1_step = (GPIO_ReadValue(0)&(0x300))>>8; //��ȡ����״̬
        
		/********************�Ƚ�����״̬�ó�������������********************/
        if(((ek2_step == 3)&&(ek1_step == 1))||((ek2_step == 0)&&(ek1_step == 2))) //����
		{
			if(flag != 2)
			{
				CarEKeyData[3] = left--; //����һ�¼�1	                
			}
			flag=1; //��־λ
		}
		else if(((ek2_step == 3)&&(ek1_step == 2))||((ek2_step == 0)&&(ek1_step == 1))) //����
		{
			if(flag != 1)
			{
				CarEKeyData[3] = right++; //����һ�¼�һ 
			}
			flag = 2; //��־λ
		}	
	}
    
	if(ReadUserTimer(&ekIntervalTime) > T_1MS*200) //���200ms��û����һ����ת
	{
		ResetUserTimer(&ekIntervalTime);
		if((flag == 1)||(flag == 2))
		{	
			flag = 0;	
			left = 0xff;
            right = 0x1;
            
            Appliction_CanData_Init(&CarEKey_MSG, CarEKeyData, 8, CarEKey_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarEKey_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
		}
	}
}

/***********************CarGear_ID*****************************/
void Gear_InfoChange(void) //1-1��1-2��1-3��1-4����
{          
    static bool PGear = FALSE;
    static bool RGear = FALSE;
    static bool NGear = FALSE;
    static bool DGear = FALSE;

    /* 1-1 ��������  P�� */
    if((ADCKeyFlag>>16)&(0x01))
    {
        if(PGear == FALSE)
        {
            CarGearData[2] = 0X97;
            
            Appliction_CanData_Init(&CarGear_MSG, CarGearData, 8, CarGear_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarGear_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
            
            PGear = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����  
        }
    }
    /* 1-2 ��������  R�� */
    else if((ADCKeyFlag>>16)&(0x02))
    {
        if(RGear == FALSE)
        {
            CarGearData[2] = 0XD7;
            
            Appliction_CanData_Init(&CarGear_MSG, CarGearData, 8, CarGear_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarGear_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
            
            RGear = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����  
        }
    }
    /* 1-3 ��������  N�� */
    else if((ADCKeyFlag>>16)&(0x04))
    {
        if(NGear == FALSE)
        {
            CarGearData[2] = 0X07;
            
            Appliction_CanData_Init(&CarGear_MSG, CarGearData, 8, CarGear_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarGear_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
            
            NGear = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����  
        }
    }
    /* 1-4 ��������  D�� */
    else if((ADCKeyFlag>>16)&(0x08))
    {
        if(DGear == FALSE)
        {
            CarGearData[2] = 0XB7;
            
            Appliction_CanData_Init(&CarGear_MSG, CarGearData, 8, CarGear_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarGear_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
            
            DGear = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����  
        }
    }
    else //û�а��������£������ñ�־���ȴ��´ΰ��°�������Է��Ͱ��µ�����
    {
        PGear = FALSE;
        RGear = FALSE;
        NGear = FALSE;
        DGear = FALSE;
    }   

}

/***********************CarAngle_ID***************************************/
void Angle_InfoChange(void) //1-5��1-6����
{      
    static bool angleLeftFlag = FALSE;
    static bool angleRightFlag = FALSE;
    
    /* 1-5 ��������  ��������ת */
    if((ADCKeyFlag>>16)&(0x10))
    {
        if(angleLeftFlag == FALSE)
        {
            CarAngleData[3] -= 0x20; //�Ƕ�-
            CarAngleData[2] = CarAngleData[3]; //��ֵ

            Appliction_CanData_Init(&CarAngle_MSG, CarAngleData, 8, CarAngle_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarAngle_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����  
                    
            angleLeftFlag = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����           
        }
    }
    /* 1-6 ��������  ��������ת  */
    else if((ADCKeyFlag>>16)&(0x20))
    {
        if(angleRightFlag == FALSE)
        {

            CarAngleData[3] += 0x20; //�Ƕ�+
            CarAngleData[2] = CarAngleData[3]; //��ֵ

            Appliction_CanData_Init(&CarAngle_MSG, CarAngleData, 8, CarAngle_ID, CAN_ID_STD);   
            ADD_data_to_CANxTxFifo(CAN_3, CarAngle_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����  
        
            angleRightFlag = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����           
        }
    }
     /* û�з����̰�������*/   
    else
    {
        angleLeftFlag = FALSE; //��־���ȴ��´ΰ��°�������Է��Ͱ��µ�����
        angleRightFlag = FALSE; //��־���ȴ��´ΰ��°�������Է��Ͱ��µ�����
    }   
}
/***********************CarLeft_ID*****************************/
void Left_InfoChange(void) //3-4����
{
    static bool flag = FALSE;

    if((ADCKeyFlag>>24)&(0x08))
    {
        if(flag == FALSE)
        {
            CarLeftData[3] = 0x07;
            Appliction_CanData_Init(&CarLeft_MSG, CarLeftData, 8, CarLeft_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarLeft_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
                       
            flag = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����           
        }            
    }
    else
    {
//        CarLeftData[3] = 0x00;
        
        flag = FALSE; //��־���ȴ��´ΰ��°�������Է��Ͱ��µ�����
    }
}
/***********************CarRight_ID*****************************/
void Right_InfoChange(void) //3-6����
{
    static bool flag = FALSE;
    
    if((ADCKeyFlag>>24)&(0x20))
    {
        if(flag == FALSE)
        {
            CarRightData[3] = 0x03;
            Appliction_CanData_Init(&CarRight_MSG, CarRightData, 8, CarRight_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarRight_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
            
            flag = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����           
        }
    }
    else
    {
//        CarRightData[3] = 0x00;

        flag = FALSE; //��־���ȴ��´ΰ��°�������Է��Ͱ��µ�����
    }
}
/***********************CarSure_ID*****************************/
void Sure_InfoChange(void) //3-5����
{
    static bool flag = FALSE;

    if((ADCKeyFlag>>24)&(0x10))
    {
        if(flag == FALSE)
        {
            CarSureData[3] = 0x01;
            Appliction_CanData_Init(&CarSure_MSG, CarSureData, 8, CarSure_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarSure_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����

            flag = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����           
        }
    }
    else
    {
//        CarSureData[3] = 0x00;
        
        flag = FALSE; //��־���ȴ��´ΰ��°�������Է��Ͱ��µ�����
    }
}
/***********************CarBack_ID*****************************/
void Back_InfoChange(void) //3-1����
{
    static bool flag = FALSE;

    if((ADCKeyFlag>>24)&(0x01))
    {
        if(flag == FALSE)
        {
            CarBackData[3] = 0x01;
            Appliction_CanData_Init(&CarBack_MSG, CarBackData, 8, CarBack_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarBack_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����

            flag = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����           
        }
    }
    else
    {
//        CarBackData[3] = 0x00;
        
        flag = FALSE; //��־���ȴ��´ΰ��°�������Է��Ͱ��µ�����
    }
}
/***********************CarHome_ID*****************************/
void Home_InfoChange(void) //3-2����
{
    static bool flag = FALSE;

    if((ADCKeyFlag>>24)&(0x02))
    {
        if(flag == FALSE)
        {
            CarHomeData[3] = 0x01;
            Appliction_CanData_Init(&CarHome_MSG, CarHomeData, 8, CarHome_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarHome_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����

            flag = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����           
        }
    }
    else
    {
//        CarHomeData[3] = 0x00;
        
        flag = FALSE; //��־���ȴ��´ΰ��°�������Է��Ͱ��µ�����
    }   
}
/***********************CarUp_ID*****************************/
void Up_InfoChange(void) //2-5����
{
    static bool flag = FALSE;

    if((ADCKeyFlag>>8)&(0x10))
    {
        if(flag == FALSE)
        {
            CarUpData[3] = 0x01;
            Appliction_CanData_Init(&CarUp_MSG, CarUpData, 8, CarUp_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarUp_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
                        
            flag = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����           
        }
    }
    else
    {
//        CarUpData[3] = 0x00;

        flag = FALSE; //��־���ȴ��´ΰ��°�������Է��Ͱ��µ�����
    }   
}
/***********************CarDown_ID*****************************/
void Down_InfoChange(void) //4-5����
{
    static bool flag = FALSE;

    if((ADCKeyFlag)&(0x10))
    {
        if(flag == FALSE)
        {
            CarDownData[3] = 0x05;
            Appliction_CanData_Init(&CarDown_MSG, CarDownData, 8, CarDown_ID, CAN_ID_EXT);   
            ADD_data_to_CANxTxFifo(CAN_2, CarDown_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����

            flag = TRUE; //��־�������ظ����Ͱ��µ����ݣ��������ְ���һ��ȴִ�м��ε�����           
        }
    }
    else
    {
//        CarDownData[3] = 0x00;

        flag = FALSE; //��־���ȴ��´ΰ��°�������Է��Ͱ��µ�����
    }   
}

//�������أ�
/***********************ACC*****************************/
void ACC_InfoChange(void)
{
	static uint32_t InternalTime;
	static uint32_t ACCCloseTime;
        
	if(ReadUserTimer(&InternalTime) > T_100MS*5)//�������
	{		
		ResetUserTimer(&InternalTime);		    //����ʱ��
		if(IsKeyPutDowm(KeyACC))
		{
			CarACCData[2]=0x03;
            
            ACCFirstFlag = TRUE; //����ʼ����ACC�ر�ʱ��	 
            ACCSecondFlag = TRUE; //������CAN����
		}
		else
		{
			CarACCData[2]=0x00;
            
            if((ACCFirstFlag==TRUE) && (ACCSecondFlag==TRUE)) 
            {
                ResetUserTimer(&ACCCloseTime);	//��ʼ����ACC�ر�ʱ��	   
                ACCFirstFlag = FALSE; //�������������ü�ʱ����
            }  
            if(ReadUserTimer(&ACCCloseTime) > 90000) //90S��������CAN����
            {
                ACCSecondFlag = FALSE; //��������CAN����
            }

		}
        Appliction_CanData_Init(&CarACC_MSG, CarACCData, 8, CarACC_ID, CAN_ID_STD);   
//        ADD_data_to_CANxTxFifo(CAN_3, CarACC_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
	}	
}

/***********************Radar*****************************/
void Radar_InfoChange(void)
{
	static uint32_t InternalTime;
    
    if(ReadUserTimer(&InternalTime)>T_100MS*5)//�������
    {		
        ResetUserTimer(&InternalTime);		  //����ʱ��
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
//        ADD_data_to_CANxTxFifo(CAN_3, CarRadar1_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����

        Appliction_CanData_Init(&CarRadar2_MSG, CarRadar2Data, 8, CarRADAR2_ID, CAN_ID_EXT);   
//        ADD_data_to_CANxTxFifo(CAN_3, CarRadar2_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����

        Appliction_CanData_Init(&CarRadar3_MSG, CarRadar3Data, 8, CarRADAR3_ID, CAN_ID_EXT);   
//        ADD_data_to_CANxTxFifo(CAN_3, CarRadar3_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
    }
}

/***********************Standby*****************************/
void Standby_InfoChange(void)
{
	static uint32_t InternalTime;
        
	if(ReadUserTimer(&InternalTime) > T_100MS*5)//�������
	{		
		ResetUserTimer(&InternalTime);		    //����ʱ��
		if(IsKeyPutDowm(KeyStandby))
		{
            StandbyFlag = TRUE; //��������CAN����
		}
		else
		{
            StandbyFlag = FALSE; //������CAN����
		}
	}	
}
/***********************CarSpeed_ID*****************************/
void Speed_InfoChange(void)
{
	static uint32_t InternalTime;
        
	if(ReadUserTimer(&InternalTime) > T_100MS*5)//�������
	{		
		ResetUserTimer(&InternalTime);		    //����ʱ��
        
		if(IsKeyPutDowm(KeyREV))
		{
            CarSpeedData[3] = 0x50;
		}
		else
		{
            CarSpeedData[3] = 0x00;  
		}
        
        Appliction_CanData_Init(&CarSpeed_MSG, CarSpeedData, 8, CarSpeed_ID, CAN_ID_STD);   
//        ADD_data_to_CANxTxFifo(CAN_3, CarSpeed_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
	}	
}
/***********************CarTurn_ID*****************************/
void Turn_InfoChange(void)
{
	static uint32_t InternalTime;
        
	if(ReadUserTimer(&InternalTime) > T_100MS*5)//�������
	{		
		ResetUserTimer(&InternalTime);		    //����ʱ��
        
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
//        ADD_data_to_CANxTxFifo(CAN_3, CarTurn_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
	}	
}

///***********************CAN1 ����һ��̶�����*****************************/
//void CAN1Keep_InfoChange(void) 
//{
//    Appliction_CanData_Init(&CarCAN1_MSG, CarCAN1Data, 8, CarCAN1_ID, CAN_ID_STD);//CAN1����һ��̶�����   
//    ADD_data_to_CANxTxFifo(CAN_1, CarCAN1_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
//}
///***********************CAN2 ����һ��̶�����*****************************/
//void CAN2Keep_InfoChange(void) 
//{
//    Appliction_CanData_Init(&CarCAN2_MSG, CarCAN2Data, 8, CarCAN2_ID, CAN_ID_STD);//CAN2����һ��̶�����   
//    ADD_data_to_CANxTxFifo(CAN_2, CarCAN2_MSG);//��Ҫ���͵����ݷŵ�����FIFO��ȴ�����
//}

/***********************ADD_Data_To_CANxTxFifo_APP*****************************/
void ADD_Data_To_CANxTxFifo_APP(uint8_t CAN_N)
{
	static uint32_t InternalCAN1Time;
	static uint32_t InternalCAN2Time;
	static uint32_t InternalCAN3Time;

    switch(CAN_N)
    { 
        case CAN_1: //������ݵ�CAN1 TxFIFO
            if(ReadUserTimer(&InternalCAN1Time) > T_100MS*2)//���200ms����
            {		
                ADD_data_to_CANxTxFifo(CAN_1, CarCAN1_MSG);//CAN1����һ��̶�����  

                ResetUserTimer(&InternalCAN1Time);		  
            }	
            break;
        case CAN_2: //������ݵ�CAN2 TxFIFO
            if(ReadUserTimer(&InternalCAN2Time) > T_100MS*2)//���200ms����
            {		
                ADD_data_to_CANxTxFifo(CAN_2, CarCAN2_MSG);//CAN2����һ��̶�����  
                
                ResetUserTimer(&InternalCAN2Time);		  
            }	
            break;
        case CAN_3: //������ݵ�CAN3 TxFIFO
            if(ReadUserTimer(&InternalCAN3Time) > T_100MS*2)//���200ms����
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

/***********************CAN_Info_Init��Ĭ�ϵ�һЩ���ݷ��������ʼ��*****************************/
void CAN_Info_Init(void) 
{
    Appliction_CanData_Init(&CarCAN1_MSG, CarCAN1Data, 8, CarCAN1_ID, CAN_ID_STD);//CAN1����һ��̶�����   
    Appliction_CanData_Init(&CarCAN2_MSG, CarCAN2Data, 8, CarCAN2_ID, CAN_ID_STD);//CAN2����һ��̶�����   

    Appliction_CanData_Init(&CarTime1_MSG, CarTime1Data, 8, CarTime1_ID, CAN_ID_EXT);//ͬ��ʱ��
    Appliction_CanData_Init(&CarTime2_MSG, CarTime2Data, 8, CarTime2_ID, CAN_ID_EXT);//ͬ��ʱ��

    Appliction_CanData_Init(&CarGear_MSG, CarGearData, 8, CarGear_ID, CAN_ID_STD);//Ĭ��P��
}


