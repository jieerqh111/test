#include "bsp_gpio.h"
#include "lpc17xx_gpio.h"

uint32_t GPIOTickCnt = 0;

/*********************************************************************//**
 * @brief		GPIO_Init
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void GPIO_Init(void)   
{                       
    GPIO_SetDir(0, (1<<3), 1);           /* GPIO on PORT0 defined as Output    */
    GPIO_SetDir(2, (1<<2), 1);           /* GPIO on PORT2 defined as Output    */

    GPIO_ClearValue(0, (1<<3));
    GPIO_ClearValue(2, (1<<2));
}

/*********************************************************************//**
 * @brief		GPIO_TOLLGO
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void GPIO_Toggle(void)   
{                       
    GPIO_SetValue(0, (1<<3));
    Delay_ms(100); 
    GPIO_ClearValue(0, (1<<3));
    Delay_ms(100); 
}

/*********************************************************************//**
* @brief		GPIO_ReadPin: 读取GPIO的电平
 * @param[in]	None
 * @return 		None
 **********************************************************************/
uint32_t GPIO_ReadPinStatus(uint8_t Portnum)   
{   
    uint32_t pin_value = 0;
    uint32_t GPIOPinStatus = 0;

    if(ReadUserTimer(&GPIOTickCnt) > 100)
    {
        pin_value = GPIO_ReadValue(Portnum);//读取Portnum引脚的电平状态
        
        if(pin_value & (1<<0))
        {
            GPIOPinStatus |= 0x08;
        }
        if(pin_value & (1<<1))
        {
            GPIOPinStatus |= 0x80;
        }
        if(pin_value & (1<<4))
        {
            GPIOPinStatus |= 0x04;
        }
        if(pin_value & (1<<8))
        {
            GPIOPinStatus |= 0x40;
        }
        if(pin_value & (1<<9))
        {
            GPIOPinStatus |= 0x20;
        }
        if(pin_value & (1<<10))
        {
            GPIOPinStatus |= 0x02;
        }
        if(pin_value & (1<<14))
        {
            GPIOPinStatus |= 0x10;
        }
        if(pin_value & (1<<15))
        {
            GPIOPinStatus |= 0x01;
        }
        ResetUserTimer(&GPIOTickCnt);        
    }
    return GPIOPinStatus;
}


/*********************************************************************//**
* @brief		EvtGPIO_ReadPinStatus: 解析ACC等按键的状态
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EvtGPIO_ReadPinStatus(uint32_t  PinValue)   
{           
    if(PinValue & 0x01)
    {
        _DBG("ACC is down! \r\n");
    }
    if(PinValue & 0x02)
    {
        _DBG("ILL is down! \r\n");
    }
    if(PinValue & 0x04)
    {
        _DBG("REV is down! \r\n");
    }
    if(PinValue & 0x08)
    {
        _DBG("Standby is down! \r\n");
    }
    if(PinValue & 0x10)
    {
        _DBG("SCREEN is down! \r\n");
    }
    if(PinValue & 0x20)
    {
        _DBG("BT is down! \r\n");
    }
    if(PinValue & 0x40)
    {
        _DBG("LEFT_DOOR is down! \r\n");
    } 
    if(PinValue & 0x80)
    {
        _DBG("AIR_DISPLAY is down! \r\n");
    }
}


