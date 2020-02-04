#include "interrupts.h"
#include "UART.h"
#include "GPIO.h"

volatile uint32_t timer_ms=0;
volatile uint32_t timerProtect_ms=0;

void TIM1_UP_IRQHandler(void)
{
	/*тест частоты systick*/
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
		static int del=4, count=0; //del делитель частоты
		count++;
		Read_Input();/*80kHz*/
		if(count>=del)
		{
			count=0;
			ControlTask();/*20kHz*/
		}
		TIM1->SR &= ~ TIM_SR_UIF;
	}
}

void SysTick_Handler(void)
{
#ifdef USE_STDPERIPH_DRIVER
	timer_ms++;
	timerProtect_ms++;
	/*тест частоты systick*/
	// static int counter = 0;
	// counter = counter ^ 1;
	// if (counter == 0)
	// {
	// 	CONTROL_ENABLE_Ch1_2
	// }
	// if (counter == 1)
	// {
	// 	CONTROL_DISABLE_Ch1_2
	// }
#ifdef WATCHDOG_ENABLEDb
	IWDG_ReloadCounter();
#endif

#else
	HAL_IncTick();
	if (IWDG_Inst.State == HAL_IWDG_STATE_READY)
		HAL_IWDG_Refresh(&IWDG_Inst);
	HAL_SYSTICK_IRQHandler();
#endif
}