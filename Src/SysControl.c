#include "SysControl.h"
#include "UART.h"
#include "GPIO.h"

STAB STAB_Ch1={0,0,0,0,.D = &(TIM4->CCR1)};
STAB STAB_Ch2={0,0,0,0,.D = &(TIM4->CCR2)};
STAB STAB_Ch3={0,0,0,0,.D = &(TIM3->CCR3)};
STAB STAB_Ch4={0,0,0,0,.D = &(TIM3->CCR4)};
STAB STAB_Ch5={0,0,0,0,.D = &(TIM2->CCR1)};
STAB STAB_Ch6={0,0,0,0,.D = &(TIM2->CCR2)};

void ControlTask(void)
{
	extern system_param sys;

	// *(STAB_Ch1.D)=sys.Dmax;
	// TIM4->CCR1=sys.Dmax;
	if(sys.workState==START)
	{

		CONTROL_ENABLE_Ch1_2
		
		STAB_Ch1.Iust=sys.Iust/sys.amountChannel;
		PIreg(&STAB_Ch1,PWM_ACTIV_1);

		STAB_Ch2.Iust=sys.Iust/sys.amountChannel;
		PIreg(&STAB_Ch2,PWM_ACTIV_0);

		CONTROL_ENABLE_Ch3_4
		STAB_Ch3.Iust=sys.Iust/sys.amountChannel;
		PIreg(&STAB_Ch3,PWM_ACTIV_1);

		STAB_Ch4.Iust=sys.Iust/sys.amountChannel;
		PIreg(&STAB_Ch4,PWM_ACTIV_0);

		CONTROL_ENABLE_Ch5_6
		STAB_Ch5.Iust=sys.Iust/sys.amountChannel;
		PIreg(&STAB_Ch5,PWM_ACTIV_1);

		STAB_Ch6.Iust=sys.Iust/sys.amountChannel;
		PIreg(&STAB_Ch6,PWM_ACTIV_0);
	/*TEST_SPEED2:2*/
	
	// 	count_tic = DWT_CYCCNT;
	// 	int bufferUART1[20];
	// 	sprintf(bufferUART1, "\r\ncount_tic=%d\0", count_tic); 
	// 	USARTSend(bufferUART1);
	// /*TEST_SPEED2:2*/
	}
	if(sys.workState==STOP)
	{
		CONTROL_DISABLE_Ch1_2
		*STAB_Ch1.D=0;
		STAB_Ch1.ErrorNow=0;
		STAB_Ch1.ErrorFull=0;

		*STAB_Ch2.D=sys.Dmax;
		STAB_Ch2.ErrorNow=0;
		STAB_Ch2.ErrorFull=0;

		CONTROL_DISABLE_Ch3_4
		*STAB_Ch3.D=0;
		STAB_Ch3.ErrorNow=0;
		STAB_Ch3.ErrorFull=0;

		*STAB_Ch4.D=sys.Dmax;
		STAB_Ch4.ErrorNow=0;
		STAB_Ch4.ErrorFull=0;

		CONTROL_DISABLE_Ch5_6
		*STAB_Ch5.D=0;
		STAB_Ch5.ErrorNow=0;
		STAB_Ch5.ErrorFull=0;

		*STAB_Ch6.D=sys.Dmax;
		STAB_Ch6.ErrorNow=0;
		STAB_Ch6.ErrorFull=0;

		// /*test*/
	    //CONTROL_ENABLE_Ch1_2
		// CONTROL_ENABLE_Ch3_4
		// CONTROL_ENABLE_Ch5_6
	    // TIM3->CCR3=200;
		//TIM3->CCR4=sys.Dmax-sys.Dmax/10;

		//TIM4->CCR1=50;
		//TIM4->CCR2=sys.Dmax-5;

		// TIM2->CCR1=5;
		// // TIM2->CCR2=sys.Dmax-sys.Dmax/10;
	}
}

void PIreg(STAB *ch,Type_Control TypeControl)
{
	extern system_param sys;
	ch->ErrorNow = (ch->Iust) - (ch->Inow);//ошибка по току
	int32_t SumE = ch->ErrorNow + ch->ErrorFull;
	
	int32_t SumPI=(sys.kp * (ch->ErrorNow) + sys.ki * SumE)/10000;//10000 для компенсации больших значений ki/p
	if(SumPI <= sys.Dmax)
	{
		ch->ErrorFull+=ch->ErrorNow;
	}
	else
	{
		SumPI=(sys.kp * (ch->ErrorNow) + sys.ki * ch->ErrorFull)/10000;//10000 для компенсации больших значений ki/p
	}
		
	if (SumPI > sys.Dmax)
	{
		SumPI = sys.Dmax;
	}
		
	if (SumPI < 0)
	{
		ch->ErrorFull = 0;
	}
    //НАписать защиту от маленькой скважности ключа
	//1000000000/(sys.Fshim*sys.Dmax) //время вкл ключа соответствующе одному делению скважности
	// if ((SumPI <= (sys.Dmax / 10)) && (SumPI >= 0))
	// { //МИНИМАЛЬНОЕ ВРЕМЯ, КОТОРОЕ КЛЮЧ ДОЛЖЕН БЫТЬ ОТКРЫТ 5мкс
	// 	if(TypeControl==PWM_ACTIV_1) {
	// 		*(ch->D) = 0;
	// 	}
	// 	if(TypeControl==PWM_ACTIV_0) {
	// 		*(ch->D) = sys.Dmax;
	// 	} 
		
	// }
	// else
	// {
		if(TypeControl==PWM_ACTIV_1) {
			*(ch->D) = SumPI; // D коэффициент заполнения //Dmax=36000 т.е ключ всегда включен Dmin=0  т.е ключ всегда выключен Минимальная скважность при пропускании тока 328
		}
		if(TypeControl==PWM_ACTIV_0) {
			*(ch->D) = sys.Dmax - SumPI;
		}
	// }
}