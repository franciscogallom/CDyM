#ifndef TIMERS
	#define TIMERS
	#include <avr/io.h>

void TIMER1_INIT(void);

void PWM_soft_update(void);

void set_ocr1a(uint8_t);

void set_ocr1b(uint8_t);

void set_pwm_soft_value(uint8_t);

uint8_t get_pwm_soft_value(void);

#endif