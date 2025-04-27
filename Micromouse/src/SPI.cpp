#include "SPI.h"
#include <avr/io.h>
#include "timer.h"
#include <timer.h>
#include <Arduino.h>

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

void shutDown() {
  // Put the MAX7219 in shutdown mode
  sendData(REG_SHUTDOWN, 0x00);
}

void wakeUp() {
  // Wake up the MAX7219 from shutdown mode
  sendData(REG_SHUTDOWN, 0x01);
}