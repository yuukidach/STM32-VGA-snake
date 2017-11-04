#include "adcrand.h"

void adcrand_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_ADC1,ENABLE);

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                   // 12M  (max -- 14M)

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;             // ADC Pin PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 1; 
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
	
	ADC_Cmd(ADC1,ENABLE);	

	ADC_ResetCalibration(ADC1);//重置指定的ADC的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));//获取ADC重置校准寄存器的状态
	
	ADC_StartCalibration(ADC1);//开始指定ADC的校准状态
	while(ADC_GetCalibrationStatus(ADC1));//获取指定ADC的校准程序

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能或者失能指定的ADC的软件转换启动功能

}

// Generate random numbers
uint16_t adc_ran_num(void)
{	
	uint16_t ad = 0;
	uint16_t temp = 0;
	uint16_t i;
	for(i = 0; i < 16; i++) {
		while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)); 
		ad = ADC_GetConversionValue(ADC1);         	
		ad = ad & 0x01;
		if (ad == 0x01)
			temp = temp + 1;
		temp = temp << 1;
	}		
	return(temp);
}

