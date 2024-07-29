#include "timer.h"
#include <avr/interrupt.h>

#define PIN_RED PORTB5
#define PIN_GREEN PORTB2
#define PIN_BLUE PORTB1
#define PWM_PERIOD 255
#define PWM_OFF PORTB |= (1<<PORTB5)
#define PWM_ON PORTB &=~ (1<<PORTB5)

static uint8_t PWM_DELTA = 0; // Valor inicial ROJO (mientras mas grande el valor, mas brillo)

void TIMER1_INIT(){
	TCCR1A |= (1<<WGM10);	
	TCCR1B |= (1<<WGM12);				// Fast PWM 8-bit
	TCCR1A |= (1<<COM1A1);				// Set OC1A/OC1B on compare match, clear OC1A/OC1B at BOTTOM (inverting mode)
	TCCR1A |= (1<<COM1B1);				// Set OC1A/OC1B on compare match, clear OC1A/OC1B at BOTTOM (inverting mode)
	TCCR1B |= (1<<CS01) | (1<<CS00);	// Prescaler DIV 1024, 61 hz de frecuencia
	DDRB |= (1<<PIN_GREEN);				//Pin 0C1B como salida (PB2) Verde
	DDRB |= (1<<PIN_BLUE);				//Pin 0C1A como salida (PB1) Azul

	// Ciclo de trabajo
	OCR1A = 255; // Valor inicial AZUL (mientras mas chico el valor, mas brillo)
	OCR1B = 255; // Valor inicial VERDE (mientras mas chico el valor, mas brillo)
}

// PWM por software que controla el color rojo (salida por PB5)
void PWM_soft_update(){
	static uint16_t PWM_position = 0;
	if (++PWM_position >= PWM_PERIOD){
		PWM_position = 0;
		PWM_OFF;
	} else {
		if (PWM_position < PWM_DELTA){
			PWM_ON;
		} else {
			PWM_OFF;
		}
	}
}

void set_ocr1a(uint8_t value){
	OCR1A = value;
}

void set_ocr1b(uint8_t value){
	OCR1B = value;
}

void set_pwm_soft_value(uint8_t value){
	PWM_DELTA = value;
}

uint8_t get_pwm_soft_value(){
	return PWM_DELTA;
}
