#ifndef SPI_H
#define SPI_H
#include <avr/io.h>
#include <Arduino.h>    //to use byte

void sendData(byte address, byte data);
void sendDataToAll(byte address, byte data2, byte data1);
void updateDisplay();
void initMAX7219();


void shutDown();
void wakeUp();

void displayForwardAnimation();
void displayLeftAnimation();
void displayRightAnimation();
void setArraysToZero();

void displaySolution(byte pathFrames[30][5][5]);

extern byte matrix1[8];
extern byte matrix2[8];




#endif
