#include "WATCH.h"
#include "common.h"

static uint8_t seg;
static uint8_t min;
static uint8_t hour;
void WATCH_update(){

	// Verificar si se ha alcanzado 1 minuto
	if (++seg >= 60) {
		seg = 0;
		// Verificar si se ha alcanzado 1 hora
		if (++min >= 60) {
			min = 0;
			if (++hour >= 24)
			{
				hour = 0;
			}
		}
	}
}

uint8_t WATCH_getSeg(){
	return seg;
}

uint8_t WATCH_getMin(){
	return min;
}

uint8_t WATCH_getHour(){
	return hour;
}