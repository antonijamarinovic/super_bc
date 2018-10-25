#include "DMA.h"
#include "usart.h"
#include <math.h>


#include <stdio.h>
#define A 10.888
#define B 0.00347
#define C 1777.3

DMA_InitTypeDef			DMA_InitStruct;
NVIC_InitTypeDef			NVIC_InitStruct;
uint16_t ADCConvertedValue1[4];
uint16_t ADCConvertedValue2[4];
float y;
float z;

//DMA init
void DMA_init(void) {
		
	  //configure DMA - ADC3
		DMA_InitStruct.DMA_Channel = DMA_Channel_2;
		DMA_InitStruct.DMA_PeripheralBaseAddr = ADC3_DR_ADDRESS;
		DMA_InitStruct.DMA_Memory0BaseAddr =(uint32_t)&ADCConvertedValue1[0];
		DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStruct.DMA_BufferSize = 4;
		DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStruct.DMA_Priority = DMA_Priority_High;
		DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
		DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
		DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream1, &DMA_InitStruct);
		//* Enable DMA2 Stream1 */  Stream1 higher priority than Stream2...
		DMA_Cmd(DMA2_Stream1, ENABLE);
	
	
		  //configure DMA - ADC2
		DMA_InitStruct.DMA_Channel = DMA_Channel_1;
		DMA_InitStruct.DMA_PeripheralBaseAddr = ADC2_DR_ADDRESS;
		DMA_InitStruct.DMA_Memory0BaseAddr =(uint32_t)&ADCConvertedValue2[0];
		DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStruct.DMA_BufferSize = 4;
		DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStruct.DMA_Priority = DMA_Priority_High;
		DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
		DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
		DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA2_Stream2, &DMA_InitStruct);
		/* Enable DMA2 Stream2 */
		DMA_Cmd(DMA2_Stream2, ENABLE);
		
		DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);
		DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);
		
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		// 4 bits preemptive,0 bits for subpriority
		
			// Configure NVIC(nested vectored interrupt controller) for ADC
		NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream1_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 7;	// highest priority
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;					// highest subpriority
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
			// Configure priority bits
		
			// Configure NVIC(nested vectored interrupt controller) for ADC
		NVIC_InitStruct.NVIC_IRQChannel = DMA2_Stream2_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 7;	// highest priority
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;					// highest subpriority
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
		
		// enable requests after last transfer		
		//ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
		ADC_DMARequestAfterLastTransferCmd(ADC2,ENABLE);
		ADC_DMARequestAfterLastTransferCmd(ADC3,ENABLE);
}


struct{
		uint16_t U_OUT;
		uint16_t U_BAT;
		uint16_t U_TEMPBAT;
		uint16_t U_TEMPOK;
}Voltage;

struct{
		uint16_t UC_1;
		uint16_t UC_2;
		uint16_t UC_3;
		uint16_t UC_4;
}SuperConds;

void DMA2_Stream1_IRQHandler(void) {

	if(DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF1) == SET){
	
	SuperConds.UC_1 = ADCConvertedValue1[0]*3220/0xFFF; 
	SuperConds.UC_2 = ADCConvertedValue1[1]*3220/0xFFF; 
	SuperConds.UC_3 = ADCConvertedValue1[2]*3220/0xFFF;
	SuperConds.UC_4 = ADCConvertedValue1[3]*3220/0xFFF;
		
		
	if ((SuperConds.UC_1+SuperConds.UC_2+SuperConds.UC_3+SuperConds.UC_4)>= 9.9){
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);
	}else{
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);
	}
	
	DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1);		// clear pending bit
	ADC_SoftwareStartConv(ADC2);
	}	
	
}	
	
void DMA2_Stream2_IRQHandler(void) {

	 if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2) == SET){
		Voltage.U_OUT = ADCConvertedValue2[0]*3220/0xFFF; 
		Voltage.U_BAT = ADCConvertedValue2[1]*3220/0xFFF; 
		Voltage.U_TEMPBAT = ADCConvertedValue2[2]*3220/0xFFF; 
    Voltage.U_TEMPOK = ADCConvertedValue2[3]*3220/0xFFF;    

		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);			// clear pending bit
		ADC_SoftwareStartConv(ADC3);
	}
}


void voltage_supercond(void){
	
	char buff1[500];
  sprintf(buff1, "Napon na superkondenzatoru UC1: %d [mV]\r\n",SuperConds.UC_1);
  send_message(buff1);
	
	sprintf(buff1, "Napon na superkondenzatoru UC2: %d [mV]\r\n",SuperConds.UC_2);
  send_message(buff1);
	
	sprintf(buff1, "Napon na superkondenzatoru UC3: %d [mV]\r\n",SuperConds.UC_3);
  send_message(buff1);
	
	sprintf(buff1, "Napon na superkondenzatoru UC4: %d [mV]\r\n",SuperConds.UC_4);
  send_message(buff1);
}


void temp_uout_ubat(void){
	
	char buff2[200];
	sprintf(buff2, "Napon na izlazu UOUT: %d [mV]\r\n", Voltage.U_OUT);
  send_message(buff2);
	
	sprintf(buff2, "Napon na baterijama UBAT: %d [mV]\r\n",Voltage.U_BAT);
  send_message(buff2);
	
	y = ((A-sqrt(A*A+4*B*(C-(Voltage.U_TEMPBAT))))/(2*(-B)) + 30);
	sprintf(buff2, "Temperatura baterija: %4.1f[Celsius]\r\n",y);
  send_message(buff2);
	
	
	z=((A-sqrt(A*A+4*B*(C-(Voltage.U_TEMPOK))))/(2*(-B)) + 30); //datasheet
  sprintf(buff2, "Temperatura okoline: %4.1f [Celsius]\r\n", z);
  send_message(buff2);
}
