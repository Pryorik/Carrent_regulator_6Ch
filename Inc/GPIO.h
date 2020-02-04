#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f10x.h"

//prototypes
void rcc_ini(void); //Вкл тактированния портов
void port_ini(void); // инициализация ножек портов
void GPIO_ini(void);
//macross
#define CONTROL_ENABLE_Ch1_2 GPIOB->BSRR  |= GPIO_BSRR_BR5;
#define CONTROL_DISABLE_Ch1_2 GPIOB->BSRR |= GPIO_BSRR_BS5;

#define CONTROL_ENABLE_Ch3_4 GPIOB->BSRR  |= GPIO_BSRR_BR4;
#define CONTROL_DISABLE_Ch3_4 GPIOB->BSRR |= GPIO_BSRR_BS4;

#define CONTROL_ENABLE_Ch5_6 GPIOB->BSRR  |= GPIO_BSRR_BR3;
#define CONTROL_DISABLE_Ch5_6 GPIOB->BSRR |= GPIO_BSRR_BS3;
//constants



#endif /*GPIO_H_*/
