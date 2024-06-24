#include <string.h>

#include "serialPort.h"
#include "UART.h"
#define BUFFER_LENGTH 50 // 100 en decimal 
 
volatile char RX_String_Buffer[BUFFER_LENGTH];
volatile char TX_String_Buffer[BUFFER_LENGTH];
volatile char RX_Buffer = 0;
static uint8_t flagD = 0;
static uint8_t flagTX = 0;
static uint8_t flagMEF = 0;
volatile uint8_t readIndex = 0;
static uint8_t lengthCadena;
 
// String recibido de la linea de comando
char* UART_Get_String_From_Buffer(){
	return RX_String_Buffer;
} 
 
// Manda mensaje a imprimir a la linea de comando
void UART_Send_String_To_Transmit(char* string_pointer){
	while(flagTX);
	flagTX = 1;
	lengthCadena = strlen(string_pointer);
	strcpy(TX_String_Buffer, string_pointer);
	SerialPort_TX_Interrupt_Enable();
} 
 
// Flag para chequear si se recibio un input por la terminal
uint8_t UART_Get_Flag(){
	return flagMEF;
}

void UART_Set_flag(uint8_t value){
	flagMEF = value;
}

void UART_Update(){	
	if(flagD){ // Recepción NO Bloqueante
		strcpy(TX_String_Buffer, RX_String_Buffer);
		flagTX = 1;
		SerialPort_TX_Interrupt_Enable();
		while (flagTX);
		flagMEF = 1;
		flagD = 0;
	}
}

// Recepcion
ISR(USART_RX_vect){
	RX_Buffer = UDR0; // La lectura del UDR borra flag RXC
	RX_String_Buffer[0] = RX_Buffer;
	flagD = 1;
}

// Transmision
ISR(USART_UDRE_vect){
	UDR0 = TX_String_Buffer[readIndex];
	readIndex++;
	if(lengthCadena == 0){
		readIndex = 0;
		lengthCadena = 0;
		flagTX = 0;
		UCSR0B &=~(1<<UDRIE0); // Deshabilito interrupcion
	} else {
		lengthCadena--;
	}	
}