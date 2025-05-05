#include "SPI.h"
#include <avr/io.h>
#include "timer.h"
#include <timer.h>
#include <Arduino.h>

//PROVIDED IN MAX7219 DATA SHEET
#define REG_NOOP        0x00
#define REG_DIGIT0      0x01
#define REG_DIGIT1      0x02
#define REG_DIGIT2      0x03
#define REG_DIGIT3      0x04
#define REG_DIGIT4      0x05
#define REG_DIGIT5      0x06
#define REG_DIGIT6      0x07
#define REG_DIGIT7      0x08
#define REG_DECODE      0x09
#define REG_INTENSITY   0x0A
#define REG_SCANLIMIT   0x0B
#define REG_SHUTDOWN    0x0C
#define REG_DISPLAYTEST 0x0F

//Definitions for custom displays:
//All segments ON
const byte allOn[8] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//All segments OFF
const byte allOff[8] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


//Variables for each matrix (initialized to all segments OFF)
//8 bytes for each row in matrix 1
byte matrix1[8] = {0};

//8 bytes for each row in matrix 2
byte matrix2[8] = {0};

//8 bytes for each row in matrix 3
byte matrix3[8] = {0};

//8 bytes for each row in matrix 4
byte matrix4[8] = {0};

// Pin definitions
#define CS_PIN   0  // PB0
#define CLK_PIN  1  // PB1
#define DIN_PIN  2  // PB2


// Function to send data to MAX7219
void sendData(byte address, byte data) {
  // Pull CS low to start transmission
  PORTB &= ~(1 << CS_PIN);
  
  // Send address byte
  for (byte i = 0; i < 8; i++) {
    // Set CLK low
    PORTB &= ~(1 << CLK_PIN);
    
    // Set DIN high or low based on bit value
    if (address & (1 << (7 - i))) {
      PORTB |= (1 << DIN_PIN);
    } else {
      PORTB &= ~(1 << DIN_PIN);
    }
    
    // Pulse CLK high
    PORTB |= (1 << CLK_PIN);
  }
  
  // Send data byte
  for (byte i = 0; i < 8; i++) {
    // Set CLK low
    PORTB &= ~(1 << CLK_PIN);
    
    // Set DIN high or low based on bit value
    if (data & (1 << (7 - i))) {
      PORTB |= (1 << DIN_PIN);
    } else {
      PORTB &= ~(1 << DIN_PIN);
    }
    
    // Pulse CLK high
    PORTB |= (1 << CLK_PIN);
  }
  
  // Pull CS high to end transmission
  PORTB |= (1 << CS_PIN);
}

//send data to ALL 4 matrixes
void sendDataToAll(byte address, byte data4, byte data3, byte data2, byte data1) {
  PORTB &= ~(1 << CS_PIN);  // CS LOW to start transmission

  byte dataBytes[4] = {data4, data3, data2, data1};
  byte addrBytes[4] = {address, address, address, address};

  // Send to all 4 modules (MSB first: module 4 first)
  for (byte m = 0; m < 4; m++) {
    // Send address
    for (byte i = 0; i < 8; i++) {
      PORTB &= ~(1 << CLK_PIN);
      if (addrBytes[m] & (1 << (7 - i))) PORTB |= (1 << DIN_PIN);
      else PORTB &= ~(1 << DIN_PIN);
      PORTB |= (1 << CLK_PIN);
    }

    // Send data
    for (byte i = 0; i < 8; i++) {
      PORTB &= ~(1 << CLK_PIN);
      if (dataBytes[m] & (1 << (7 - i))) PORTB |= (1 << DIN_PIN);
      else PORTB &= ~(1 << DIN_PIN);
      PORTB |= (1 << CLK_PIN);
    }
  }

  PORTB |= (1 << CS_PIN);  // CS HIGH to latch
}

// Function to update the display
void updateDisplay() {
  // Update the display with the current data
  for (byte i = 0; i < 8; i++) {
    sendDataToAll(i + 1, matrix4[i], matrix3[i], matrix2[i], matrix1[i]);
  }
}


void initMAX7219() {
  // Set pin directions
  DDRB |= (1 << CS_PIN) | (1 << CLK_PIN) | (1 << DIN_PIN);
  
  // Set initial pin states
  PORTB |= (1 << CS_PIN);  // CS high
  PORTB &= ~(1 << CLK_PIN); // CLK low
  
  // Wake up from shutdown mode
  sendDataToAll(REG_SHUTDOWN, 0x01, 0x01, 0x01, 0x01);
  
  // Disable test mode
  sendDataToAll(REG_DISPLAYTEST, 0x00, 0x00, 0x00, 0x00);
  
  // Set scan limit to display all digits (rows 0-7)
  sendDataToAll(REG_SCANLIMIT, 0x07, 0x07, 0x07, 0x07);
  
  // Disable BCD decoding
  sendDataToAll(REG_DECODE, 0x00, 0x00, 0x00, 0x00);
  
  // Set brightness
  sendDataToAll(REG_INTENSITY, 0x08, 0x08, 0x08, 0x08);
  
  // Clear the display
  for (byte i = 1; i <= 8; i++) {
    sendDataToAll(i, 0x00, 0x00, 0x00, 0x00);
  }
}

void setArraysToZero() {
  for (int i = 0; i < 8; i++) {
    matrix1[i] = 0;
    matrix2[i] = 0;
    matrix3[i] = 0;
    matrix4[i] = 0;
  }
}

void displayAnimation() {
  setArraysToZero();
  updateDisplay();
  matrix1[7] = 0x01;
  matrix2[0] = 0x01;
  matrix3[0] = 0x01;
  matrix4[7] = 0x01;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[6] = 0x01;
  matrix1[7] = 0x02;
  matrix2[0] = 0x02;
  matrix2[1] = 0x01;
  matrix3[0] = 0x02;
  matrix3[1] = 0x01;
  matrix4[6] = 0x01;
  matrix4[7] = 0x02;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[5] = 0x01;
  matrix1[6] = 0x02;
  matrix1[7] = 0x04;
  matrix2[0] = 0x04;
  matrix2[1] = 0x02;
  matrix2[2] = 0x01;
  matrix3[0] = 0x04;
  matrix3[1] = 0x02;
  matrix3[2] = 0x01;
  matrix4[5] = 0x01;
  matrix4[6] = 0x02;
  matrix4[7] = 0x04;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[4] = 0x01;
  matrix1[5] = 0x02;
  matrix1[6] = 0x04;
  matrix1[7] = 0x08;
  matrix2[0] = 0x08;
  matrix2[1] = 0x04;
  matrix2[2] = 0x02;
  matrix2[3] = 0x01;
  matrix3[0] = 0x08;
  matrix3[1] = 0x04;
  matrix3[2] = 0x02;
  matrix3[3] = 0x01;
  matrix4[4] = 0x01;
  matrix4[5] = 0x02;
  matrix4[6] = 0x04;
  matrix4[7] = 0x08;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  matrix1[3] = 0x01;
  matrix1[4] = 0x02;
  matrix1[5] = 0x04;
  matrix1[6] = 0x08;
  matrix1[7] = 0x10;
  matrix2[0] = 0x10;
  matrix2[1] = 0x08;
  matrix2[2] = 0x04;
  matrix2[3] = 0x02;
  matrix2[4] = 0x01;
  matrix3[0] = 0x10;
  matrix3[1] = 0x08;
  matrix3[2] = 0x04;
  matrix3[3] = 0x02;
  matrix3[4] = 0x01;
  matrix4[3] = 0x01;
  matrix4[4] = 0x02;
  matrix4[5] = 0x04;
  matrix4[6] = 0x08;
  matrix4[7] = 0x10;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[2] = 0x01;
  matrix1[3] = 0x02;
  matrix1[4] = 0x04;
  matrix1[5] = 0x08;
  matrix1[6] = 0x10;
  matrix1[7] = 0x20;
  matrix2[0] = 0x20;
  matrix2[1] = 0x10;
  matrix2[2] = 0x08;
  matrix2[3] = 0x04;
  matrix2[4] = 0x02;
  matrix2[5] = 0x01;
  matrix3[0] = 0x20;
  matrix3[1] = 0x10;
  matrix3[2] = 0x08;
  matrix3[3] = 0x04;
  matrix3[4] = 0x02;
  matrix3[5] = 0x01;
  matrix4[2] = 0x01;
  matrix4[3] = 0x02;
  matrix4[4] = 0x04;
  matrix4[5] = 0x08;
  matrix4[6] = 0x10;
  matrix4[7] = 0x20;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[1] = 0x01;
  matrix1[2] = 0x02;
  matrix1[3] = 0x04;
  matrix1[4] = 0x08;
  matrix1[5] = 0x10;
  matrix1[6] = 0x20;
  matrix1[7] = 0x40;
  matrix2[0] = 0x40;
  matrix2[1] = 0x20;
  matrix2[2] = 0x10;
  matrix2[3] = 0x08;
  matrix2[4] = 0x04;
  matrix2[5] = 0x02;
  matrix2[6] = 0x01;
  matrix3[0] = 0x40;
  matrix3[1] = 0x20;
  matrix3[2] = 0x10;
  matrix3[3] = 0x08;
  matrix3[4] = 0x04;
  matrix3[5] = 0x02;
  matrix3[6] = 0x01;
  matrix4[1] = 0x01;
  matrix4[2] = 0x02;
  matrix4[3] = 0x04;
  matrix4[4] = 0x08;
  matrix4[5] = 0x10;
  matrix4[6] = 0x20;
  matrix4[7] = 0x40;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[0] = 0x01;
  matrix1[1] = 0x02;
  matrix1[2] = 0x04;
  matrix1[3] = 0x08;
  matrix1[4] = 0x10;
  matrix1[5] = 0x20;
  matrix1[6] = 0x40;
  matrix1[7] = 0x80;
  matrix2[0] = 0x80;
  matrix2[1] = 0x40;
  matrix2[2] = 0x20;
  matrix2[3] = 0x10;
  matrix2[4] = 0x08;
  matrix2[5] = 0x04;
  matrix2[6] = 0x02;
  matrix2[7] = 0x01;
  matrix3[0] = 0x80;
  matrix3[1] = 0x40;
  matrix3[2] = 0x20;
  matrix3[3] = 0x10;
  matrix3[4] = 0x08;
  matrix3[5] = 0x04;
  matrix3[6] = 0x02;
  matrix3[7] = 0x01;
  matrix4[0] = 0x01;
  matrix4[1] = 0x02;
  matrix4[2] = 0x04;
  matrix4[3] = 0x08;
  matrix4[4] = 0x10;
  matrix4[5] = 0x20;
  matrix4[6] = 0x40;
  matrix4[7] = 0x80;
  updateDisplay();
  delayMs(99999);
  
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[1] = 0x02;
  matrix1[2] = 0x04;
  matrix1[3] = 0x08;
  matrix1[4] = 0x10;
  matrix1[5] = 0x20;
  matrix1[6] = 0x40;
  matrix1[7] = 0x80;
  matrix2[0] = 0x80;
  matrix2[1] = 0x40;
  matrix2[2] = 0x20;
  matrix2[3] = 0x10;
  matrix2[4] = 0x08;
  matrix2[5] = 0x04;
  matrix2[6] = 0x02;
  matrix3[0] = 0x80;
  matrix3[1] = 0x40;
  matrix3[2] = 0x20;
  matrix3[3] = 0x10;
  matrix3[4] = 0x08;
  matrix3[5] = 0x04;
  matrix3[6] = 0x02;
  matrix4[1] = 0x02;
  matrix4[2] = 0x04;
  matrix4[3] = 0x08;
  matrix4[4] = 0x10;
  matrix4[5] = 0x20;
  matrix4[6] = 0x40;
  matrix4[7] = 0x80;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[2] = 0x04;
  matrix1[3] = 0x08;
  matrix1[4] = 0x10;
  matrix1[5] = 0x20;
  matrix1[6] = 0x40;
  matrix1[7] = 0x80;
  matrix2[0] = 0x80;
  matrix2[1] = 0x40;
  matrix2[2] = 0x20;
  matrix2[3] = 0x10;
  matrix2[4] = 0x08;
  matrix2[5] = 0x04;
  matrix3[0] = 0x80;
  matrix3[1] = 0x40;
  matrix3[2] = 0x20;
  matrix3[3] = 0x10;
  matrix3[4] = 0x08;
  matrix3[5] = 0x04;
  matrix4[2] = 0x04;
  matrix4[3] = 0x08;
  matrix4[4] = 0x10;
  matrix4[5] = 0x20;
  matrix4[6] = 0x40;
  matrix4[7] = 0x80;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[3] = 0x08;
  matrix1[4] = 0x10;
  matrix1[5] = 0x20;
  matrix1[6] = 0x40;
  matrix1[7] = 0x80;
  matrix2[0] = 0x80;
  matrix2[1] = 0x40;
  matrix2[2] = 0x20;
  matrix2[3] = 0x10;
  matrix2[4] = 0x08;
  matrix3[0] = 0x80;
  matrix3[1] = 0x40;
  matrix3[2] = 0x20;
  matrix3[3] = 0x10;
  matrix3[4] = 0x08;
  matrix4[3] = 0x08;
  matrix4[4] = 0x10;
  matrix4[5] = 0x20;
  matrix4[6] = 0x40;
  matrix4[7] = 0x80;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[4] = 0x10;
  matrix1[5] = 0x20;
  matrix1[6] = 0x40;
  matrix1[7] = 0x80;
  matrix2[0] = 0x80;
  matrix2[1] = 0x40;
  matrix2[2] = 0x20;
  matrix2[3] = 0x10;
  matrix3[0] = 0x80;
  matrix3[1] = 0x40;
  matrix3[2] = 0x20;
  matrix3[3] = 0x10;
  matrix4[4] = 0x10;
  matrix4[5] = 0x20;
  matrix4[6] = 0x40;
  matrix4[7] = 0x80;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[5] = 0x20;
  matrix1[6] = 0x40;
  matrix1[7] = 0x80;
  matrix2[0] = 0x80;
  matrix2[1] = 0x40;
  matrix2[2] = 0x20;
  matrix3[0] = 0x80;
  matrix3[1] = 0x40;
  matrix3[2] = 0x20;
  matrix4[5] = 0x20;
  matrix4[6] = 0x40;
  matrix4[7] = 0x80;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[6] = 0x40;
  matrix1[7] = 0x80;
  matrix2[0] = 0x80;
  matrix2[1] = 0x40;
  matrix3[0] = 0x80;
  matrix3[1] = 0x40;
  matrix4[6] = 0x40;
  matrix4[7] = 0x80;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
  
  matrix1[7] = 0x80;
  matrix2[0] = 0x80;
  matrix3[0] = 0x80;
  matrix4[7] = 0x80;
  updateDisplay();
  delayMs(99999);
  
  setArraysToZero();
  updateDisplay();
}

void displaySolution(byte pathFrames[30][5][5]) {
  for (int i = 0; i < 30; i++) {
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 5; col++) {
        if (pathFrames[i][row][col] == 1) {
          matrix1[row] |= (1 << (7 - col));  // draw on matrix1 only
        }
      }
    }

    updateDisplay();
    delayMs(99999);
  }
}

void shutDown() {
  // Put the MAX7219 in shutdown mode
 // sendData(REG_SHUTDOWN, 0x00);
}

void wakeUp() {
  // Wake up the MAX7219 from shutdown mode
  sendData(REG_SHUTDOWN, 0x01);
}
