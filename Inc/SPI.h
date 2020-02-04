#ifndef SPI_H_
#define SPI_H_

#include "main.h"

//prototypes
void spi_ini(void);//инициализация spi
int Spi2_Read_Send_Data(uint16_t data);
// int  Spi1_Read_Send_Data(int data, int channel); //прием данных по spi1
// int  Spi2_Read_Send_Data(int data, int channel); //прием данных по spi2

//macross
#define		 CS_LOW 	GPIOB->BSRR |= GPIO_BSRR_BR12;
#define 	 CS_HIGH 	GPIOB->BSRR |= GPIO_BSRR_BS12;
//constants

#endif
