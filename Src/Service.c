#include "service.h"
#include "main.h"
#include "GPIO.h"
#include "UART.h"



ADC ADC_Ch1={.ADCval=0, .ADCvalAvg=0, .kmeasure=233, .filtr = {.inc=0,.del=0,.flag_FilterReady=0,.flag_CalibrationOK=0,.CalibCounter=0,.sum=0,.Adc_old=0,.ADCBuff={},.Adc_now=0,.ADCzero=0,.=1}};
ADC ADC_Ch2={.ADCval=0, .ADCvalAvg=0, .kmeasure=225, .filtr = {.inc=0,.del=0,.flag_FilterReady=0,.flag_CalibrationOK=0,.CalibCounter=0,.sum=0,.Adc_old=0,.ADCBuff={},.Adc_now=0,.ADCzero=0,.=2}};
ADC ADC_Ch3={.ADCval=0, .ADCvalAvg=0, .kmeasure=225, .filtr = {.inc=0,.del=0,.flag_FilterReady=0,.flag_CalibrationOK=0,.CalibCounter=0,.sum=0,.Adc_old=0,.ADCBuff={},.Adc_now=0,.ADCzero=0,.=3}};
ADC ADC_Ch4={.ADCval=0, .ADCvalAvg=0, .kmeasure=225, .filtr = {.inc=0,.del=0,.flag_FilterReady=0,.flag_CalibrationOK=0,.CalibCounter=0,.sum=0,.Adc_old=0,.ADCBuff={},.Adc_now=0,.ADCzero=0,.=4}};
ADC ADC_Ch5={.ADCval=0, .ADCvalAvg=0, .kmeasure=225, .filtr = {.inc=0,.del=0,.flag_FilterReady=0,.flag_CalibrationOK=0,.CalibCounter=0,.sum=0,.Adc_old=0,.ADCBuff={},.Adc_now=0,.ADCzero=0,.=5}};
ADC ADC_Ch6={.ADCval=0, .ADCvalAvg=0, .kmeasure=225, .filtr = {.inc=0,.del=0,.flag_FilterReady=0,.flag_CalibrationOK=0,.CalibCounter=0,.sum=0,.Adc_old=0,.ADCBuff={},.Adc_now=0,.ADCzero=0,.=6}};

extern STAB STAB_Ch1;
extern STAB STAB_Ch2;
extern STAB STAB_Ch3;
extern STAB STAB_Ch4;
extern STAB STAB_Ch5;
extern STAB STAB_Ch6;

void Read_Input(void)//выполняется за 4.5мкСек
{
	extern volatile uint16_t ADCBuffer[];
	ADC_Ch1.ADCval = ADCBuffer[0];
	ADC_Ch1.ADCvalAvg= filtr(ADC_Ch1.ADCval,&ADC_Ch1.filtr);
	
	ADC_Ch2.ADCval = ADCBuffer[1];
	ADC_Ch2.ADCvalAvg= filtr(ADC_Ch2.ADCval,&ADC_Ch2.filtr);

	ADC_Ch3.ADCval = ADCBuffer[2];
	ADC_Ch3.ADCvalAvg= filtr(ADC_Ch3.ADCval,&ADC_Ch3.filtr);

	ADC_Ch4.ADCval = ADCBuffer[3];
	ADC_Ch4.ADCvalAvg= filtr(ADC_Ch4.ADCval,&ADC_Ch4.filtr);

	ADC_Ch5.ADCval = ADCBuffer[4];
	ADC_Ch5.ADCvalAvg= filtr(ADC_Ch5.ADCval,&ADC_Ch5.filtr);

	ADC_Ch6.ADCval = ADCBuffer[5];
	ADC_Ch6.ADCvalAvg= filtr(ADC_Ch6.ADCval,&ADC_Ch6.filtr);

	STAB_Ch1.Inow=measure(ADC_Ch1.ADCvalAvg, ADC_Ch1.filtr.ADCzero, ADC_Ch1.kmeasure, ADC_Ch1.filtr.flag_CalibrationOK);
	STAB_Ch2.Inow=measure(ADC_Ch2.ADCvalAvg, ADC_Ch2.filtr.ADCzero, ADC_Ch2.kmeasure, ADC_Ch2.filtr.flag_CalibrationOK);
	STAB_Ch3.Inow=measure(ADC_Ch3.ADCvalAvg, ADC_Ch3.filtr.ADCzero, ADC_Ch3.kmeasure, ADC_Ch3.filtr.flag_CalibrationOK);
	STAB_Ch4.Inow=measure(ADC_Ch4.ADCvalAvg, ADC_Ch4.filtr.ADCzero, ADC_Ch4.kmeasure, ADC_Ch4.filtr.flag_CalibrationOK);
	STAB_Ch5.Inow=measure(ADC_Ch5.ADCvalAvg, ADC_Ch5.filtr.ADCzero, ADC_Ch5.kmeasure, ADC_Ch5.filtr.flag_CalibrationOK);
	STAB_Ch6.Inow=measure(ADC_Ch6.ADCvalAvg, ADC_Ch6.filtr.ADCzero, ADC_Ch6.kmeasure, ADC_Ch6.filtr.flag_CalibrationOK);
}

int16_t measure(uint16_t ADCavg, int32_t ADCzero, uint16_t kmeasure, uint8_t flagCalibrationOk)
{
	if(flagCalibrationOk)
	{
		if ((ADCavg <= ADCzero+20) && (ADCavg >= ADCzero-400))
		{ 
			return 0;
		}
		else
		{
			return ((1000 * (ADCavg - ADCzero)) / kmeasure);
		}
	}
}

uint16_t filtr(uint16_t ADCval, ADC_filtr *chx)
{
	chx->Adc_old = chx->ADCBuff[chx->inc];

	chx->ADCBuff[chx->inc] = ADCval;

	if (chx->del != SIZE_FILTR)
	{
		chx->del++;
	}
	else
	{
		chx->flag_FilterReady = 1;
		chx->sum = chx->sum - chx->Adc_old;
	}
	chx->sum = chx->sum + chx->ADCBuff[chx->inc];
	chx->inc++;
	if (chx->inc == SIZE_FILTR)
	{
		chx->inc = 0;
	}
	chx->Adc_now = chx->sum / chx->del;

	if(chx->flag_FilterReady == 1 && chx->flag_CalibrationOK == 0)
	{
		CalibrationSens(chx);
	}
	return chx->Adc_now;
}

void CalibrationSens(ADC_filtr *chx)
{
	//Калибровка датчиков тока-----------------------------------------
	if (chx->flag_FilterReady)
	{
		chx->ADCzero += chx->Adc_now;
		chx->CalibCounter++;
		if (chx->CalibCounter == 1000)
		{
			chx->ADCzero = chx->ADCzero / chx->CalibCounter;
			chx->flag_CalibrationOK = 1;
			USARTSend("\r\nSensors Calibration Complite!\r\n");
		}
	}
	//-----------------------------------------------------------------
}

void UART_Task(void)
{
	extern int  RX_FLAG_END_LINE;
	extern char RX_BUF[RX_BUF_SIZE];
	extern system_param sys;

	if (RX_FLAG_END_LINE == 1)
	{
		char NumRX[NUM_RX_BUF_SIZE]={}; 
		char buffer[100];
		int i = 0;
		RX_FLAG_END_LINE = 0; //Reset END_LINE Flag
		if (strncmp(RX_BUF, "ki=\r\n", 3) == 0)
		{
			USARTSend("COMMAND: ki=...\r\n");
			i = 3;
			while (RX_BUF[i] != 13)
			{
				NumRX[i - 3] = RX_BUF[i];
				i++;
			}
			if (atof(NumRX) != 0)
				sys.ki = atof(NumRX);
			STAB_Ch1.ErrorFull = round(*(STAB_Ch1.D) / sys.ki);
			STAB_Ch2.ErrorFull = round(*(STAB_Ch1.D) / sys.ki);
			sprintf(buffer, "ki=%0.3f\r\n", sys.ki);
			USARTSend(buffer);
		}
		if (strncmp(RX_BUF, "Iust=\r\n", 5) == 0)
		{
			USARTSend("COMMAND: Iust=...\r\n");
			i = 5;
			while (RX_BUF[i] != 13)
			{
				NumRX[i - 5] = RX_BUF[i];
				i++;
			}
			if (atof(NumRX) != 0)
				sys.Iust = atoi(NumRX);
			sprintf(buffer, "Iust=%d\r\n", sys.Iust);
			USARTSend(buffer);
		}
		if (strncmp(RX_BUF, "CLB\r\n", 5) == 0)
		{
			USARTSend("COMMAND: Sensors Calibration!\r\n");
			ADC_Ch1.filtr.ADCzero = 0;
			ADC_Ch2.filtr.ADCzero = 0;
			ADC_Ch3.filtr.ADCzero = 0;
			ADC_Ch4.filtr.ADCzero = 0;
			ADC_Ch5.filtr.ADCzero = 0;
			ADC_Ch6.filtr.ADCzero = 0;
			ADC_Ch1.filtr.CalibCounter = 0;
			ADC_Ch2.filtr.CalibCounter = 0;
			ADC_Ch3.filtr.CalibCounter = 0;
			ADC_Ch4.filtr.CalibCounter = 0;
			ADC_Ch5.filtr.CalibCounter = 0;
			ADC_Ch6.filtr.CalibCounter = 0;
			ADC_Ch1.filtr.flag_CalibrationOK = 0;
			ADC_Ch2.filtr.flag_CalibrationOK = 0;
			ADC_Ch3.filtr.flag_CalibrationOK = 0;
			ADC_Ch4.filtr.flag_CalibrationOK = 0;
			ADC_Ch5.filtr.flag_CalibrationOK = 0;
			ADC_Ch6.filtr.flag_CalibrationOK = 0;
		}
		if (strncmp(RX_BUF, "START\r\n", 7) == 0)
		{
			USARTSend("COMMAND: START!\r\n");
			sys.workState = START;
		}
		if (strncmp(RX_BUF, "STOP\r\n", 6) == 0)
		{
			USARTSend("COMMAND: STOP!\r\n");
			sys.workState = STOP;
		}
		clear_CharBuffer(RX_BUF,RX_BUF_SIZE);
	}
}

