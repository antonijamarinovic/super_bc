#include "main.h"

int main(){

//je li radi ovako?
	
ADC_Init123();	
TIM1_Pin_Config();
GPIO_init();

DMA_init();

USART3_Init();
timer2_init();
CONTROL_A (1);
CONTROL_B (1);

ADC_SoftwareStartConv(ADC3);
	
while(1){
}
}
