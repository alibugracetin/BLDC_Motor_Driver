#include "Delay.h"
#include "stm32f10x.h"                  // Device header

uint32_t time_ms;
uint32_t time_DeadTime_ms;

void Delay_Init(void)
{
	SystemCoreClockUpdate();
	SysTick_Config((SystemCoreClock / 1000000));
}

void Delay_ms(uint32_t value_ms)
{
	time_ms = value_ms;
	
	while (time_ms--)
	{
		DeadTime(1000);
		//wait
	}

}

void DeadTime(uint32_t value_DeadTime_ms)
{
	time_DeadTime_ms = value_DeadTime_ms;
	
	while (time_DeadTime_ms)
	{
	//wait
	}

}
void SysTick_Handler(void)
{

		if(time_DeadTime_ms)
		{
			time_DeadTime_ms = time_DeadTime_ms - 1;
		}
}
