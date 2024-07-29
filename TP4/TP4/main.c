#define F_CPU 16000000UL
#define BR9600 (0x67)		// 0x67 = 103 --> Configura BAUDRATE = 9600@16MHz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ADC.h"
#include "LED.h"
#include "serialPort.h"
#include "timer.h"

#define PWM_START DDRB |= (1<<PORTB5)
#define PIN_RED PORTB5
#define PIN_GREEN PORTB2
#define PIN_BLUE PORTB1

int main(void) {
	static uint16_t ADC_result;
	static uint8_t value;
	char color[1];
	
	TIMER1_INIT();
	ADC_Init();
	ADC_Enable();
	PWM_START;

	sei();	
	SerialPort_Init(BR9600); 			// Inicializo formato 8N1 y BAUDRATE = 9600bps
	SerialPort_TX_Enable();				// Activo el Transmisor del Puerto Serie
	SerialPort_RX_Enable();				// Activo el Receptor del Puerto Serie
	SerialPort_RX_Interrupt_Enable();	// Activo Interrupción de recepcion.
	
	ADC_Start_Conversion();
	
    while (1) { 
		while((ADCSRA&(1<<ADIF)) == 0); // Esperar que termine la conversion
		ADCSRA |= (1<<ADIF);
		ADC_result = ADC_GetResult(); 
		value = ADC_result >> 2; // De 10 a 8 bits
		PWM_soft_update();
		UART_Update();
			
		if (UART_Get_Flag() == 1) {
			UART_Set_flag(0);
			strcpy(color, UART_Get_String_From_Buffer());
			if (strcmp(color, "R") == 0 || strcmp(color, "G") == 0 || strcmp(color, "B") == 0) {
				LED_Update(value, color);
			}
		}
		
		ADC_Start_Conversion();	
    }
}