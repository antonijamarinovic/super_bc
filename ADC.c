#include "ADC.h"
#include "math.h"




void ADC_Init123(void){
	
GPIO_InitTypeDef			GPIO_InitStruct;
ADC_InitTypeDef				ADC_InitStruct;
ADC_CommonInitTypeDef		ADC_CommonInitStruct;
NVIC_InitTypeDef			NVIC_InitStruct;


		// enable the ADC interface and GPIO clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 			//ADC's APB interface clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);			// ADC2 interface clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);			// ADC3 interface clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);			// ADC GPIOs clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);			// ADC GPIOs clock
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);			// DMA clock
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);			// ADC GPIOs clock
			// Configure ADC pins in analog mode
		GPIO_InitStruct.GPIO_Pin = UC1 | UC2 | UC3 | UC4 ;			// port C ADC inputs
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;							// analog mode
		GPIO_Init(GPIOC, &GPIO_InitStruct);										// set GPIOC
	
		// Configure ADC pins in analog mode
		GPIO_InitStruct.GPIO_Pin = UOUT | UBAT | UTEMPBAT | UTEMPOK | UR8 | UR16; 	// port A ADC inputs
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;											// analog mode
		GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
		
		// Select the Independent mode ADC regular channels features
		ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;							// 
		ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;						// pclk2/2 (60MHz?/2)
		ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;				// 3 half-words one by one
		//ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	// for interleaved mode
		ADC_CommonInit(&ADC_CommonInitStruct);
		
		
		// Configure the ADC3
		ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;					// 12 bit
		ADC_InitStruct.ADC_ScanConvMode = ENABLE;										// DMA cares for results 
		ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;							// Continuous mode disabled
		ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
		ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;							// Right alignement
		ADC_InitStruct.ADC_NbrOfConversion = 4;								
		ADC_Init(ADC3, &ADC_InitStruct);
		
		
		// Configure regular channel group
		ADC_RegularChannelConfig(ADC3,												// ADC peripheral
								 ADC_Channel_10,													// ADC channel to configure UC1
								 1,																				// rank, first in order
								 ADC_SampleTime_28Cycles);								// 28 sampling cycles
		ADC_RegularChannelConfig(ADC3,												// ADC peripheral
								 ADC_Channel_11,													// ADC channel to configure UC2
								 2,																				// rank, second in order
								 ADC_SampleTime_28Cycles);								// 28 sampling cycles
		ADC_RegularChannelConfig(ADC3,												// ADC peripheral
								 ADC_Channel_12,													// ADC channel to configure UC3
								 3,																				// rank, third in order
								 ADC_SampleTime_28Cycles);								// 28 sampling cycles
		ADC_RegularChannelConfig(ADC3,												// ADC peripheral
								 ADC_Channel_13,													// ADC channel to configure UC4
								 4,																				// rank, fourth in order
								 ADC_SampleTime_28Cycles);								// 28 sampling cycles
								 
					
		
	// Configure the ADC2
		ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;							// 12 bit
		ADC_InitStruct.ADC_ScanConvMode = ENABLE;												// DMA cares for results 
		ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;								// Continuous mode disabled
		ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
		ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;							// Right alignement
		ADC_InitStruct.ADC_NbrOfConversion = 4;									 
		ADC_Init(ADC2, &ADC_InitStruct);
		
		// Configure regular channel group
		ADC_RegularChannelConfig(ADC2,												// ADC peripheral
								 ADC_Channel_0,														// ADC channel to configure UOUT
								 1,																				// rank, first in order
								 ADC_SampleTime_28Cycles);								// 28 sampling cycles
		ADC_RegularChannelConfig(ADC2,												// ADC peripheral
								 ADC_Channel_1,														// ADC channel to configure UBAT
								 2,																				// rank, second in order
								 ADC_SampleTime_28Cycles);								// 28 sampling cycles
		ADC_RegularChannelConfig(ADC2,												// ADC peripheral
								 ADC_Channel_6,														// ADC channel to configure UTEMPBAT
								 3,																				// rank, third in order
								 ADC_SampleTime_28Cycles);								// 28 sampling cycles
		ADC_RegularChannelConfig(ADC2,												// ADC peripheral
								 ADC_Channel_7,														// ADC channel to configure UTEMPOK
								 4,																				// rank, fourth in order
								 ADC_SampleTime_28Cycles);								// 28 sampling cycles

		
		
				// ADC1 injected group configure
				
				
		ADC_InitStruct.ADC_ScanConvMode = ENABLE;		
    ADC_Init(ADC1, &ADC_InitStruct);		
		
		ADC_InjectedSequencerLengthConfig(ADC1, 2);														// 2 sequences  2->1
		ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_TRGO);				    // TIM1 TRGO (update event)
		ADC_ExternalTrigInjectedConvEdgeConfig(ADC1, ADC_ExternalTrigInjecConvEdge_Rising);			// rising edges
		ADC_InjectedDiscModeCmd(ADC1, ENABLE);																// enable discontinuous mode
		
		
		// Configure injected channel group
		ADC_InjectedChannelConfig(ADC1,												// ADC peripheral
								 ADC_Channel_3,														
								 1,																				
								 ADC_SampleTime_15Cycles);								// 15 sampling cycles
    ADC_InjectedChannelConfig(ADC1,												// ADC peripheral
    						ADC_Channel_2,														
								2,																				
							  ADC_SampleTime_15Cycles);									// 15 sampling cycles
	
	
		// enable DMA for ADCs
      ADC_DMACmd(ADC2, ENABLE);
      ADC_DMACmd(ADC3, ENABLE);
		
		// Configure priority bits
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		// 4 bits preemptive
		
    // Enable interrupts for ADCs
		ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);
		
		// Configure NVIC for ADC1
		NVIC_InitStruct.NVIC_IRQChannel = ADC_IRQn;								// ADC channel
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;		// highest priority
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;						// highest subpriority
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
		
		// Set ADC ON
		ADC_Cmd(ADC1, ENABLE);					// enable ADC1
		ADC_Cmd(ADC2, ENABLE);					// enable ADC2
		ADC_Cmd(ADC3, ENABLE);					// enable ADC3
		
}

struct
{
		int16_t PRIM;
		int16_t SEC;
		
}current;

uint16_t pom;

struct
{
		uint16_t PRIM;
		uint16_t SEC;
		
}voltage;

volatile uint16_t k = 0;
volatile uint16_t c1=0;
volatile uint16_t brojac1=0;
volatile uint16_t brojac2=0;
volatile uint16_t ref_PRIM=0;
volatile uint16_t ref_SEC=0;
volatile uint16_t f1=0; // status flag,  becomes 1 when the reference voltage is set
volatile uint16_t f2=0; // status flag,  becomes 1 when reference voltage is set

void  ADC_IRQHandler(void) {
	
	if(ADC_GetITStatus(ADC1, ADC_IT_JEOC) == SET){
		GPIO_ToggleBits(GPIOC, GPIO_Pin_13);
		if(k%2==0){  
			k++;
			voltage.PRIM = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1)*3232/0xFFF;		// get value from ADC1 injected channel
     		
		    if(brojac1<N){												
				   ref_PRIM+=voltage.PRIM;
				   brojac1++;
			}
			  if (brojac1==N && !(f1)){	//set reference voltage of primar before driver is enabled
			     ref_PRIM= ref_PRIM/N;
			
		       CONTROL_A (1);
			     f1=1;
			}
	  current.PRIM = (voltage.PRIM-ref_PRIM)*20;  // 20=1000/50 (struja u mA- zato se mnozilo s 1000, 50 je zbog pojacala koji je 10x i otpora od 5 mOhma) 
			
		}else{
			  k--;
			  voltage.SEC = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_2)*3232/0xFFF;
			  
      	if(brojac2<N){											
				ref_SEC+=voltage.SEC;
				brojac2++;
			}			
		
			
	      if (brojac2==N && !(f2)){ //set reference voltage of primar before driver is enabled
				ref_SEC= ref_SEC/N;
				
				CONTROL_B (1);
				f2=1;
			}
     	current.SEC = (voltage.SEC-ref_SEC)*20;	// 20=1000/50 (struja u mA-zato se mnozilo s 1000, 50 je zbog pojacala koji je 10x i otpora od 5 mOhma) 
			
		}
		//
		ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC);	
	
	}
}

void current_prim_sec(void){
	char buff1[500];
  sprintf(buff1, "Struja primara : %d [mA]\r\n", current.PRIM);
  send_message(buff1);
	
	sprintf(buff1, "Struja sekundara : %d [mA]\r\n", current.SEC);
  send_message(buff1);
}

