#include "adcrand.h"

__IO uint16_t adc_val;

void adc1_c2_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;             // ADC Pin PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                   // Multi channel scan off.
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	  
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);		
	
    // Write down the channel you need.
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5); 
	
	ADC_DMACmd(ADC1, ENABLE);
	
     
	// Enable ADC1
	ADC_Cmd(ADC1, ENABLE);		
	
	// Enable ADC1 reset calibaration register
	ADC_ResetCalibration(ADC1);						
	// Check the end of ADC1 reset calibration register
	while(ADC_GetResetCalibrationStatus(ADC1));		
	
	// Start ADC1 calibaration
	ADC_StartCalibration(ADC1);				
	//Check the end of ADC1 calibration
	while(ADC_GetCalibrationStatus(ADC1));	
	
	// Start ADC1 Software Conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);		
}


void dma1_c1_config(void)
{	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* ADC1 DMA1 Channel Config */
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &adc_val;  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 
	DMA_InitStructure.DMA_BufferSize = 2; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
}


void adcrand_init(void)
{
    adc1_c2_config();
    dma1_c1_config();
}


// Generate random numbers
uint16_t adc_ran_num(void)
{	
	uint16_t ad = 0;
	uint16_t temp = 0;
	uint16_t i;
	for(i = 0; i < 4; i++) {
		ad = adc_val;       	
		ad = ad & 0x0f;
		temp = (temp << 4) + ad;
	}		
	return temp;
}

