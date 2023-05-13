#ifndef MEF

#define MEF
#include "keypad.h"
#include "common.h"

typedef enum {Inicio, D0, D1, D2, D3, Correcto, Incorrecto, EdicionH, EdicionM, EdicionS} state;


void MEF_init(void);

uint8_t KEYPAD_update(uint8_t *);



void MEF_update(void);	


#endif