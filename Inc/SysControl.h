#ifndef SysControl_H_
#define SysControl_H_
#include "main.h"
typedef enum{
    PWM_ACTIV_0=0,
    PWM_ACTIV_1=1
}Type_Control;

//prototypes
void ControlTask(void);
void PIreg(STAB *ch,Type_Control TypeControl);
//macross

//constants

#endif /*SysControl_H_*/

