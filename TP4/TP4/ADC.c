#include <avr/io.h> 
#include "ADC.h"

void ADC_Init() {
	ADMUX |= (1<<MUX1)|(1<<MUX0); // ADC3 como entrada analógica
	ADMUX |= (1<<REFS0);
	ADMUX &=~ (1<<ADLAR); // Justificacion derecha
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // Establecer prescaler de 128, teniendo muestro de 125kHz
	DIDR0 |= (1<<PORTC3);
	DDRC &=~ (1<<PORTC3); // Configurar PC3 entrada (PC3)
}

void ADC_Enable() {
	ADCSRA |= (1<<ADEN);	// Habilitar conversor ADC 
}

void ADC_Disable() {
	ADCSRA &= ~(1<<ADEN); // Deshabilitar conversor ADC
}

void ADC_Start_Conversion() {
	ADCSRA |= (1<<ADSC); // Start conversion
}

uint16_t ADC_GetResult() {
	uint16_t aux = ADC;
	return aux;
}

