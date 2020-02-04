#ifndef MAIN_H_
#define MAIN_H_


#include <stdint.h>
#include <stdio.h>
#include <misc.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


#include "service.h"
//macross
#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC

#define		 Speed_Flag_SET 	 GPIOC->BSRR = GPIO_BSRR_BS13; 
#define 	 Speed_Flag_RESET  GPIOC->BSRR = GPIO_BSRR_BR13; 
#define SizebufferUART 200 //буфер отправляемых данных
#define F_CPU 		72000000UL	// Тактовая у нас 72МГЦ
#define TimerTick  	F_CPU/1000-1	// Нам нужен килогерц

typedef struct STAB{
    uint16_t Iust;
    int16_t Inow;
    int16_t ErrorNow;
    int32_t ErrorFull;
    volatile uint16_t *D;      //скважность
}STAB;

typedef struct current_channel_all{
    STAB *ch1;
    STAB *ch2;
    STAB *ch3;
    STAB *ch4;
    STAB *ch5;
    STAB *ch6;
}STAB_channel_all;

typedef struct ADC_channel_all{
    ADC *ch1;
    ADC *ch2;
    ADC *ch3;
    ADC *ch4;
    ADC *ch5;
    ADC *ch6;
}ADC_channel_all;

typedef enum work_state{
    START = 0,
    STOP = 1,
    TEST = 2
}work_state;

typedef struct system_param{
    int32_t             Fshim;
    uint16_t            kp;
    uint16_t            ki;
    uint16_t            Iust;
    uint16_t            Dmax;
    uint16_t            Vbat;
    uint16_t            t_minOpen; 
    work_state          workState;
    uint8_t             amountChannel;
    STAB_channel_all    STAB_ChannelAll;
    ADC_channel_all     ADC_ChannelAll;

}system_param;

void struct_ini(system_param *ptrsys);
//constants
#define buffer_SIZE 100 //буфер отправляемых данных

#endif /*MAIN_H_*/

