#include <Arduino.h>
#include "timer.h"
#include "SPI.h"
#include "Ultra.h"
#include "Algorithm.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//currDirection: 0 = North, 1 = East, 2 = South, 3 = West 
void updateDirection(int LeftorRight) { //Left = 0, Right = 1
      if (currDirection == 1 && LeftOrRight == 0) {	//if facing EAST and turn LEFT, new direction is NORTH 
        currDirection = 0;
      }
      else if (currDirection == 1 && LeftOrRight == 1) {
        currDirection = 2;
      }
      else if (currDirection == 2 && LeftorRight == 0) {
        currDirection = 1;
      }
      else if (currDirection == 2 && LeftorRight == 1) {
        currDirection = 3;
      }
      else if (currDirection == 3 && LeftorRight == 0) {
        currDirection = 2;
      }
      else if (currDirection == 3 && LeftorRight == 1) {
        currDirection = 0;
      }
}

void updatePosition() {
if (currDirection == 0) { //North
        currentX--;
      }
      else if (currDirection == 1) { //East
        currentY++;
      }
      else if (currDirection == 2) { //South
        currentX++;
      }
      else if (currDirection == 3) { //West
        currentY--;
      }
}

int main () {
  DDRA |= (1<<DDA5) | (1<<DDA4) | (1<<DDA6) | (1<<DDA7);
  Serial.begin(9600);
  Serial.flush();
  //Serial.println("Starting...");
  initTimer1();
  initTimer2();
  setUpPins();
  byte walls;
  byte mazePath[5][5] = {
    {0,0,0,0,0},
    {1,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0}
  };

  int currentX = 1;
  int currentY = 0;

  int targetX = 0;
  int targetY = 0;

  int currDirection = 1;

  while (1) {
    walls = loopUltra();
    switch (walls) {
      case 0b000: // No walls detected. Go forward.
          goForwardAndThenStop();
          break;
      case 0b001: // Right wall detected. Orient left and go forward.
          orientLeft();
          delayMs(100);
          goForwardAndThenStop();
          break;
      case 0b010: // Front wall detected. Orient left and go forward.
          orientLeft();
          delayMs(100);
          goForwardAndThenStop();
          break;
      case 0b011: // Front and right walls detected. Orient left and go forward.
          orientLeft();
          delayMs(100);
          goForwardAndThenStop();
          break;
      case 0b100: // Left wall detected. Go forward.
          goForwardAndThenStop();
          delayMs(100);
          break;
      case 0b101: // Left and right walls detected. Go forward.
          goForwardAndThenStop();
          delayMs(100);
          break;
      case 0b110: // Left and front walls detected. Orient right and go forward.
          orientRight();
          delayMs(100);
          goForwardAndThenStop();
          break;
      case 0b111: // Left, front, and right walls detetected. Turn around.
          turnAround();
          delayMs(100);
          break;
      default:
          goForwardAndThenStop();
          break;
      }
	}
  return 0;
}

