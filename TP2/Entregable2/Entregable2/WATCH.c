#include "WATCH.h"

static seg, min, hour;

void update_WATCH(){

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