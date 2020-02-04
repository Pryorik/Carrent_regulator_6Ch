#ifndef UART_H_
#define UART_H_

#include "stm32f10x.h"
//prototypes
void USART_ini(void);//инициализация UARTvoid USART1_IRQHandler(void);//обработка прерывания по появлкнию данных в data buffer
void USARTSend( char *pucBuffer ); //отправка строки в терминал
void USART1_IRQHandler(void); //обработчик прерываний
void clear_CharBuffer(char *ptrBUF, int size); // отчистка буффера принятых с ПК данных
//macross

//constants
#define RX_BUF_SIZE 80  //буфер принятых данных
#define bufferDMA_SIZE 100 //буфер отправляемых в DMA данных
#define NUM_RX_BUF_SIZE 20 //буфер для вводимых с консоли цифр

#endif /*SPI_H_*/

