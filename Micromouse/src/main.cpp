#include <Arduino.h>
#include "timer.h"
#include "SPI.h"

#include <avr/io.h>
#include <avr/interrupt.h>

int main () {
  DDRA |= (1<<DDA5) | (1<<DDA4) | (1<<DDA6) | (1<<DDA7);
  Serial.begin(9600);
  Serial.flush();
  //Serial.println("Starting...");
  initTimer1();
  initTimer2();

  while (1) {

	}
  return 0;
}

