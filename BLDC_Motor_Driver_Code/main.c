#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework

int ButtonStatePA1, ButtonStatePA2, ButtonStatePA3 = 0;
int Start, Stop ;
int ClockWise, CounterClockWise;
int MotorMode, GeneratorMode;
int HallA, HallB, HallC ;
int Step, Position;

void GPIO_Input_Definition( void );
void GPIO_Hall_Input_Definition( void );

int GetInformationFroSensor( void );
void OpenAllSwitch( int pwmA, int pwmB, int pwmC );


int main()
{
	SystemCoreClockUpdate();
	GPIO_Input_Definition();
	GPIO_Hall_Input_Definition();
	
	while(1)
	{
	
		ButtonStatePA1 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
		if(0 == ButtonStatePA1)
		{
		
		Start = 0;
		Stop = 1 ;
		OpenAllSwitch(0,0,0);
		//if Stop==1, All switch is isolation
	
		}

		else
		{
		Start = 1;
		Stop = 0;
		
		}	
		
		ButtonStatePA2 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4);
		if(0 == ButtonStatePA2)
		{
			
			ClockWise = 0;
			CounterClockWise = 1;

		}
		
		else
		{
		ClockWise = 1;
		CounterClockWise = 0;

		}
		
		ButtonStatePA3 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
		if(0 == ButtonStatePA3)
		{
		MotorMode = 0;
		GeneratorMode = 1;

		}
	
		else
		{
		MotorMode = 1;
		GeneratorMode = 0;

		}
		if((Start == 1 ) && (ClockWise == 1 ) && (MotorMode == 1 ))
		{
	
		Position = GetInformationFroSensor();
		}
		
	
	
		if((Start == 1 ) && (CounterClockWise == 1 ) && (MotorMode == 1 ))
		{
		Position = GetInformationFroSensor();
	
		}
	
	}
	
	}

void GPIO_Input_Definition()
{
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_Input_Definition;
	
	GPIO_Input_Definition.GPIO_Mode = GPIO_Mode_IPU;
	//Input Pull_Up Mode
	GPIO_Input_Definition.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_15;
	/*
	PB3 = Start || Stop
	PB4 = ClockWise || CounterClockWise
	PB15 = GeneratorMode || MotorMode
	*/
	GPIO_Input_Definition.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB, &GPIO_Input_Definition);
		
}

void GPIO_Hall_Input_Definition()
{
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_Hall_Input_Definition;
	
	GPIO_Hall_Input_Definition.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//Input Float Mode
	GPIO_Hall_Input_Definition.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15 ;
	GPIO_Hall_Input_Definition.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_Hall_Input_Definition);
		
}
int GetInformationFroSensor( void )
{

		HallA = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);
		HallB = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
		HallC = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15);
	
	if((HallA == 1) && (HallB == 0) && (HallC == 1))
	{
		Step = 2;
	}
	else if((HallA == 1) && (HallB == 0) && (HallC == 0))
	{
		Step = 3;
	}
	else if((HallA == 1) && (HallB == 1) && (HallC == 0))
	{
		Step = 4;
	}
	else if((HallA == 0) && (HallB == 1) && (HallC == 0))
	{
		Step = 5;
	}
	else if((HallA == 0) && (HallB == 1) && (HallC == 1))
	{
		Step = 6;
	}
	else if((HallA == 0) && (HallB == 0) && (HallC == 1))
	{
		Step = 1;
	}
	
	return Step;

}
void OpenAllSwitch(int pwmA, int pwmB, int pwmC)
{

	
}
