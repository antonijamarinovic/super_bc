#include "main.h"

int main(){

CONTROL_A (0);
CONTROL_B (0);

ADC_Init123();	
TIM1_Pin_Config();
GPIO_init();

DMA_init();

USART3_Init();
timer2_init();


ADC_SoftwareStartConv(ADC3);
	
while(1){
}
}
