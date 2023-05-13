#ifndef WATCH
#define WATCH

#include "common.h"

void WATCH_update();

uint8_t WATCH_getSeg();
uint8_t WATCH_getMin();
uint8_t WATCH_getHour();
void WATCH_setHour();
void WATCH_setMinutes();
void WATCH_setSeconds();


#endif