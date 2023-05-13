#ifndef sEOS
#define sEOS

#include "common.h"


void TIMER0_Init(void);

void MEF_update();

//ISR(TIMER0_COMPA_vect);

void sEOS_Dispatch_Tasks(void);

void update_WATCH();

#endif