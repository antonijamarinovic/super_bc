#include <GPIO.h>
#include <stm32f4xx_gpio.h>

void GPIO_init(void){
	
	GPIO_InitTypeDef         	GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		
				//configure pins ENA & ENB 
        GPIO_InitStruct.GPIO_Pin = ENA | ENB;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; 
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; 
        GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
        GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	      //configure pin for overvoltage (>9.9V)
	
	      GPIO_InitStruct.GPIO_Pin = OVERVOLT | LED;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; 
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; 
        GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
        GPIO_Init(GPIOC, &GPIO_InitStruct); 
				
				

	
}

void CONTROL_A (int i){
	if(i==1){
		GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_SET);
	}else if(i==0){
		GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET);
	}	
}

void CONTROL_B (int i){
	if(i==1){
		GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET);
	}else if(i==0){
		GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET);
	}	
}
