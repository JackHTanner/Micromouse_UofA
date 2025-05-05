#ifndef SPI_H
#define SPI_H
#include <avr/io.h>
#include <Arduino.h>    //to use byte

void sendData(byte address, byte data);
void sendDataToAll(byte address, byte data4, byte data3, byte data2, byte data1);
void updateDisplay();
void initMAX7219();


void shutDown();
void wakeUp();

void displayAnimation();
void setArraysToZero();

void displaySolution(byte pathFrames[30][5][5]);

extern byte matrix1[8];
extern byte matrix2[8];
extern byte matrix3[8];
extern byte matrix4[8];




#endif
