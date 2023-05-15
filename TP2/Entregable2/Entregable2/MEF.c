#include "common.h"
#include "lcd.h"
#include "string.h"
#include "MEF.h"
#include "WATCH.h"

static state estado;
static uint8_t call_count;
static uint8_t call_count_keypad;
static uint8_t key;
static uint8_t pos;
static uint8_t password[4]; //Utilizar uint8
static uint8_t data[2];
static uint8_t dataCount = 0;
static uint8_t num;
 
//Funcion privada
static void resetCallCountAndGoToState(uint8_t callCount, state st){
		if (call_count == callCount)
		{
			LCDclr();
			call_count = 0;
			estado = st;
		}
}

//Funcion privada 
static uint8_t isCorrectPassword(uint8_t * pass){
	
	if((pass[0] == '5') && (pass[1] == '9') && (pass[2] == '1') && (pass[3] == '3')){
		return 1;
	} else {
		return 0;
	}
}

//Funcion privada 
static uint8_t MEF_readKey(uint8_t pos){
	if(!(KEYPAD_Scan(&key))){ //Cada 100 ms
		return 0;
		}else{
			password[pos] = key;
		return 1;
	}
	
}

static void handleInicio(uint8_t * pass){
	switch(pass[0]){
		case 'A':
			estado = EdicionH;
			LCDGotoXY(4,0);
			call_count = 0;
			break;
		case 'B':
			estado = EdicionM;
			LCDGotoXY(7,0);
			call_count = 0;
			break;
		case 'C':
			estado = EdicionS;
			LCDGotoXY(10,0);
			call_count = 0;
			break;
		case 'D':
			estado = Inicio;
			call_count = 0;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			estado = D0;
			call_count = 0;
			break;
		default:
			
			break;
	}
}

void MEF_init(){
	estado = Inicio;
	call_count = 0;
	LCDclr();
	LCDGotoXY(4, 0);
	LCDescribeDato(WATCH_getSeg(), 2);
	LCDsendChar(':');
	LCDescribeDato(WATCH_getMin(), 2);
	LCDsendChar(':');
	LCDescribeDato(WATCH_getHour(), 2);
	LCDGotoXY(5, 1);
	LCDstring("CERRADO", 7);
}

void MEF_update(){ //Update cada 100 ms
	
	call_count++;
	switch(estado)
	{
		case Inicio:
			// Muestro la hora y el mensaje "CERRADO".
			LCDGotoXY(4, 0);
			LCDescribeDato(WATCH_getHour(), 2);
			LCDsendChar(':');
			LCDescribeDato(WATCH_getMin(), 2);
			LCDsendChar(':');
			LCDescribeDato(WATCH_getSeg(), 2);
			LCDGotoXY(5, 1);
			LCDstring("CERRADO", 7);
			
			// Detectar que se presiono una tecla.
			if (++call_count_keypad == 3)
			{
				if(MEF_readKey(0)){
					handleInicio(password);
				}
				call_count_keypad = 0;
			}
			
			
			
			break;
		
		case D0:
			LCDclr();
			LCDGotoXY(5,1);
			LCDsendChar('*');
			//LCDsendChar(password[0]);
			if (++call_count_keypad == 3)
			{
				if(MEF_readKey(1)){
					call_count = 0;
					estado = D1;
				}
				call_count_keypad = 0;
			}
			
			resetCallCountAndGoToState(30, Inicio); //Si pasaron 3 sg y no se leyo ninguna tecla, vuelve al estado Inicio.
			break;
		
		case D1:
			LCDGotoXY(6,1);
			LCDsendChar('*');
			//LCDsendChar(password[1]);
				if (++call_count_keypad == 3)
				{
					if(MEF_readKey(2)){
						call_count = 0;
						estado = D2;
					}
					call_count_keypad = 0;
				}
			resetCallCountAndGoToState(30, Inicio);
			break;
		
		case D2:
			LCDGotoXY(7,1);
			LCDsendChar('*');
			//LCDsendChar(password[2]);
				if (++call_count_keypad == 3)
				{
					if(MEF_readKey(3)){
						call_count = 0;
						estado = D3;
					}
					call_count_keypad = 0;
				}
			resetCallCountAndGoToState(30, Inicio);
			break;
		
		case D3:
			// Al llegar a este estado, ya tengo los 4 caracteres ingresados.
			LCDGotoXY(8,1);
			LCDsendChar(password[3]);
			if (isCorrectPassword(password)) {
				call_count = 0;
				estado = Correcto;
			} else {
				call_count = 0;
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
			if(++call_count_keypad == 3){
				call_count_keypad = 0;;
				if(MEF_readKey(0)){
					switch(password[0]){
						case 'A':
							// Guardar
							num = ( ((data[0] - '0')* 10) + (data[1] - '0'));
							// dataCount == 2 significa que el usuario ingreso dos numeros
							if (dataCount == 2) {
								if (num >= 0 && num <= 23) {
									WATCH_setHour(num);	
								}	
							}
							dataCount = 0;
							estado = Inicio;
						break;
					case '#':
						// Cancelar.
						estado = Inicio;
						dataCount = 0;
						break;
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						if(dataCount < 2) {
							data[dataCount] = password[0];
							LCDsendChar(data[dataCount]);
							dataCount++;
						}
						break;
					default:
					
						break;
					}
				}
				call_count_keypad == 0;
			}
			break;
				
		case EdicionM:
		if(++call_count_keypad == 3){
			call_count_keypad = 0;;
			if(MEF_readKey(0)){
				switch(password[0]){
					case 'B':
					// Guardar
					num = ( ((data[0] - '0')* 10) + (data[1] - '0'));
					// dataCount == 2 significa que el usuario ingreso dos numeros
					if (dataCount == 2) {
						if (num >= 0 && num <= 59) {
							WATCH_setMinutes(num);
						}
					}
					dataCount = 0;
					estado = Inicio;
					break;
					case '#':
					// Cancelar.
					estado = Inicio;
					dataCount = 0;
					break;
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
					if(dataCount < 2) {
						data[dataCount] = password[0];
						LCDsendChar(data[dataCount]);
						dataCount++;
					}
					break;
					default:
					
					break;
				}
			}
			call_count_keypad == 0;
		}
		break;
		
		case EdicionS:
		if(++call_count_keypad == 3){
			call_count_keypad = 0;;
			if(MEF_readKey(0)){
				switch(password[0]){
					case 'C':
					// Guardar
					num = ( ((data[0] - '0')* 10) + (data[1] - '0'));
					// dataCount == 2 significa que el usuario ingreso dos numeros
					if (dataCount == 2) {
						if (num >= 0 && num <= 59) {
							WATCH_setSeconds(num);
						}
					}
					dataCount = 0;
					estado = Inicio;
					break;
					case '#':
					// Cancelar.
					estado = Inicio;
					dataCount = 0;
					break;
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
					if(dataCount < 2) {
						data[dataCount] = password[0];
						LCDsendChar(data[dataCount]);
						dataCount++;
					}
					break;
					default:
					
					break;
				}
			}
			call_count_keypad == 0;
		}
			break;
		
	}
	
}
