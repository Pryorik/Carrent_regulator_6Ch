#include "Variable.h"

//varible UART
extern int RX_FLAG_END_LINE;

//varible Timer
extern int32_t Fshim, Psc, Dmax;

//varible SysControl
extern float ki;
extern int8_t Mode;
extern volatile uint16_t *D1, *D2;

//varible SysControl
extern int16_t flagMAINsendFshim;

//varible 
int8_t Device, IchMax;
	
void variable_ini(void)
{
	//Инициализция переменных при включении
	Device = ONE;
	IchMax=100; //Ток канла в Амперах
	RX_FLAG_END_LINE = 0;
	flagMAINsendFshim = 0;
	ki=0.1;
	Mode=STOP;
	Fshim=2000;
	Psc=1;
    D1 = &(TIM4->CCR1);
	*D1 = 0;
	D2 = &(TIM4->CCR2); 
	*D2 = Dmax-0;
}



