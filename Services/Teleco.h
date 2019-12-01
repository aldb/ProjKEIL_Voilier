#include "stm32f103xb.h" 
/**
	* @brief  Active l'horloge et règle l'ARR et le PSC du timer visé + configure le mode PWM input sur le timer visé
  * @note   Fonction à lancer avant toute autre.
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4.
																Dans le cadre du projet, on doit utiliser TIM4.
	* 				int Arr   : valeur à placer dans ARR
	*					int Psc   : valeur à placer dans PSC
  * @retval None
  */
void PWM_Input_Conf(TIM_TypeDef * Timer,int Arr, int Psc );

/**
  * @brief  Récupère la valeur dans le registre CCR2 de TIM4 (valeur du compteur lors d'un falling edge)
  * @note   
	* @param 
  * @retval la valeur (int) du compteur qui nous indique dans quelle direction tourner le plateau
  */
int PWM_get(TIM_TypeDef * Timer);
