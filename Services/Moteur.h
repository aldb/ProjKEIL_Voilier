#include "stm32f103xb.h" 


/**
	* @brief  Active l'horloge et règle l'ARR et le PSC du timer visé + configure le mode PWM Output sur le timer visé
  * @note   Fonction à lancer avant toute autre.
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4.
																Dans le cadre du projet, on doit utiliser TIM4
	* 				int Arr   : valeur à placer dans ARR
	*					int Psc   : valeur à placer dans PSC
  * @retval None
  */
void PWM_Output_Conf(TIM_TypeDef * Timer, int ARR, int PSC);


/**
	* @brief  Active l'horloge et règle l'ARR et le PSC du timer visé + configure le mode PWM input sur le timer visé
  * @note   Fonction à lancer avant toute autre.
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4.
																Dans le cadre du projet, on doit utiliser TIM4
	* 				int Arr   : valeur à placer dans ARR
	*					int Psc   : valeur à placer dans PSC
  * @retval None
  */
void PWM_Output_set(TIM_TypeDef * Timer,float TelecomValue);
