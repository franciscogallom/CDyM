#define F_CPU 16000000UL
#define BR9600 (0x67)		// 0x67 = 103 --> Configura BAUDRATE = 9600@16MHz

#include <avr/io.h>	
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "serialPort.h"
#include "DHT11.h"
#include "UART.h"
#include "MEF.h"
#include "sEOS.h"
#include "I2C.h"

int main(void) {
	TIMER0_Init();
	MEF_init();	
	DHT11_init();
	i2c_init();
	SerialPort_Init(BR9600); 			// Inicializo formato 8N1 y BAUDRATE = 9600bps
	SerialPort_TX_Enable();				// Activo el Transmisor del Puerto Serie
	SerialPort_RX_Enable();				// Activo el Receptor del Puerto Serie
	SerialPort_RX_Interrupt_Enable();	// Activo Interrupción de recepcion.
	sei();								// Activo la mascara global de interrupciones (Bit I del SREG en 1)
		
    while (1) {
		sEOS_Dispatch_Tasks();
    }
}