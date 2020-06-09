#ifndef Delay_h_
#define Delay_h_
#include "stm32f10x.h"                  // Device header


void Delay_Init(void);
void Delay_ms(uint32_t value_ms);
void DeadTime(uint32_t value_DeadTime_ms);



#endif
