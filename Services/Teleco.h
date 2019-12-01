#include "stm32f103xb.h" 
/**
	* @brief  Active l'horloge et r�gle l'ARR et le PSC du timer vis� + configure le mode PWM input sur le timer vis�
  * @note   Fonction � lancer avant toute autre.
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4.
																Dans le cadre du projet, on doit utiliser TIM4.
	* 				int Arr   : valeur � placer dans ARR
	*					int Psc   : valeur � placer dans PSC
  * @retval None
  */
void PWM_Input_Conf(TIM_TypeDef * Timer,int Arr, int Psc );

/**
  * @brief  R�cup�re la valeur dans le registre CCR2 de TIM4 (valeur du compteur lors d'un falling edge)
  * @note   
	* @param 
  * @retval la valeur (int) du compteur qui nous indique dans quelle direction tourner le plateau
  */
int PWM_get(TIM_TypeDef * Timer);
