#ifndef SPI_H
#define SPI_H
#include <avr/io.h>
#include <Arduino.h>    //to use byte

void SendData(byte address, byte data);

void shutDown();
void wakeUp();



#endif