#include "RTC.h"
#include "I2C.h"
#include <avr/io.h> 
#include <stdio.h>
#include <stdlib.h>

void set_rtc_time(void) {
    // Obtener fecha y hora actuales de la PC
    char date_str[] = __DATE__; // "MMM DD YYYY"
    char time_str[] = __TIME__; // "HH:MM:SS"

    int year = atoi(date_str + 9);		// Año
    int month = monthToInt(date_str);	// Mes
    int day = atoi(date_str + 4);		// Día
    int hour = atoi(time_str);			// Hora
    int minute = atoi(time_str + 3);	// Minuto
    int second = atoi(time_str + 6);	// Segundo

    // Convertir fecha y hora a formato BCD
    year = decToBcd(year);
    month = decToBcd(month);
    day = decToBcd(day);
    hour = decToBcd(hour);
    minute = decToBcd(minute);
    second = decToBcd(second);

    // Enviar fecha y hora al módulo RTC DS3232
    i2c_init();
    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x00); // Registro de segundos
    i2c_write(second);
    i2c_write(minute);
    i2c_write(hour);
	i2c_write(0);
    i2c_write(day);
    i2c_write(month);
    i2c_write(year);
    i2c_stop();
}

int decToBcd(int decimal) {
	int bcd = 0;
	bcd = (decimal / 10) << 4;
	bcd |= decimal % 10;
	return bcd;
}

int bcdToDec(int bcd) {
	int dec = 0;
	dec += (bcd >> 4) * 10;
	dec += bcd & 0x0F;
	return dec;
}

int monthToInt(char *monthStr) {
	char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	for (int i = 0; i < 12; i++) {
		if (strncmp(monthStr, months[i], 3) == 0) {
			return i + 1; // Devuelve el número del mes (1-12)
		}
	}
	return 0; // Error, mes no encontrado
}

void read_rtc(uint8_t *date , uint8_t *time) {
	// Inicializar la interfaz I2C
	i2c_init();
	uint8_t data [7];
	// Leer la fecha y hora del módulo RTC
	i2c_start();
	i2c_write(0xD0);					// Dirección del módulo RTC, modo escritura
	i2c_write(0x00);					// Registro de la fecha y hora
	i2c_start();
	i2c_write(0xD1);					// Dirección del módulo RTC + 1, modo lectura 
	for (int i = 0; i < 6; i++) {
		data[i] = i2c_read(0);			// El cero indica que se esperan recibir mas bytes
	}
	data[6] = i2c_read(1);				// El uno indica que no se esperan recibir mas bytes, tambien sirve para leer un solo byte
	i2c_stop();
	for (int i = 0; i < 7; i++ ){
		data[i] = bcdToDec(data[i]);
	}
	// Formatear la fecha y hora
 	sprintf(date, "FECHA: %02u/%02u/%02u", data[4], data[5], data[6]);
 	sprintf(time, "HORA: %02u:%02u:%02u", data[2], data[1], data[0]);
}
