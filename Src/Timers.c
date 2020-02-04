#include "Timers.h"
extern volatile uint32_t timer_ms;

uint16_t TIM_ini(int32_t Fshim)
{ 
	NVIC_InitTypeDef myNVIC;
	GPIO_InitTypeDef myGPIO; 
	uint16_t Dmax;
	//TIM1_Вызов функции управления

    RCC->APB2ENR|=RCC_APB2ENR_TIM1EN;
    TIM1->SMCR &= ~ TIM_SMCR_SMS;
    TIM1->PSC = 2 - 1;//6MHz // (частота тактирования У нас 72000000)/(PSC+1) = Частота счёта таймера 
    TIM1->ARR =72000000/((TIM1->PSC)+1)/(80000);//5кHz; 1200 точек на период пилы;    //Period предел счета таймера досчитав до ARR будет вызываться прерывание в нашем случае 
    TIM1->CR1 |= TIM_CR1_CEN;
    TIM1->DIER |=TIM_DIER_UIE;
    NVIC_EnableIRQ (TIM1_UP_IRQn);
           /* NVIC Configuration */
    /* Enable the TIM3_IRQn Interrupt */
    myNVIC.NVIC_IRQChannel = TIM1_UP_IRQn;
    myNVIC.NVIC_IRQChannelPreemptionPriority = 1;
    myNVIC.NVIC_IRQChannelSubPriority = 0;
    myNVIC.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&myNVIC);	
	
	//TIM4_PWM_for_1/2_channel-------------------------------------------------------------------------------------------------------------------------
    RCC->APB1ENR|=RCC_APB1ENR_TIM4EN; //Активация тактирования TIM4 в enable register
    TIM4->PSC = 1 - 1;//72MHz // (частота тактирования У нас 72000000)/(1) = Частота счёта таймера 
	  Dmax = ( clk_TIM4/(Fshim*(TIM4->PSC+1)) ); //Предел счета таймера ИЛИ Предельное занчение коэфициента заполнения 
    if(Dmax>65535)Dmax=65535;
	  TIM4->ARR = Dmax; ////2kHz     //Period предел счета таймера. Досчитав до ARR будет вызываться прерывание. 
      
	//1 канал TIM4
	  RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;//Активация тактирования GPIOB в enable register 
	  myGPIO.GPIO_Pin   = GPIO_Pin_6; //первый канал таймера
	  myGPIO.GPIO_Mode  = GPIO_Mode_AF_PP;
	  myGPIO.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIOB,&myGPIO);
	  TIM4->CCER  |=TIM_CCER_CC1E; //Разрешаем таймеру использовать первый канал( ножка PB6) в регистре TIMx_CCER capture/compare enable register
	  TIM4->CCMR1 |=TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2; //Включаем на первом канале обртный ШИМ    УПРАВЛЕНИЕ ДРЙВЕРАМИ НУЛЕМ
      
    //2 канал TIM4
    myGPIO.GPIO_Pin   = GPIO_Pin_7; //второй канал таймера
	  myGPIO.GPIO_Mode  = GPIO_Mode_AF_PP;
	  myGPIO.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIOB,&myGPIO);
	  TIM4->CCER  |=TIM_CCER_CC2E; //Разрешаем таймеру использовать 2 канал( ножка PB7) в регистре TIMx_CCER capture/compare enable register
	  TIM4->CCMR1 |=TIM_CCMR1_OC2M_0|TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_2; //Включаем на первом канале прямой ШИМ    УПРАВЛЕНИЕ ДРЙВЕРАМИ НУЛЕМ

    TIM4->CCR1 = 0;
    TIM4->CCR2 = Dmax-0;
    
	//------------------------------------------------------------------------------------------------------------------------------TIM4_PWM_for_1/2_channel
  
  //TIM3_PWM_for_3/4_channel-------------------------------------------------------------------------------------------------------------------------
    RCC->APB1ENR|=RCC_APB1ENR_TIM3EN; //Активация тактирования TIM3 в enable register
    TIM3->PSC = 1 - 1;//72MHz // (частота тактирования У нас 72000000)/(1) = Частота счёта таймера 
	  //Dmax = ( clk_TIM3/(Fshim*(TIM4->PSC+1)) ); //Предел счета таймера ИЛИ Предельное занчение коэфициента заполнения 
    //if(Dmax>65535)Dmax=65535;
	  TIM3->ARR = Dmax; ////2kHz     //Period предел счета таймера. Досчитав до ARR будет вызываться прерывание. 
      
	//3 канал TIM3
	  RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;//Активация тактирования GPIOB в enable register 
	  myGPIO.GPIO_Pin   = GPIO_Pin_0; //первый канал таймера
	  myGPIO.GPIO_Mode  = GPIO_Mode_AF_PP;
	  myGPIO.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIOB,&myGPIO);
	  TIM3->CCER  |=TIM_CCER_CC3E; //Разрешаем таймеру использовать 3 канал( ножка PB6) в регистре TIMx_CCER capture/compare enable register
	  TIM3->CCMR2 |=TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3M_2; //Включаем на первом канале обртный ШИМ    УПРАВЛЕНИЕ ДРЙВЕРАМИ НУЛЕМ
      
    //4 канал TIM3
    myGPIO.GPIO_Pin   = GPIO_Pin_1; //второй канал таймера
	  myGPIO.GPIO_Mode  = GPIO_Mode_AF_PP;
	  myGPIO.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIOB,&myGPIO);
	  TIM3->CCER  |=TIM_CCER_CC4E; //Разрешаем таймеру использовать 2 канал( ножка PB7) в регистре TIMx_CCER capture/compare enable register
	  TIM3->CCMR2 |=TIM_CCMR2_OC4M_0|TIM_CCMR2_OC4M_1|TIM_CCMR2_OC4M_2; //Включаем на первом канале прямой ШИМ    УПРАВЛЕНИЕ ДРЙВЕРАМИ НУЛЕМ

    TIM3->CCR3 = 0;
    TIM3->CCR4 = Dmax-0;
	//------------------------------------------------------------------------------------------------------------------------------TIM3_PWM_for_3/4_channel

	//TIM2_PWM_for_5/6_channel-------------------------------------------------------------------------------------------------------------------------
    RCC->APB1ENR|=RCC_APB1ENR_TIM2EN; //Активация тактирования TIM2 в enable register
    TIM2->PSC = 1 - 1;//72MHz // (частота тактирования У нас 72000000)/(1) = Частота счёта таймера 
	  //Dmax = ( clk_TIM4/(Fshim*(TIM4->PSC+1)) ); //Предел счета таймера ИЛИ Предельное занчение коэфициента заполнения 
    //if(Dmax>65535)Dmax=65535;
	  TIM2->ARR = Dmax; ////2kHz     //Period предел счета таймера. Досчитав до ARR будет вызываться прерывание. 
      
	//1 канал TIM2
	  RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;//Активация тактирования GPIOB в enable register 
	  myGPIO.GPIO_Pin   = GPIO_Pin_0; //первый канал таймера
	  myGPIO.GPIO_Mode  = GPIO_Mode_AF_PP;
	  myGPIO.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIOA,&myGPIO);
	  TIM2->CCER  |=TIM_CCER_CC1E; //Разрешаем таймеру использовать первый канал( ножка PB6) в регистре TIMx_CCER capture/compare enable register
	  TIM2->CCMR1 |=TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2; //Включаем на первом канале обртный ШИМ    УПРАВЛЕНИЕ ДРЙВЕРАМИ НУЛЕМ
      
    //2 канал TIM2
      myGPIO.GPIO_Pin   = GPIO_Pin_1; //второй канал таймера
	  myGPIO.GPIO_Mode  = GPIO_Mode_AF_PP;
	  myGPIO.GPIO_Speed = GPIO_Speed_2MHz;
	  GPIO_Init(GPIOA,&myGPIO);
	  TIM2->CCER  |=TIM_CCER_CC2E; //Разрешаем таймеру использовать 2 канал( ножка PB7) в регистре TIMx_CCER capture/compare enable register
	  TIM2->CCMR1 |=TIM_CCMR1_OC2M_0|TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_2; //Включаем на первом канале прямой ШИМ    УПРАВЛЕНИЕ ДРЙВЕРАМИ НУЛЕМ

    TIM2->CCR1 = 0;
    TIM2->CCR2 = Dmax-0;
    
	//------------------------------------------------------------------------------------------------------------------------------TIM2_PWM_for_5/6_channel

	TIM4->CNT=0;
	TIM3->CNT=Dmax/3;
	TIM2->CNT=2*Dmax/3;
	timer_ms=0;
	while(timer_ms<=1000);

	TIM4->CR1|=1; //Активация таймера  
	TIM3->CR1|=1; //Активация таймера 
	TIM2->CR1|=1; //Активация таймера 
	return Dmax;
}

