
#include "girouette.h"
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_bus.h" 
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_exti.h" 

void Girouette_Conf()
{
	// Init input PA5 PA6 PA7 
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	// PIN D13 D12 D11
	LL_GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Mode = LL_GPIO_MODE_INPUT; // remettre en floating
	GPIO_Init.Pin = LL_GPIO_PIN_5;
	GPIO_Init.OutputType = 0;
	GPIO_Init.Pull = LL_GPIO_PULL_DOWN;
	GPIO_Init.Speed = LL_GPIO_MODE_OUTPUT_10MHz;
	LL_GPIO_Init(GPIOA,&GPIO_Init);
	
	GPIO_Init.Pin = LL_GPIO_PIN_6;
	LL_GPIO_Init(GPIOA,&GPIO_Init);
	
	GPIO_Init.Pin = LL_GPIO_PIN_7;
	LL_GPIO_Init(GPIOA,&GPIO_Init);
	
	// Conf timer encoder inremental
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	
	LL_TIM_ENCODER_InitTypeDef Timer_Incremental;
	Timer_Incremental.EncoderMode = LL_TIM_ENCODERMODE_X4_TI12;
	Timer_Incremental.IC1ActiveInput= LL_TIM_ACTIVEINPUT_DIRECTTI;
	Timer_Incremental.IC1Polarity = LL_TIM_IC_POLARITY_RISING;
	Timer_Incremental.IC1Prescaler =  LL_TIM_ICPSC_DIV1;
	Timer_Incremental.IC1Filter = 0;
	Timer_Incremental.IC2ActiveInput= LL_TIM_ACTIVEINPUT_DIRECTTI;
	Timer_Incremental.IC2Polarity = LL_TIM_IC_POLARITY_RISING;
	Timer_Incremental.IC2Prescaler =  LL_TIM_ICPSC_DIV1;
	Timer_Incremental.IC2Filter = 0;
	LL_TIM_ENCODER_Init(TIM3, &Timer_Incremental);
	LL_TIM_EnableCounter(TIM3);
	
	// Init EXTI Pin GPIOA
	LL_EXTI_InitTypeDef EXTI_Init_Index;
	EXTI_Init_Index.Line_0_31 = LL_EXTI_LINE_5;
	EXTI_Init_Index.LineCommand = ENABLE;
	EXTI_Init_Index.Mode = LL_EXTI_MODE_IT;
	EXTI_Init_Index.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init (&EXTI_Init_Index);
	
	// validation du canal NVIC
	NVIC_SetPriority(EXTI9_5_IRQn, 3);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}


void EXTI9_5_IRQHandler(void)
{
	//Clear flag 
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5);
	//reset counter 
	TIM3->CNT = 0;
}

int angle; 
int Get_Angle(void)
{
	if (TIM3->CNT > 0x8000)
	{
		angle= (0xFFFF - TIM3->CNT)/4;
	}
	else angle= TIM3->CNT/4; 
	return angle;
}
