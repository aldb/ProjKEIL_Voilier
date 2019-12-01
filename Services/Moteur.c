#include "Moteur.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_gpio.h"
#include <stdlib.h>
int globalV ;


//Utilisez le timer2
void PWM_Output_Conf(TIM_TypeDef * Timer, int ARR, int PSC) {
		
	
		// Validation horloge locale
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Clock enable
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		
	
		// Init structure pour configurer le port A1 pour la PWM output
		LL_GPIO_InitTypeDef GPIO_Init = {	
			LL_GPIO_PIN_1,
			LL_GPIO_MODE_ALTERNATE,
			LL_GPIO_MODE_OUTPUT_2MHz, //
			LL_GPIO_OUTPUT_PUSHPULL,
			LL_GPIO_PULL_UP
		};
		// init structure configurer le GPIOA2 output
		LL_GPIO_InitTypeDef GPIO_Init2 = {	
			LL_GPIO_PIN_2,
			LL_GPIO_MODE_OUTPUT,
			LL_GPIO_MODE_OUTPUT_2MHz, //
			LL_GPIO_OUTPUT_PUSHPULL,
			LL_GPIO_PULL_UP
		};
		// Initialisation des registres GPIO avec les informations de les structures initiées avant.
		LL_GPIO_Init( GPIOA, &GPIO_Init ); 
		LL_GPIO_Init( GPIOA, &GPIO_Init2 ); 

	LL_TIM_InitTypeDef TIM_InitStruct; // genere un compteur avec ARR/PSC => 1ms
	TIM_InitStruct.Prescaler         = PSC;
  TIM_InitStruct.CounterMode       = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload        = ARR;
  TIM_InitStruct.ClockDivision     = LL_TIM_CLOCKDIVISION_DIV1;
  TIM_InitStruct.RepetitionCounter = (uint8_t)0x00;	
	
	LL_TIM_Init(Timer,&TIM_InitStruct);// Initialisation des registres avec la structure initialiser avant
	LL_TIM_CC_EnableChannel(Timer,LL_TIM_CHANNEL_CH2);
	LL_TIM_OC_SetMode(TIM2,LL_TIM_CHANNEL_CH2,LL_TIM_OCMODE_PWM1);
	LL_TIM_OC_SetCompareCH2(Timer,0); // initialisation avec une vitesse à 0
	LL_TIM_EnableAllOutputs(Timer);
	LL_TIM_EnableCounter(Timer);
}

void PWM_Output_set(TIM_TypeDef * Timer,float TelecomValue ) {
	// On génère une PWM en fonction de la valeur du compteur 
		/*
	  if (Value >= 50 && Value <=75) { // On tourne vers la gauche
			// On genere une PWM
		}
	  if (Value == 75) { // Il se passe rien
		  
		}
		if (Value >= 75) { // On tourne vers la droite
	  */                                                                                                                                                  
	int vitesse =abs((int)(TelecomValue) - 78); // Calcul de la valeur a appliqué en sortie
	globalV = vitesse ;
	if(TelecomValue < 78) // Clacul du sens ( On recoit un signal entre 5% et 10% (float 5->10) Si inf a 7.5 alors sens = gauche sinon droite
	{
		LL_GPIO_ResetOutputPin (GPIOA,LL_GPIO_PIN_2);// sens = gauche
	}
	else
	{
		LL_GPIO_SetOutputPin (GPIOA,LL_GPIO_PIN_2); // sens = droite
	}	
	vitesse*=40;	// Remise à l'echelle car ARR=999 et vitesse comprise entre 0 et 25
	globalV=vitesse; // test dans le watch.
	LL_TIM_OC_SetCompareCH2(Timer,vitesse); // compare -> Tant que le compteur est inf a vivi alors état haut puis état bas
		
}	
