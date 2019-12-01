#include "ADC.h"
#include <math.h>

void USART_init(USART_TypeDef * USART){
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	
	LL_USART_InitTypeDef USART_InitStruct;
	LL_USART_StructInit (&USART_InitStruct);
	
	USART_InitStruct.BaudRate = 19200;
	USART_InitStruct.Parity = 0;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX;
	
	LL_USART_Init (USART, &USART_InitStruct);
	LL_USART_Enable(USART);
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	LL_GPIO_StructInit (&GPIO_InitStruct);
	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	
	LL_GPIO_Init (GPIOA, &GPIO_InitStruct);
}

void init_adc(){
	
	RCC->CFGR &= ~(1<<14);//ADCPRE prescaler clock = 72MHz/6
	RCC->CFGR |= (1<<15);
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	
	ADC1->CR2 |= (1<<0);//ADON =1	On
	
	ADC1->CR2 |= (1<<20);//EXTTRIG =1 External trigger conversion mode for regular channels
	
	ADC1->SQR1 &= ~(1<<20);//Channel =0000 (1 convertion)
	ADC1->SQR1 &= ~(1<<21);
	ADC1->SQR1 &= ~(1<<22);
	ADC1->SQR1 &= ~(1<<23);
	
	ADC1->CR2 |= (1<<17);//ext trigger = SWSTART (111)
	ADC1->CR2 |= (1<<18);
	ADC1->CR2 |= (1<<19);
	
	ADC1->SMPR2 &= ~(1<<0);//sample time = 13,5 cycles
	ADC1->SMPR2 |= (1<<1);
	ADC1->SMPR2 &= ~(1<<2);
	
	ADC1->CR2 |= (1<<2);//calibration ON
	while(ADC1->CR2 & (1<<2));
	
	//USART_init(USART1);
}

int get_roulis(){
	ADC1->SQR3 = 10;//ADC_IN10 sur PC0
	ADC1->CR2 |= (1<<22);//SWSTART : Starts conversion of regular channels
	while( !(ADC1->SR & (1<<1)) );//tant que la convesrion n'et pas finie
	ADC1->SR &= ~(1<<1);//eteindre EOC
	
	float angle=ADC1->DR;
	angle = asin((angle-1690)/1300)*208;
	return (int)angle;
}

int get_batterie(){
	ADC1->SQR3 = 12;//ADC_IN12 sur PC2
	ADC1->CR2 |= (1<<22);//SWSTART : Starts conversion of regular channels
	while( !(ADC1->SR & (1<<1)) );//tant que la convesrion n'et pas finie
	ADC1->SR &= ~(1<<1);//eteindre EOC
	
	int batterie=ADC1->DR;
	batterie = batterie/9 - 20;
	if(batterie<0)batterie=0;
	if(batterie>100)batterie=100;
	return batterie;
}

void send_angle(int a){
	uint8_t tab[6];
	tab[4] = a%10 + 0x30;
	tab[3] = a/10%10 + 0x30;
	tab[2] = a/100%10 + 0x30;
	tab[1] = a/1000%10 + 0x30;
	tab[0] = a/10000%10 + 0x30;
	tab[5] = 0x0D;//carriage return
	
	for(int i=0;i<6;i++){
		while(!LL_USART_IsActiveFlag_TXE(USART1)){}
		LL_USART_TransmitData8 (USART1, tab[i]);
	}
}

