#ifndef GPIO_H
#define  GPIO_H

#include <stm32f4xx.h> // common stuff


#define ENA		GPIO_Pin_10
#define ENB		GPIO_Pin_11
#define OVERVOLT GPIO_Pin_6
#define LED GPIO_Pin_13
void GPIO_init(void);
void CONTROL_A (int i);
void CONTROL_B (int i);

#endif
