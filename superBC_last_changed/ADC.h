#ifndef ADC_H
#define ADC_H

// peripheral headers
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"

#include "stdio.h"
#include "DMA.h"

#define UC1					GPIO_Pin_0
#define UC2					GPIO_Pin_1
#define UC3					GPIO_Pin_2
#define UC4					GPIO_Pin_3

#define UOUT				GPIO_Pin_0
#define UBAT				GPIO_Pin_1
#define UTEMPBAT		GPIO_Pin_6
#define UTEMPOK			GPIO_Pin_7

#define UR8					GPIO_Pin_2
#define UR16				GPIO_Pin_3



void ADC_Init123(void);
void DMA2_Stream1_IRQHandler(void);
void ADC_IRQHandler(void) ;

#endif
