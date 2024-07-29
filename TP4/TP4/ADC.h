#ifndef ADC
	#define ADC

void ADC_Init();
void ADC_Start_Conversion();
uint16_t ADC_GetResult();
void ADC_Enable();

#endif