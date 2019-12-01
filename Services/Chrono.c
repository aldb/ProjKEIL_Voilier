// A COMPLETER

/*
Service permettant de chornom�trer jusqu'� 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 � TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "Chrono.h"
#include "MyTimer.h"
#include "ADC.h"


// variable priv�e de type Time qui m�morise la dur�e mesur�e
static Time Chrono_Time; // rem : static rend la visibilit� de la variable Chrono_Time limit�e � ce fichier 

// variable priv�e qui m�morise pour le module le timer utilis� par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par d�faut au cas o� l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.

// d�claration callback appel� toute les 10ms
void Chrono_Task_10ms(void);

//init des structures USART et clock
void USART_init(USART_TypeDef * USART);

//Envoie des donn�es par l'USART
void Send(Time Chrono_Time);


void Chrono_Conf_io(void){
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	//PC8
	// 00 = input mode
  GPIOC->CRH &= ~(1<<0);
	GPIOC->CRH &= ~(1<<1);
	// 01 = floating input
	GPIOC->CRH |= (1<<2);
	GPIOC->CRH &= ~(1<<3);
	
	//PC6
	// 00 = input mode
  GPIOC->CRL &= ~(1<<24);
	GPIOC->CRL &= ~(1<<25);
	// 10 = Pull-down input
	GPIOC->CRL &= ~(1<<26);
	GPIOC->CRL |= (1<<27);
	//Config pull-down au lieu de up
	//GPIOC->ODR &= ~(1<<6);
	
	//PC10
	// 01 = Output mode
  GPIOC->CRH |= (1<<8);
	GPIOC->CRH &= ~(1<<9);
	// 00 = Push-pull
	GPIOC->CRH &= ~(1<<10);
	GPIOC->CRH &= ~(1<<11);
	// 01 = Open-drain
	//GPIOC->CRH |= (1<<10);
	//GPIOC->CRH &= ~(1<<11);
	
	USART_init(USART2);
}

void Chrono_Background(void){
	/*
	if( GPIOC->IDR & GPIO_IDR_IDR8){//Bouton start
		Chrono_Start();
	}
	if( GPIOC->IDR & GPIO_IDR_IDR6){
		//Eteindre la led
		GPIOC->ODR &= ~(1<<10);
		Chrono_Stop();
	}
	if(!( GPIOC->IDR & GPIO_IDR_IDR13)){
		//Eteindre la led
		GPIOC->ODR &= ~(1<<10);
		Chrono_Reset();
	}*/
}

/**
	* @brief  Configure le chronom�tre. 
  * @note   A lancer avant toute autre fonction.
	* @param  Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Chrono_Conf(TIM_TypeDef * Timer)
{
	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
	
	// Fixation du Timer
	Chrono_Timer=Timer;

	// R�glage Timer pour un d�bordement � 10ms
	MyTimer_Conf(Chrono_Timer,999, 719);
	
	Chrono_Conf_io();
	
	// R�glage interruption du Timer avec callback : Chrono_Task_10ms()
	MyTimer_IT_Conf(Chrono_Timer, Chrono_Task_10ms,3);
	
	// Validation IT
	MyTimer_IT_Enable(Chrono_Timer);
}


/**
	* @brief  D�marre le chronom�tre. 
  * @note   si la dur�e d�passe 59mn 59sec 99 Hund, elle est remise � z�ro et repart
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Start(void)
{
	MyTimer_Start(Chrono_Timer);
}


/**
	* @brief  Arr�te le chronom�tre. 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Stop(void)
{
	MyTimer_Stop(Chrono_Timer);
}


/**
	* @brief  Remet le chronom�tre � 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void)
{
  // Arr�t Chrono
	MyTimer_Stop(Chrono_Timer);

	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
}


/**
	* @brief  Renvoie l'adresse de la variable Time priv�e g�r�e dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void)
{
	return &Chrono_Time;
}


/**
	* @brief  incr�mente la variable priv�e Chron_Time modulo 60mn 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Task_10ms(void)
{ if(Chrono_Time.Hund/10%2)
		//Allumer la led
		GPIOC->ODR |= (1<<10);
	else
		//Eteindre la led
		GPIOC->ODR &= ~(1<<10);
	
	Chrono_Time.Hund++;
	if (Chrono_Time.Hund==100)
	{
		Chrono_Time.Sec++;
		Chrono_Time.Hund=0;
	}
	if (Chrono_Time.Sec==60)
	{
		Chrono_Time.Min++;
		Chrono_Time.Sec=0;
	}
	if (Chrono_Time.Min==60)
	{
		Chrono_Time.Min=0;
	}
	Send(Chrono_Time);
}

void Send(Time Chrono_Time){
	uint8_t tab[10];
	tab[7] = Chrono_Time.Hund%10 + 0x30;
	tab[6] = Chrono_Time.Hund/10 + 0x30;
	tab[5] = ':';
	tab[4] = Chrono_Time.Sec%10 + 0x30;
	tab[3] = Chrono_Time.Sec/10 + 0x30;
	tab[2] = ':';
	tab[1] = Chrono_Time.Min%10 + 0x30;
	tab[0] = Chrono_Time.Min/10 + 0x30;
	tab[8] = 0x0D;//carriage return
	
	for(int i=0;i<10;i++){
		while(!LL_USART_IsActiveFlag_TXE(USART2)){}
		LL_USART_TransmitData8 (USART2, tab[i]);
	}
}
