#include "keypad.h"

const uint8_t matriz [4][4] = {	'A', '1', '3', '2',
	'B', '4', '6', '5',
	'C', '7', '9', '8',
	'D', '0', '#' ,'*'
};



uint8_t KEYPAD_Scan(uint8_t *key){
		// Entradas
		DDRB &= (0b11100110);
		DDRD &= (0b01111111);
		
		PORTB |= (0b00011001);
		PORTD |= (0b10000000);
		
		// Salidas
		DDRD |= (0b00111100);
		
		uint8_t mascarasPINB[3] = {0b00010000, 0b00001000, 0b00000001};
		uint8_t mascaraPIND = 0b10000000;
		
		for (uint8_t c = 0; c < 4; c++) {
			PORTD |= (0b00111100);
			PORTD &= ~(1 << (c+2));
			
			for (uint8_t f = 0; f < 3; f++) {
				if(!(PINB & mascarasPINB[f])){
					*key = matriz[f][c];
					// Esperar que se suelte el boton y delay para evitar rebotes adicionales.
					//while(!(PINB & mascarasPINB[f])){}
					//_delay_ms(100);
					return 1;
				}
			}
			if(!(PIND & mascaraPIND)){
				*key = matriz[3][c];
				// Esperar que se suelte el boton y delay para evitar rebotes adicionales.
				//while(!(PIND & mascaraPIND)){}
				//_delay_ms(100);
				return 1;
			}
		}
		return 0;
	
	
}

