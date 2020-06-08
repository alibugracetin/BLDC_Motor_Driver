#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework

int ButtonStatePA1, ButtonStatePA2, ButtonStatePA3 = 0;
int Start, Stop ;
int ClockWise, CounterClockWise;
int MotorMode, GeneratorMode;

void GPIO_Input_Definition( void );

int main()
{
	SystemCoreClockUpdate();
	GPIO_Input_Definition();
	
	while(1)
	{
	
		ButtonStatePA1 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
		ButtonStatePA2 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
		ButtonStatePA3 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
		
	}
	
}

void GPIO_Input_Definition ()
{
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_Input_Definition;
	
	GPIO_Input_Definition.GPIO_Mode = GPIO_Mode_IPU;
	//Input Pull_Up MOde
	GPIO_Input_Definition.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_15;
	/*
	PB3 = Start || Stop
	PB4 = ClockWise || CounterClockWise
	PB15 = GeneratorMode || MotorMode
	*/
	GPIO_Input_Definition.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB, &GPIO_Input_Definition);
		
}
