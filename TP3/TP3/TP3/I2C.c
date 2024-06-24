#include "I2C.h"
#include <avr/io.h> 

void i2c_init(){
	
// 		Configura los pines SCL y SDA como salidas
		DDRC |= (1<< PINC4); // Configura el pin SDA como salida  
		DDRC |= (1<< PINC5); // Configura el pin SCL como salida

		// Configurar los pines SDA y SCL para funcionar como I2C
		//DDRC &= ~((1 << PINC4) | (1 << PINC5)); // Configurar como entrada (sin afectar otros pines)
		PORTC |= (1 << PINC4) | (1 << PINC5);   // Activar resistencias pull-up en SDA y SCL

		
		TWSR=0X00;	// set prescaler bits to zero 
		
		// Configura la frecuencia de la interfaz I2C
		TWBR = 72;	// SCL frecuency is 100k for XTAL = 16M ( modo estandar )	
		
		// Habilita la interfaz I2C
		TWCR=0X04;	// enable the TWI module
}

void i2c_start(){ // enviar start y esperar confirmacion 
	TWCR = (1 << TWINT) | (1 << TWSTA ) | (1 << TWEN );
	while ((TWCR & ( 1 << TWINT )) == 0 );
}

void i2c_write(unsigned char data){
	TWDR = data ;
	TWCR = (1 << TWINT) | (1 << TWEN );
	while ((TWCR & ( 1 << TWINT )) == 0 ); // esperar hasta que el bit se transmita	
}

unsigned char i2c_read(unsigned char isLast){
	if(isLast == 0 ) // send ACK
		TWCR = (1 << TWINT) | (1 << TWEN ) | (1 << TWEA); // send ACK
	else
		TWCR = (1 << TWINT) | (1 << TWEN ); // sen NACK
	while ((TWCR & ( 1 << TWINT )) == 0 );
	return TWDR;
}

void i2c_stop(){
	TWCR = (1 << TWINT) | (1 << TWEN ) | (1 << TWSTO );
}