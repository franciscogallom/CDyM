#include "MEF.h"
#include "RTC.h"
#include "DHT11.h"
#include "UART.h"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

states estadoActual;
uint8_t temperatura;
uint8_t humedad;
uint8_t readStatus;
char stringTemp[2];
char stringHum[2];
char input[1];
int data[7];			// Para guardar fecha y hora
uint8_t time_str[15];	// Para guardar la hora
uint8_t date_str[15];	// Para guardar la fecha
	
void MEF_init(){
	estadoActual = PLAY;
	set_rtc_time();
}

void handleUpdate(){
	if (UART_Get_Flag() == 1) {
		UART_Set_flag(0);
		strcpy(input, UART_Get_String_From_Buffer());
		if (strcmp(input, "s") == 0 || strcmp(input, "S") == 0) {
			if (estadoActual == PLAY) {
				estadoActual = PAUSE;				
			} else if (estadoActual == PAUSE) {
				estadoActual = PLAY;
			}
		}
	}
}

void MEF_update(){
	handleUpdate();

	switch (estadoActual) {
		case PLAY:
			readStatus = DHT11_read(&temperatura, &humedad);
			if (readStatus) {		
				UART_Send_String_To_Transmit("\r\nTEMP: ");
				itoa(temperatura, stringTemp, 10);
				UART_Send_String_To_Transmit(stringTemp);
				UART_Send_String_To_Transmit("°C ");
				UART_Send_String_To_Transmit("HUM: ");
				itoa(humedad, stringHum, 10);
				UART_Send_String_To_Transmit(stringHum);
				UART_Send_String_To_Transmit("% ");
				read_rtc(&date_str,&time_str);
				UART_Send_String_To_Transmit(date_str);
				UART_Send_String_To_Transmit(" ");
				UART_Send_String_To_Transmit(time_str);
				UART_Send_String_To_Transmit("\r\n");
			} else {
				UART_Send_String_To_Transmit("ERROR al leer los datos del periferico DHT11");
			}
			break;
			
		case PAUSE:
			// Nada
			break;
		
		default:
			// Nada
			break;		
	}
}