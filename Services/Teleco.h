#include "stm32f103xb.h" 
/**
	* @brief  Active l'horloge et règle l'ARR et le PSC du timer vis� + configure le mode PWM input sur le timer visé
  * @note   Fonction à lancer au début (avant la boucle while) pour configurer le PWM Input Mode.
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4.
																Dans le cadre du projet, on doit utiliser TIM4 pour pouvoir recupérer les inputs de la télécommande.
	* 				int Arr   : valeur à placer dans ARR 
	*					int Psc   : valeur à placer dans PSC
  * @retval None
  */
void PWM_Input_Conf(TIM_TypeDef * Timer,int Arr, int Psc );

/**
  * @brief  Récupère la valeur du compteur dans le registre CCR2 de Timer (valeur du compteur lors d'un falling edge)
  * @note   A utiliser dans la boucle while(1)
	* @param  TIM_TypeDef Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4.
																Dans le cadre du projet, on doit utiliser TIM4 car c'est le timer sur lequel on a configuré la PWM Input.
  * @retval la valeur (int) du compteur qui nous indique dans quelle direction tourner le moteur du plateau.
  *         Cette valeur reflète un input (gauche, droite ou rien) sur la télécommande.
  */
int PWM_get(TIM_TypeDef * Timer);
