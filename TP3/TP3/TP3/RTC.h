#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

void read_rtc(uint8_t * , uint8_t *);
void set_rtc_time(void);
int decToBcd(int decimal);
int bcdToDec(int bcd);
int monthToInt(char *monthStr);