/* timer.h */
#ifndef TIMER_H
#define TIMER_H

#define PWM1A			GPIO_Pin_8
#define PWM1B			GPIO_Pin_13

#include <stm32f4xx.h> // common stuff
#include <stm32f4xx_gpio.h> // gpio control
#include <stm32f4xx_rcc.h> // reset anc clocking
#include <stm32f4xx_tim.h> // timers

void timer2_init(void);
void TIM1_Pin_Config(void);
uint32_t timer2_get_millisec(void);
void timer2_wait_millisec(uint32_t ms);
#endif
