#include "adcrand.h"

void adcrand_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                   // 12M  (max -- 14M)

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;             // ADC Pin PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 1; 
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);	

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);         // 使能或者失能指定的ADC的软件转换启动功能

}

// Generate random numbers
uint16_t adc_ran_num(void)
{	
	uint16_t ad = 0;
	uint16_t temp = 0;
	uint16_t i;
	for(i = 0; i < 4; i++) {
		ad = ADC_GetConversionValue(ADC1);         	
		ad = ad & 0x0f;
		temp = (temp << 4) + ad;
	}		
	return temp;
}

