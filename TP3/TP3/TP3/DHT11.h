#define DHT_DDR DDRC
#define DHT_PORT PORTC
#define DHT_PIN	PINC
#define PIN 0

#include <stdint.h>

void DHT11_init(void);				

uint8_t DHT11_read(uint8_t *, uint8_t *);