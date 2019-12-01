
#include "Teleco.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 

void PWM_Input_Conf(TIM_TypeDef * Timer,int Arr,int Psc) {
	
	// Configuration du Timer (4)
	
	LL_TIM_InitTypeDef My_LL_Tim_Init_Struct;
	
	// Validation horloge locale
	if (Timer==TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (Timer==TIM2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (Timer==TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	// chargement structure Arr, Psc, Up Count
	My_LL_Tim_Init_Struct.Autoreload=Arr;
	My_LL_Tim_Init_Struct.Prescaler=Psc;
	My_LL_Tim_Init_Struct.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1;
	My_LL_Tim_Init_Struct.CounterMode=LL_TIM_COUNTERMODE_UP;
	My_LL_Tim_Init_Struct.RepetitionCounter=0;
	
	LL_TIM_Init(Timer,&My_LL_Tim_Init_Struct);
	
	//Enable counter
	Timer->CR1|=TIM_CR1_CEN;
	
	//Configuration de la PWM Input
	
	Timer->CCMR1|=TIM_CCMR1_CC1S_0; //1
	Timer->CCMR1&=~TIM_CCMR1_CC1S_1; //0
	
	Timer->CCER&=~TIM_CCER_CC1P; //0
	
	Timer->CCMR1&=~TIM_CCMR1_CC2S_0; //0
	Timer->CCMR1|=TIM_CCMR1_CC2S_1; //1
	
	Timer->CCER|=TIM_CCER_CC2P; //1 
	
	Timer->SMCR|=TIM_SMCR_TS_0; //1
	Timer->SMCR&=~TIM_SMCR_TS_1; //0
	Timer->SMCR|=TIM_SMCR_TS_2; //1
	
	Timer->SMCR&=~TIM_SMCR_SMS_0; //0
	Timer->SMCR&=~TIM_SMCR_SMS_1; //0
	Timer->SMCR|=TIM_SMCR_SMS_2; //1
	
	Timer->CCER|=TIM_CCER_CC1E; //1
	Timer->CCER|=TIM_CCER_CC2E; //1
	
	// hyp : pulse toutes les 20ms
	// On d�tecte des valeurs de compteurs entre 0 et 100. parce que le psc est 2/100
	// Or le signal PWM recu est mini a 1ms, et max a 2ms
	// donc on r�cup�re une valeur de compteur mini a 50.
	// 1.5ms = pas d'entr�e = CNT = 75 
	// entre 1 et 1.5 -> CNT entre 50 et 75 
	// entre 1.5 et 2 -> CNT entre 75 et 100 
	// On genere la PWM qui controlera le plateau en fonction de la valeur dans le CCR2 
	// On a 50 pts pour gerer la PWM (25 gauche , 25 droite) 
}

int PWM_get(TIM_TypeDef * Timer) {
	return LL_TIM_IC_GetCaptureCH2(Timer);
}
