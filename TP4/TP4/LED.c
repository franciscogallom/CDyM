#include "LED.h"

const uint8_t RED_BRIGHT = 250;	
const uint8_t BLUE_BRIGHT = 0;	
const uint8_t GREEN_BRIGHT = 0;

void LED_Update(uint8_t value, char *color){

		// Modificar brillo del ROJO
		if (strcmp(color, "R") == 0) {
			set_pwm_soft_value(value );
		}

		// Modificar brillo del VERDE		
		if (strcmp(color, "G") == 0) { 
			OCR1B = value;
		}
		
		// Modificar brillo del AZUL
		if (strcmp(color, "B") == 0) { 
			OCR1A = value;
		}	
}
