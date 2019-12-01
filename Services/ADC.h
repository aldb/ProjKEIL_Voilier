#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"

//a appeler qu'une fois avant get_roulis
void init_adc(void);

//le roulis du  bateau en degrés, centré sur 0°
int get_roulis(void);

//la batterie en pourcentage (entre 0 et 100)
int get_batterie(void);

void send_angle(int angle);
