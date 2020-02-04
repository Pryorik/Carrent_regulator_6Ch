#include "GPIO.h"

void GPIO_ini()
{
    rcc_ini();
    port_ini();
    CONTROL_DISABLE_Ch1_2
    CONTROL_DISABLE_Ch3_4
    CONTROL_DISABLE_Ch5_6
}

void rcc_ini(void)
{
	 //RCC->APB2ENR |= (1<<4); //Активация тактирования GPIOC в enable register
     RCC->APB2ENR|=(1<<3);   //Активация тактирования GPIOB в enable register 
	 RCC->APB2ENR |= (1<<2); //Активация тактирования GPIOA в enable register
}

void port_ini(void)
{
	GPIO_InitTypeDef myGPIO;
	
	    //LED
	myGPIO.GPIO_Pin = GPIO_Pin_13; // инициализция параметров ножки порта прводится в регистре CRH  configuration register high
    myGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &myGPIO);

        /*Channel_1/2_DisableControl_Active_HIGH*/
	myGPIO.GPIO_Pin = GPIO_Pin_5; // инициализция параметров ножки порта прводится в регистре CRH  configuration register high
    myGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &myGPIO);
    
        /*Channel_3/4_DisableControl_Active_HIGH*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST,ENABLE);
	myGPIO.GPIO_Pin = GPIO_Pin_4; // инициализция параметров ножки порта прводится в регистре CRH  configuration register high
    myGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &myGPIO);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
        /*Channel_5/6_DisableControl_Active_HIGH*/
	myGPIO.GPIO_Pin = GPIO_Pin_3; // инициализция параметров ножки порта прводится в регистре CRH  configuration register high
    myGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &myGPIO);
}