#ifndef TWI_H
#define TWI_H

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
#include "OPT3101A.h"

void TWI_Init(void);
void TWI_Start(void);
void TWI_Stop(void);
void TWI_Write(uint8_t b);
uint8_t TWI_ReadACK(void);
uint8_t TWI_ReadNACK(void);

#endif