#define F_CPU 16000000UL

#include "sEOS.h"
#include "MEF.h"
#include "UART.h"

#include <avr/interrupt.h>
#include <stdint.h>	
#include <util/delay.h>
#include <stdlib.h>	

uint8_t FLAG_MEF = 0;

void TIMER0_Init(){
	DDRD |= (1<<PORTD6);				// waveform generator output
	OCR0A = 249;						// configuración del TOPE del contador TCNT0 -- 250
	TCCR0A = (1<<COM0A0) | (1<<WGM01);	// modo CTC, Toggle on compare match
	TCCR0B |= (1 << CS02);				// 16Mhz / 256 = 62,5kHz
	TIMSK0 = (1<<OCIE0A);				// habilitamos interrupución COMPA
}

ISR(TIMER0_COMPA_vect){ // Tisr = 250 / 62500 -> 4 ms
	static int count_MEF = 0;
	
	// Actualizo cada 2000ms para leer la temperatura y la humedad
	// Si cada 4 ms realiza la rutina de interrupcion y quiero actualizar el estado de la MEF cada 2000 ms -> 2000ms / 4ms = 500
	if(++count_MEF == 500) {  
		FLAG_MEF = 1;
		count_MEF = 0;
	}
}

void sEOS_Dispatch_Tasks(){
	UART_Update();
	
	if (FLAG_MEF){
		_delay_ms(1);
		MEF_update();
		FLAG_MEF = 0;
	}
}