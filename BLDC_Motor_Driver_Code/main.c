#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "Delay.h"

int ButtonStatePA1, ButtonStatePA2, ButtonStatePA3 = 0;
int Start, Stop ;
int ClockWise, CounterClockWise;
int MotorMode, GeneratorMode;
int HallA, HallB, HallC ;
int Step, Position;
int16_t pwm = 0;

void GPIO_Input_Definition( void );
void GPIO_Hall_Input_Definition( void );
void GPIO_ADC_Input_Definition( void );
void adcRead_Definition( void );
void GPIODefinitionForDriver( void );
void PWM_Pin_Definition( void );
void PWM_Timer_Init( void );

uint16_t ADC_Read(void);
int GetInformationFromSensor( void );
void OpenAllSwitch( int pwmA, int pwmB, int pwmC );
void PWM_Definition( int pwmA, int pwmB, int pwmC );


int main()
{
	SystemCoreClockUpdate();
	GPIO_Input_Definition();
	GPIO_Hall_Input_Definition();
	Delay_Init();
	GPIO_ADC_Input_Definition();
	adcRead_Definition();
	GPIODefinitionForDriver();
	PWM_Pin_Definition();
	PWM_Timer_Init();
	PWM_Definition(pwm, pwm, pwm);
	
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
		
		pwm = ADC_Read();
		
		if((Start == 1 ) && (ClockWise == 1 ) && (MotorMode == 1 ))
		{
	
			Position = GetInformationFromSensor();
			//Every 70uS Read Rotor position from GetInformationFromSensor
			DeadTime(70);
			
			
		}
		
	
	
		if((Start == 1 ) && (CounterClockWise == 1 ) && (MotorMode == 1 ))
		{
			
			Position = GetInformationFromSensor();
			//Every 70uS Read Rotor position from GetInformationFromSensor
			DeadTime(70);
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
	/*
	PA11 = HallA
	PA12 = HallB
	PA13 = HallC
	*/
	GPIO_Hall_Input_Definition.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_Hall_Input_Definition);
		
}

void GPIO_ADC_Input_Definition()
{
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_ADC_Input_Definition;
	
	GPIO_ADC_Input_Definition.GPIO_Mode = GPIO_Mode_AIN;
	//Analog Input MOde
	GPIO_ADC_Input_Definition.GPIO_Pin = GPIO_Pin_1 ;
	// PA1 = Pot(Variable pwm)
	GPIO_ADC_Input_Definition.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_ADC_Input_Definition);
		
}

void adcRead_Definition()
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_InitTypeDef ADCtypedefinition;

	ADCtypedefinition.ADC_Mode = ADC_Mode_Independent;
	ADCtypedefinition.ADC_ScanConvMode = DISABLE;
	ADCtypedefinition.ADC_ContinuousConvMode = ENABLE;	// we work in continuous sampling mode
	ADCtypedefinition.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADCtypedefinition.ADC_DataAlign = ADC_DataAlign_Right;
	ADCtypedefinition.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADCtypedefinition);
	ADC_Cmd(ADC1,ENABLE);

}

uint16_t ADC_Read(void)
{
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1,ADC_SampleTime_28Cycles5);  //channel 1, PA1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);
	
}

void GPIODefinitionForDriver(){
	   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitTypeDef GPIODefinitionForDriver;
  GPIODefinitionForDriver.GPIO_Mode = GPIO_Mode_Out_PP;
	//Output Push-Pull Mode
  GPIODefinitionForDriver.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13 | GPIO_Pin_14;
	/*
	PB12 = AL
	PB13 = BL
	PB14 = CL
	*/
  GPIODefinitionForDriver.GPIO_Speed = GPIO_Speed_2MHz;
	
  GPIO_Init(GPIOB, &GPIODefinitionForDriver);

}

int GetInformationFromSensor( void )
{

	HallA = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);
	HallB = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
	HallC = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15);
	
	//120 degree commutation for BLDC motor
	
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

void PWM_Pin_Definition()
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef PWM_Pin_Definition;
	
	PWM_Pin_Definition.GPIO_Mode = GPIO_Mode_AF_PP;
	//Alternative Function Output Push Pull Mode
	PWM_Pin_Definition.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	/*
	PB6 = AH
	PB7 = BH
	PB8 = CH
	*/
	PWM_Pin_Definition.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOB, &PWM_Pin_Definition);
	
}

void PWM_Timer_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	
	TIM_BaseStruct.TIM_Prescaler = 0;
	// (72000000/(0+1)) = frequency
  TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseStruct.TIM_Period = 7199;
	//  72000000/(7199+1) = 10kHz PWM 
  TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_BaseStruct.TIM_RepetitionCounter = 0;
  // Initialize TIM4 
  TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
  // Start count on TIM4 
  TIM_Cmd(TIM4, ENABLE);

}

void PWM_Definition( int pwmA, int pwmB, int pwmC)
{
	
	TIM_OCInitTypeDef timerPWM;

	TIM_OCStructInit(&timerPWM);
	timerPWM.TIM_Pulse = pwmA;
	timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
	timerPWM.TIM_OutputState = TIM_OutputState_Enable;
	timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &timerPWM);
	
	TIM_OCStructInit(&timerPWM);
	timerPWM.TIM_Pulse = pwmB;
	timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
	timerPWM.TIM_OutputState = TIM_OutputState_Enable;
	timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM4, &timerPWM);
	
	TIM_OCStructInit(&timerPWM);
	timerPWM.TIM_Pulse = pwmC;
	timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
	timerPWM.TIM_OutputState = TIM_OutputState_Enable;
	timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM4, &timerPWM);
	
}

void OpenAllSwitch(int pwmA, int pwmB, int pwmC)
{

	
}
