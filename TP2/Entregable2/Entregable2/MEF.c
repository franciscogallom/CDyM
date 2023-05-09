#include "MEF.h"
#include "lcd.h"
#include "string.h"

static state estado;
static call_count = 0;

char password[4];

void resetCallCountAndGoToState(uint8_t callCount, state st){
		if (call_count == callCount)
		{
			call_count = 0;
			estado = st;
		}
}

uint8_t isCorrectPassword(char pass[4]){
	char *inputPassword = "5913";

	if (strcmp(pass, inputPassword) == 0) {
		return 1;
	} else {
		return 0;
	}
}

void handleInicio(char pass[4]){
	switch(pass[0]){
		case "A":
			estado = EdicionH;
			call_count = 0;
			break;
		case "B":
			estado = EdicionM;
			call_count = 0;
			break;
		case "C":
			estado = EdicionS;
			call_count = 0;
			break;
		case "0":
		case "1":
		case "2":
		case "3":
		case "4":
		case "5":
		case "6":
		case "7":
		case "8":
		case "9":
			estado = D0;
			call_count = 0;
			break;
		default:
			break;
	}
}

void MEF_readkey(int position){
	
}

void MEF_init(){
	estado = Inicio;
}

void MEF_update(){ //Update cada 100 ms
	
	call_count++;
	switch(estado)
	{
		case Inicio:
			// Muestro la hora y el mensaje "CERRADO".
			LCDGotoXY(5, 0);
			LCDescribeDato(15, 2);
			LCDsendChar(':');
			LCDescribeDato(30, 2);
			LCDsendChar(':');
			LCDescribeDato(10, 2);
			LCDGotoXY(5, 1);
			LCDstring("CERRADO", 7);
			
			// Detectar que se presiono una tecla.
			MEF_readkey(0);
			handleInicio(password[0]);
			break;
		
		case D0:
			MEF_readkey(1);
			resetCallCountAndGoToState(30, Inicio);
			break;
		
		case D1:
			MEF_readkey(2);
			resetCallCountAndGoToState(30, Inicio);
			break;
		
		case D2:
			MEF_readkey(3);
			resetCallCountAndGoToState(30, Inicio);
			break;
		
		case D3:
			// Al llegar a este estado, ya tengo los 4 caracteres ingresados.
			if (isCorrectPassword(password)) {
				estado = Correcto;
			} else {
				estado = Incorrecto;
			}
			break;
		
		case Incorrecto:
			// Se muestra el mensaje "DENEGADO" por dos segundos y vuelve al estado por defecto.
			LCDstring("DENEGADO", 8);			
			// Pasaron 2 segundos.
			resetCallCountAndGoToState(20, Inicio);
			break;
		
		case Correcto:
			// Se muestra el mensaje "ABIERTO" por tres segundos y vuelve al estado por defecto.
			LCDstring("ABIERTO", 7);
			// Pasaron 3 segundos.
			resetCallCountAndGoToState(30, Inicio);
			break;
		
		case EdicionH:
		
		break;
		
		case EdicionM:
		
		break;
		
		case EdicionS:
		
		break;
		
	}
	
}