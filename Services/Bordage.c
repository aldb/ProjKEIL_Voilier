
#include "bordage.h"
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_bus.h" 
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_exti.h" 

void Bordage_Conf()
{
	// Conf PA8
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	// PIN D14
	LL_GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_Init.Pin = LL_GPIO_PIN_8;
	GPIO_Init.OutputType =  LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_Init.Pull = LL_GPIO_PULL_DOWN;
	GPIO_Init.Speed = LL_GPIO_MODE_OUTPUT_10MHz;
	LL_GPIO_Init(GPIOA,&GPIO_Init);
	
	// Conf timer
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	
	LL_TIM_InitTypeDef Timer_Init;
	Timer_Init.Autoreload = 999;
	Timer_Init.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	Timer_Init.CounterMode = LL_TIM_COUNTERMODE_UP;
	Timer_Init.Prescaler = 1440-1;
	Timer_Init.RepetitionCounter = 0;
	LL_TIM_Init(TIM1, &Timer_Init);
	
	LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct;	
	TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_DISABLE;
	TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_DISABLE;
	TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_OFF;
	TIM_BDTRInitStruct.DeadTime = 0;
	TIM_BDTRInitStruct.BreakState = LL_TIM_BREAK_DISABLE;
	TIM_BDTRInitStruct.BreakPolarity = LL_TIM_BREAK_POLARITY_HIGH;
	TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_ENABLE;
	LL_TIM_BDTR_Init(TIM1, &TIM_BDTRInitStruct);

	LL_TIM_OC_InitTypeDef Timer_OC_Init;
	Timer_OC_Init.CompareValue = 100;
	Timer_OC_Init.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
	Timer_OC_Init.OCMode = LL_TIM_OCMODE_PWM1 ;
	Timer_OC_Init.OCState=LL_TIM_OCSTATE_ENABLE;
	Timer_OC_Init.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	
	Timer_OC_Init.OCNState=LL_TIM_OCSTATE_DISABLE;
	Timer_OC_Init.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
	Timer_OC_Init.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
	
	LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH1, &Timer_OC_Init);
	
	LL_TIM_EnableCounter(TIM1);
	
}

float comparevalue; 
void Set_Bordage(int Angle)
{	if (Angle>360) {Angle=360;}
	if (Angle > 180) 
	{
		comparevalue=(86- (((86.0-40.0)/90.0)* ((180-(Angle-180))/2.0)));
	}
	else 
	{
		comparevalue=(86- (((86.0-40.0)/90.0)* (Angle/2.0)));
	}
	
	LL_TIM_OC_SetCompareCH1(TIM1, (int)comparevalue);
}
