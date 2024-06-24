#define F_CPU 16000000UL

#include "DHT11.h"
#include <avr/io.h>
#include <util/delay.h>

void DHT11_init(void) {
	DHT_DDR |= (1<<PIN);	// Pin como salida
	DHT_PORT |= (1<<PIN);	// Nivel alto
}		

uint8_t DHT11_read(uint8_t *temperatura, uint8_t *humedad) {
	uint8_t bits[5];
	uint8_t i, j = 0;
	uint8_t contador = 0;
	
	// Paso 1: Enviar un puslo en bajo durante 18ms 
	DHT_PORT &= ~(1<<PIN);		// Nivel bajo
	_delay_ms(18);
	DHT_PORT |= (1<<PIN);		// Nivel alto
	DHT_DDR &= ~(1<<PIN);		// Pin como entrada
	
	//Paso 2: Esperamos 20 a 40us hasta que el DHT11 envie 0
	contador = 0;
	while(DHT_PIN & (1<<PIN)) {
		_delay_us(2);
		contador += 2;
		if (contador > 60) {
			DHT_DDR |= (1<<PIN);	// Pin como salida
			DHT_PORT |= (1<<PIN);	// Nivel alto
			return 0;
		}	
	}	
	
	//Paso 3: Esperamos 80us hasta que el DHT11 envie 1
	contador = 0;
	while(!(DHT_PIN & (1<<PIN))) {
		_delay_us(2);
		contador += 2;
		if (contador > 100) {
			DHT_DDR |= (1<<PIN);	// Pin como salida
			DHT_PORT |= (1<<PIN);	// Nivel alto
			return 0;
		}	
	}
	
	//Paso 4: Esperamos 80us hasta que el DHT11 envie 0
	contador = 0;
	while(DHT_PIN & (1<<PIN)) {
		_delay_us(2);
		contador += 2;
		if (contador > 100) {
			DHT_DDR |= (1<<PIN);	// Pin como salida
			DHT_PORT |= (1<<PIN);	// Nivel alto
			return 0;
		}
	}
	
	//Paso 5: Leemos los 5 bytes (40 bits)
	for (j = 0; j < 5; j++) {
	uint8_t result=0;
		for (i = 0; i < 8; i++) {
			while (!(DHT_PIN & (1<<PIN)));
				_delay_us(35);

			if (DHT_PIN & (1<<PIN))
				result |= (1<<(7-i));
					
			while(DHT_PIN & (1<<PIN));
		}
		
		bits[j] = result;
	}

	DHT_DDR |= (1<<PIN);	// Pin como salida
	DHT_PORT |= (1<<PIN);	// Nivel alto
	
	//Paso 6: Convertir la temperatura y humedad			
	if ((uint8_t) (bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) {		// Validar datos
		*humedad = bits[0];
		*temperatura = bits[2];
		return 1;
	} else {
		return 0;
	}
	
	return 1;
}