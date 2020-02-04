#include "main.h"
#include "GPIO.h"
#include "SysControl.h"
#include "UART.h"
#include "ADC.h"
#include "Timers.h"
#include "SysClock.h"
#include "interrupts.h"
#include "SPI.h"

system_param sys={.Fshim=100000, .kp=10, .ki=10, .Iust=500, .Vbat=0, .workState=STOP, .amountChannel=1, .STAB_ChannelAll = {}, .ADC_ChannelAll = {} };
extern ADC ADC_Ch1;
extern ADC ADC_Ch2;
extern ADC ADC_Ch3;
extern ADC ADC_Ch4;
extern ADC ADC_Ch5;
extern ADC ADC_Ch6;

extern STAB STAB_Ch1;
extern STAB STAB_Ch2;
extern STAB STAB_Ch3;
extern STAB STAB_Ch4;
extern STAB STAB_Ch5;
extern STAB STAB_Ch6;

extern volatile uint16_t ADCBuffer[];

extern volatile uint32_t timer_ms;
extern int flag_interruptTIM1clk, flag_interruptTIM1clkdel;

int main(void)
{
	char bufferUART[SizebufferUART]; 

	SysClock_ini();
      /* Set NVIC Priority Group */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//выбор группы приоритетов
	 
	SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;// разрешаем использовать DWT
	DWT_CONTROL|= DWT_CTRL_CYCCNTENA_Msk; // включаем счётчик ITM_TCR_DWTENA_Msk
	DWT_CYCCNT = 0;// обнуляем значение
	GPIO_ini();
	spi_ini();
	USART_ini();
	ADC_DMA_init();
	SysTick_Config(TimerTick);
	struct_ini(&sys);
	sys.Dmax = TIM_ini(sys.Fshim);
	USARTSend(("\r\nHello, i am ready!\r\n"));
	

	while (1)
	{
	/*TEST_SPEED1:2*/
		static volatile uint32_t count_tic = 0;
		DWT_CYCCNT = 0;// обнуляем значение
	/*TEST_SPEED1:2*/
		if (timer_ms >= 100)
		{
			sys.Vbat = Spi2_Read_Send_Data(0);
			timer_ms = 0;
			// int bufferUART1[20];
			// sprintf(bufferUART1, "\r\ncount_tic=%d\0", count_tic); 
			// USARTSend(bufferUART1);
			// DWT_CYCCNT = 0;// обнуляем значение
			// sprintf(bufferUART, "\r\nIust=%d Ich1=%d  Ich2=%d  e1=%d  e2=%d  D1=%d  D2=%d Dmax=%d\0", sys.Iust, STAB_Ch1.Inow, STAB_Ch2.Inow, STAB_Ch1.ErrorNow, STAB_Ch2.ErrorNow, (TIM4->CCR1), (sys.Dmax - TIM4->CCR2), sys.Dmax); //(*D/655)
			// USARTSend(bufferUART);
			sprintf(bufferUART, "\r\nIust=%d Ich %d %d %d %d %d %d V=%d\0", sys.Iust, STAB_Ch1.Inow, STAB_Ch2.Inow, STAB_Ch3.Inow, STAB_Ch4.Inow, STAB_Ch5.Inow, STAB_Ch6.Inow, sys.Vbat); 
			USARTSend(bufferUART);
			sprintf(bufferUART, " ERR %d %d %d %d %d %d\0", STAB_Ch1.ErrorNow, STAB_Ch2.ErrorNow, STAB_Ch3.ErrorNow, STAB_Ch4.ErrorNow, STAB_Ch5.ErrorNow, STAB_Ch6.ErrorNow); 
			USARTSend(bufferUART);
			sprintf(bufferUART, " D__ %d %d %d %d %d %d Dmax=%d\0",*STAB_Ch1.D, sys.Dmax-*STAB_Ch2.D, *STAB_Ch3.D, sys.Dmax-*STAB_Ch4.D, *STAB_Ch5.D, sys.Dmax-*STAB_Ch6.D, sys.Dmax); //(*D/655)
			USARTSend(bufferUART);
			// sprintf(bufferUART, "\r\nIust=%d Ich3=%d  Ich4=%d  e3=%d  e4=%d  D3=%d  D4=%d Dmax=%d\0", sys.Iust, STAB_Ch3.Inow, STAB_Ch4.Inow, STAB_Ch3.ErrorNow, STAB_Ch4.ErrorNow, (TIM3->CCR3), (sys.Dmax - TIM3->CCR4), sys.Dmax); //(*D/655)
			// USARTSend(bufferUART);
			// count_tic = DWT_CYCCNT;
			// count_tic=count_tic;
			//  sprintf(bufferUART, "\r\n ADC1=%d ADC2=%d ADC1ave=%d ADC2ave=%d I1=%d I2=%d", ADCBuffer[0], ADCBuffer[1], ADC_Ch1.filtr.Adc_now, ADC_Ch2.filtr.Adc_now, STAB_Ch1.Inow, STAB_Ch2.Inow);//(*D/655)
			//  USARTSend(bufferUART);
		}
		UART_Task();
			/*TEST_SPEED2:2*/
			// count_tic = DWT_CYCCNT;
			// /*TEST_SPEED2:2*/
	}
}

void struct_ini(system_param *ptrsys)
{
	ptrsys->ADC_ChannelAll.ch1 =  &ADC_Ch1;
	ptrsys->STAB_ChannelAll.ch1 = &STAB_Ch1;
	
	ptrsys->ADC_ChannelAll.ch2 =  &ADC_Ch2;
	ptrsys->STAB_ChannelAll.ch2 = &STAB_Ch2;
	
	ptrsys->ADC_ChannelAll.ch3 =  &ADC_Ch3;
	ptrsys->STAB_ChannelAll.ch3 = &STAB_Ch3;
	
	ptrsys->ADC_ChannelAll.ch4 =  &ADC_Ch4;
	ptrsys->STAB_ChannelAll.ch4 = &STAB_Ch4;

	ptrsys->ADC_ChannelAll.ch5 =  &ADC_Ch5;
	ptrsys->STAB_ChannelAll.ch5 = &STAB_Ch5;

	ptrsys->ADC_ChannelAll.ch6 =  &ADC_Ch6;
	ptrsys->STAB_ChannelAll.ch6 = &STAB_Ch6;
}