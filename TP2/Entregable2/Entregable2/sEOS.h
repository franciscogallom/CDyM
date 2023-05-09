#ifndef 
#define sEOS

#include <avr/interrupt.h>
#include <avr/io.h>


void TIMER0_Init(void);


ISR(TIMER0_COMPA_vect);

void sEOS_Dispatch_Tasks(void);

#endif