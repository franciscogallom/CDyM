#include "sEOS.h"
#include "MEF.h"
#include "WATCH.h"

#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdint.h>	
#include <stdlib.h>	

uint8_t FLAG_WATCH = 0;
uint8_t FLAG_MEF = 0;
void TIMER0_Init(){
	DDRD |= (1<<PORTD6); //waveform generator output
	//configuración del TOPE del contador TCNT0
	OCR0A=249;			//250
	TCCR0A =(1<<COM0A0) | (1<<WGM01); //modo CTC, Toggle on compare match
	TCCR0B |= (1 << CS02); //16Mhz / 256 = 62,5kHz
	TIMSK0 =(1<<OCIE0A); // habilitamos interrupución COMPA
	sei(); //Habilito las interrupciones	
	
}

ISR(TIMER0_COMPA_vect){ //Tisr = 250/62500 -> 4 ms
	static uint8_t count_MEF = 0;
	static uint8_t count_WATCH = 0;
	
	
	//Actualizo cada 100 ms flag para leer matriz
	if(++count_MEF == 25) {  //Si cada 4 ms realiza la rutina de interrupcion y quiero actualizar el estado de la MEF cada 100 ms -> 100ms/4ms = 25
		FLAG_MEF = 1;
		count_MEF=0;
	} 
	if(++count_WATCH == 250 ){  //Si quiero actualizar el reloj cada 1s -> 1000ms/4ms = 250
		FLAG_WATCH = 1;
		count_WATCH=0;
	}
 
}

void sEOS_Dispatch_Tasks(){
	if (FLAG_WATCH)
	{
		WATCH_update();
		FLAG_WATCH = 0;
	}
	if (FLAG_MEF)
	{
		MEF_update();
		FLAG_MEF = 0;
		
	}
}

