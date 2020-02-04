#include "SPI.h"

// //varible SysControl
// extern int16_t Data_For_Modul_Load;
// extern int8_t Calibration;
// extern int8_t Mode;
// extern float Delta1, Delta2, S_P_Cur;
// extern volatile uint16_t *D1, *D2;

// //varible Timer
// extern int32_t Dmax;
// extern int32_t Fshim, Psc;

// //varible Varible
// extern int8_t Device,IchMax;

// //varible
// int16_t flagMAINsendFshim;

int Spi2_Read_Send_Data(uint16_t data)
 { 
  //ждём пока опустошится Tx буфер
	while(!(SPI2->SR & SPI_SR_TXE));
	 
	//активируем Chip Select
	CS_LOW			
	//отправляем данные     
	SPI2->DR = data; 
	
 	//ждём пока передатчик освободится
	while((SPI2->SR & SPI_SR_BSY))
  	{
	}		
		
	//ждём пока придёт ответ
	while(!(SPI2->SR & SPI_SR_RXNE)) // status register: RXNE: Receive buffer not empty
  	{
	}
	
	//считываем полученные данные
	data = SPI2->DR;  
  //деактивируем Chip Select
	CS_HIGH
	//ждём пока передатчик освободится
	while((SPI2->SR & SPI_SR_BSY))
	{
	}	
  return 4700*(data&0xFFF)/466;  //возвращаем то, что прочитали
 }
 
 
 void SPI2_IRQHandler(void)
{
	// int16_t Set_point_Current_Channel;
	// int32_t DmaxOLD;
	if(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==SET)
	{ //прерывание вызвано приемом байта?
	  // Set_point_Current_Channel = SPI2->DR;//прием
	}
}
void spi_ini(void)
{
	GPIO_InitTypeDef myGPIO;
	SPI_InitTypeDef mySPI;
	NVIC_InitTypeDef myNVIC;

	//SPI2_sensors_current---------------------------------------------------------------------------------
	//MASTER
	//Вкл. Тактирования
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; //Активация тактирования SPI  в enable register
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //Активация тактирования GPIOB в enable register
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //Активация тактирования Альтернативных функций в enable register

	//настройк ножек  SPI в Control Registr H
	myGPIO.GPIO_Pin = GPIO_Pin_13; //SCLK
	myGPIO.GPIO_Mode = GPIO_Mode_AF_PP;
	myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &myGPIO);

	// 	myGPIO.GPIO_Pin = GPIO_Pin_7; //MOSI
	// myGPIO.GPIO_Mode = GPIO_Mode_AF_PP;
	// myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_Init(GPIOA,&myGPIO);

	myGPIO.GPIO_Pin = GPIO_Pin_12; //SS
	myGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
	myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &myGPIO);

	myGPIO.GPIO_Pin = GPIO_Pin_14; //MISO
	myGPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &myGPIO);

	//Заполняем структуру с параметрами SPI модуля в регистре ControlReg1
	mySPI.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI_Direction_2Lines_FullDuplex полный дуплекс
	mySPI.SPI_DataSize = SPI_DataSize_16b;					// передаем по 16 бит
	mySPI.SPI_CPOL = SPI_CPOL_High;							// Полярность и
	mySPI.SPI_CPHA = SPI_CPHA_1Edge;						// фаза тактового сигнала
	mySPI.SPI_NSS = SPI_NSS_Soft;							// Управлять состоянием сигнала NSS программно
	mySPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // Предделитель SCK
	mySPI.SPI_FirstBit = SPI_FirstBit_MSB;					// Первым отправляется старший бит
	mySPI.SPI_Mode = SPI_Mode_Master;						// Режим - мастер
	SPI_Init(SPI2, &mySPI);									//Настраиваем SPI2
	SPI_Cmd(SPI2, ENABLE);									// Включаем модуль SPI2....//   Control register
	// Поскольку сигнал NSS контролируется программно, установим его в единицу
	// Если сбросить его в ноль, то наш SPI модуль подумает, что
	// у нас мультимастерная топология и его лишили полномочий мастера.
	SPI_NSSInternalSoftwareConfig(SPI2, SPI_NSSInternalSoft_Set); //установим его в 1
	//------------------------------------------------------------------------------SPI2
	
	
// 	//SPI2_current_module---------------------------------------------------------------
// 	//SLAVE
// 	//Вкл. Тактирования	
// 		RCC->APB1ENR |= (1<<14); //Активация тактирования SPI  в enable register
// 		RCC->APB2ENR |= (1<<3); //Активация тактирования GPIOB в enable register 
// 	  RCC->APB2ENR |= (1); //Активация тактирования Альтернативных функций в enable register 
	
// 	//настройк ножек  SPI в Control Registr H
//     myGPIO.GPIO_Pin = GPIO_Pin_13; //SCLK
//     myGPIO.GPIO_Mode = GPIO_Mode_AF_PP;
//     myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOB,&myGPIO);
	
// 		myGPIO.GPIO_Pin = GPIO_Pin_15; //MOSI
//     myGPIO.GPIO_Mode = GPIO_Mode_AF_PP;
//     myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOB,&myGPIO);
	
// 	  myGPIO.GPIO_Pin = GPIO_Pin_12; //SS1
//     myGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
//     myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOB,&myGPIO);
    
// 	/*myGPIO.GPIO_Pin = GPIO_Pin_3; //SS2
//     myGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
//     myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOB,&myGPIO);
// 	*/	
//     myGPIO.GPIO_Pin = GPIO_Pin_14; //MISO
//     myGPIO.GPIO_Mode = GPIO_Mode_AF_PP;;
//     myGPIO.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(GPIOB,&myGPIO);
	
// 		//Заполняем структуру с параметрами SPI модуля в регистре ControlReg1
// 		 mySPI.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //SPI_Direction_2Lines_FullDuplex полный дуплекс 
// 		 mySPI.SPI_DataSize = SPI_DataSize_16b; // передаем по 16 бит
// 		 mySPI.SPI_CPOL = SPI_CPOL_High ; // Полярность и
// 	 	 mySPI.SPI_CPHA = SPI_CPHA_1Edge; // фаза тактового сигнала
// 		 mySPI.SPI_NSS = SPI_NSS_Hard; // Управлять состоянием сигнала NSS АППАРАТНО
// 	   mySPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // Предделитель SCK
// 		 mySPI.SPI_FirstBit = SPI_FirstBit_MSB; // Первым отправляется старший бит
// 		 mySPI.SPI_Mode = SPI_Mode_Slave; // Режим - слэйв
// 		 SPI_Init(SPI2, &mySPI); //Настраиваем SPI1
// 		 SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_RXNE,ENABLE); //Включаем прерывания по приему байта
// 	 	 SPI_Cmd(SPI2, ENABLE); // Включаем модуль SPI1....//   Control register
		 
// 		 /* Enable the USARTx Interrupt *///разрешаем прерывания от SPI2
//      myNVIC.NVIC_IRQChannel = SPI2_IRQn; 
//      myNVIC.NVIC_IRQChannelPreemptionPriority = 0; //Приоритет
//      myNVIC.NVIC_IRQChannelSubPriority = 0; //Субприоритет
//      myNVIC.NVIC_IRQChannelCmd = ENABLE;
//      NVIC_Init(&myNVIC);
//   //----------------------------------------------------------------------------------SPI2
}
