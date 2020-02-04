#ifndef SERVISE_H
#define SERVISE_H

#include "stm32f10x.h"
#define  SIZE_FILTR 1

typedef struct {
    uint8_t  inc;
    uint8_t  del;
    uint8_t  flag_FilterReady;
    uint8_t  flag_CalibrationOK;
    uint32_t CalibCounter;
    int32_t  sum;
    int32_t  Adc_old;
    uint16_t ADCBuff[SIZE_FILTR];
    int16_t  Adc_now;
    int32_t  ADCzero;
    uint8_t  ;
}ADC_filtr;

typedef struct ADC{
    uint16_t ADCval;
    uint16_t ADCvalAvg;
    uint16_t kmeasure;
    ADC_filtr filtr;
}ADC;



//prototypes
void Read_Input(void);
int16_t measure(uint16_t ADCavg, int32_t ADCzero, uint16_t kmeasure, uint8_t flagCalibrationOk);
uint16_t filtr(uint16_t ADCval, ADC_filtr *chx);
void CalibrationSens(ADC_filtr *chx);
void UART_Task(void);
//macross

//constants


#endif
