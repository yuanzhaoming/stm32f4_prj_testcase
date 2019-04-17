#ifndef _timer3_h_
#define _timer3_h_

#include "stm32f4xx.h"
#include "sys_init.h"


extern void TIM3_Init(u16 arr,u16 psc);


extern unsigned int get_sys_time(void);

extern void delay_ms(int val);

#endif

