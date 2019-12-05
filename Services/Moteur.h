#include "stm32f103xb.h" 


/**
	* @brief  Active l'horloge et r�gle l'ARR et le PSC du timer vis� + configure le mode PWM Output sur le timer visé:
				-> Initialisation des structures  pour configurer le port GPIO A1 (vitesse) et le port A2 (sens) pour la PWM output.
				-> Initialisation des registres GPIO avec les informations des structures GPIO
				->  Genere un compteur avec ARR/PSC => 1ms
				

				
  * @note   Fonction de configuration à lancer avant.
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre (ici  TIM2 est utilisé).
																
	* 				@param int Arr   : valeur � placer dans ARR
	*					@param int Psc   : valeur � placer dans PSC
	
  * @retval None
  */
void PWM_Output_Conf(TIM_TypeDef * Timer, int ARR, int PSC);


/**
	* @brief  génération de la pwm output:
				->  On g�n�re une PWM en fonction de la valeur renvoyé par la télécommande:
	  if (Value >= 50 && Value <=78) { // On tourne vers la gauche
			// On genere une PWM
		}
	  if (Value == 78) { // Il se passe rien
		  
		}
		if (Value >= 78) { // On tourne vers la droite
	   
  * @note   Fonction � lancer après la configuration de la pwm output, et après avoir eu la valeur de la télécommande.
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre( ici TIM2 est utilisé)
	* @param	float TelecomValue: indique la valeur de la telecommande reçu par PWM input auparavant.
  * @retval None
  */
void PWM_Output_set(TIM_TypeDef * Timer,float TelecomValue);
