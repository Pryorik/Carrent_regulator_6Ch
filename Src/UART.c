#include "UART.h"

//varible
int  RX_FLAG_END_LINE = 0;
char RX_BUF[RX_BUF_SIZE] = {}; 

void USART_ini(void)
{
	NVIC_InitTypeDef myNVIC;
    GPIO_InitTypeDef myGPIO; 
    USART_InitTypeDef myUSART;
	
	/* Set NVIC Priority Group */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //выбор группы приоритетов 

    /* Enable the USARTx Interrupt */
    myNVIC.NVIC_IRQChannel = USART3_IRQn; 
    myNVIC.NVIC_IRQChannelPreemptionPriority = 0; //Приоритет
    myNVIC.NVIC_IRQChannelSubPriority = 0; //Субприоритет
    myNVIC.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&myNVIC);
	  
    RCC->APB1ENR |= (1<<18); //Активация тактирования UART3 в enable register
	RCC->APB2ENR|=(1<<3);    //Активация тактирования GPIOB в enable register
	
    myGPIO.GPIO_Pin = GPIO_Pin_10; //TX
    myGPIO.GPIO_Mode = GPIO_Mode_AF_PP;
    myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&myGPIO);
    myGPIO.GPIO_Pin = GPIO_Pin_11; //RX
    myGPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&myGPIO);
    
    //инициализация USART 
    myUSART.USART_BaudRate = 57600;                                       //проходит в Control register 1 (USART_CR1/2/3)
    myUSART.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
    myUSART.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
    myUSART.USART_Parity = USART_Parity_No; //бит четности
    myUSART.USART_StopBits = USART_StopBits_1; //количество стоп битов
    myUSART.USART_WordLength = USART_WordLength_8b; //количество бит в посылке
    USART_Init(USART3,&myUSART);
    
    /* Enable USART1 */
	USART3->CR1|=(1<<13);// USART_Cmd(USART1, ENABLE);//   Control register

    /* Enable the USART1 Receive interrupt: this interrupt is generated when the
        USART1 receive data register is not empty */
	USART3->CR1|=(1<<5);// USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // проходит в Control register 1
}

void USARTSend(char *pucBuffer)
{
    int i = 0;

    while (*(pucBuffer + i) != '\0')
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) //проверяет Transmission Complete flag
        {
        }
        USART_SendData(USART3, *(pucBuffer + i));
        i++;
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) //проверяет Transmission Complete flag
        {
        }
        
    }
}

void USART3_IRQHandler(void) //обработчик прерываний
{
	if ((USART3->SR & USART_FLAG_RXNE) != (u16)RESET) //убеждаемся, что прилетел байт
	{
		static int RXi=0;
		char RXc;
		RXc = USART_ReceiveData(USART3);
		RX_BUF[RXi] = RXc;
		RXi++;

		if (RXc != '\n')
		{
			if (RXi > RX_BUF_SIZE - 1)
			{
				clear_CharBuffer(RX_BUF[0],RX_BUF_SIZE);
			}
		}
		else
		{
			RX_FLAG_END_LINE = 1;
			RXi=0;
		}
		//Echo
		USART_SendData(USART3, RXc);
	}
}

void clear_CharBuffer(char *ptrBUF, int size) {
    for (int i=0; i<size; i++) 
    {
        *(ptrBUF+i) = '\0';
    }
}


