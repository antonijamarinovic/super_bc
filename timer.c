#include <timer.h>
#include <DMA.h>
#include <usart.h>

int i=0;
uint16_t CCR1_Val = 3360;
uint16_t CCR2_Val = 840;
uint16_t PrescalerValue = 0;
uint16_t PrescalerValue1 = 1;


//timer interrupt za citanje buffera svake sekunde
void timer2_init(void){ 
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // init def struct for timer
	NVIC_InitTypeDef NVIC_InitStructure; // init def struct for NVIC
	uint16_t TimerPeriod; // 16-bit value because ARR register is 16-bit (although TIM2 is 32-bit!)
	
	RCC_ClocksTypeDef RCC_Clocks; // for reading current clock setting - useful
	//	uint32_t APB1_CLK; // APB1 clock - max. 42 MHz!
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // enable TIM2 peripheral clock
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); // ensure that interrupt is disabled
	
	RCC_GetClocksFreq(&RCC_Clocks); // fill query struct
	//	APB1_CLK = RCC_Clocks.PCLK1_Frequency; // WARNING: APB1 is "slow" peripheral interface that with
	TimerPeriod = 16799;
	// Time Base configuration
	TIM_TimeBaseStructure.TIM_Prescaler = 2499;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // counts from 0 to autoreload, and then
	//back to 0
	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	// set TIM2 IRQ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// Enable the TIM2 global Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //lower priority than USART3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	// TIM2 counter enable
	TIM_Cmd(TIM2, ENABLE);

}


//komplementarni PWM signali duty cycle 50%
void TIM1_Pin_Config(void)
{

GPIO_InitTypeDef         	GPIO_InitStruct; 
TIM_OCInitTypeDef 				TIM_OCInitStruct;
TIM_BDTRInitTypeDef				TIM1_BDTR_InitStruct;
TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;

	
		// enable peripheral clocks
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	
				//configure pin		 
        GPIO_InitStruct.GPIO_Pin = PWM1A;
        GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF; 
        GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_100MHz; 
        GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
        GPIO_Init(GPIOA, &GPIO_InitStruct);    
				
				//connect TIM1 pins to AF
				GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
		
				//configure pins	
	      GPIO_InitStruct.GPIO_Pin = PWM1B;
        GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF; 
        GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_100MHz; 
        GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
        GPIO_Init(GPIOB, &GPIO_InitStruct);  
				
				//connect TIM1 pins to AF		
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_TIM1);
		
	
		// Configure Time Base with desired parameters
		TIM_TimeBaseStruct.TIM_Prescaler = PrescalerValue1;						// Internal clock = APBx*2/(prescaler+1) (168 MHz)
		TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_CenterAligned1;			// Timer counts up and down
		TIM_TimeBaseStruct.TIM_Period = 1679;												// ARR value, timer counts to this value (168MHz/25kHz-1)
		TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV2;					// clock division for BDTR (PWM only), specifies the clock division for dead-time (clk_internal/2)
		TIM_TimeBaseStruct.TIM_RepetitionCounter = 1;							// generate update event on every downcount (RC+1), TIM1_RCR
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct); 									// set timer for regular group
				
	  TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);						// update event as a trigger output
	
		//TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;			// active high
		//TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;		// low idle state
		//TIM_OC1Init(TIM1, &TIM_OCInitStruct);												// set TM1 channel 1
	
		// TIM_ Output Compare configuration
		TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;				 				// set channel high on match
		TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;		// enable output state
		TIM_OCInitStruct.TIM_Pulse = CCR2_Val;												// CCR (0x0000-0xFFFF) set high when match
		TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;				// active high
		TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;			// low idle state
		
		TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;		// enable output state 
		TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;				// high active state
		TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;			// low idle state
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
		TIM_OC1Init(TIM1, &TIM_OCInitStruct);													// set TM1 channel 1
		
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
		

		
		// Set dead-time
		TIM1_BDTR_InitStruct.TIM_OSSRState = TIM_OSSRState_Enable;					// outputs are enabled with their inactive level
		TIM1_BDTR_InitStruct.TIM_OSSIState = TIM_OSSIState_Enable;					// outputs are forced first with their idle level
		TIM1_BDTR_InitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;						// no bit is write protected
		TIM1_BDTR_InitStruct.TIM_DeadTime = 0x8B;	//0b10001011 (RM0090 p.586)		// specifies the delay time between on and off state 0x00-0xFF (2.5us)
		TIM1_BDTR_InitStruct.TIM_Break = TIM_Break_Disable;							// disable break
		TIM1_BDTR_InitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;		// enable automatic output
		TIM_BDTRConfig(TIM1, &TIM1_BDTR_InitStruct);								// set BDTR register
	
		// PWM main output enable
		TIM_CtrlPWMOutputs(TIM1, ENABLE);
		
		TIM_Cmd(TIM1, ENABLE);

}


void TIM2_IRQHandler(void){

	char c;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		while( USART3_Dequeue(&c)){
				if(c=='u'){
					
					voltage_supercond();
	
				} else if(c=='t'){
	
					temp_uout_ubat();
					
				}	else if (c=='i'){	
					current_prim_sec();
		}
	}
}
}
