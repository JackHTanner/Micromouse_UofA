#ifndef PWM_H
#define PWM_H

#include <avr/io.h>
void initPWMTimer3();

void changeDutyCycle1(uint16_t adcValue);
void changeDutyCycle2(uint16_t adcValue);

#endif