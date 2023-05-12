#include "lcd.h"
#include "string.h"

static state estado;
static call_count = 0;
static uint8_t key;
static uint8_t pos;
char password[4];
 
 
//Funcion privada
static void resetCallCountAndGoToState(uint8_t callCount, state st){
		if (call_count == callCount)
		{
			call_count = 0;
			estado = st;
		}
}

//Funcion privada 
static uint8_t isCorrectPassword(char pass[4]){
	char *inputPassword = "5913";
	if (strcmp(pass, inputPassword) == 0) {
		return 1;
	} else {
		return 0;
	}
}

//Funcion privada
static uint8_t MEF_readKey(uint8_t pos)){
	if(KEYPAD_Scan(&key)){ //Cada 100 ms
		return 0;
		}else{
			password[pos] = key;
		return 1;
	}
	
}

static void handleInicio(char pass[4]){
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
			LCDescribeDato(WATCH_getSeg(), 2);
			LCDsendChar(':');
			LCDescribeDato(WATCH_getMin(), 2);
			LCDsendChar(':');
			LCDescribeDato(WATCH_getHour(), 2);
			LCDGotoXY(5, 1);
			LCDstring("CERRADO", 7);
			
			// Detectar que se presiono una tecla.
			MEF_readKey(0);
			handleInicio(password[0]);
			break;
		
		case D0:
			MEF_readKey(1);
			resetCallCountAndGoToState(30, Inicio); //Si pasaron 3 sg y no se leyo ninguna tecla, vuelve al estado Inicio.
			break;
		
		case D1:
			MEF_readKey(2);
			resetCallCountAndGoToState(30, Inicio);
			break;
		
		case D2:
			MEF_readKey(3);
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
			LCDGotoXY(5,1);
			LCDstring("DENEGADO", 8);			
			// Pasaron 2 segundos.
			resetCallCountAndGoToState(20, Inicio);
			break;
		
		case Correcto:
			// Se muestra el mensaje "ABIERTO" por tres segundos y vuelve al estado por defecto.
			LCDGotoXY(5,1);
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