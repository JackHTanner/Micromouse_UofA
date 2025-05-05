#include <Arduino.h>
#include "timer.h"
#include "SPI.h"
#include "Ultra.h"
#include "Algorithm.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int currentX = 1;
int currentY = 0;

int targetX = 0;
int targetY = 0;

volatile int currDirection = 2; //Start facing SOUTH
//currDirection: 0 = North, 1 = East, 2 = South, 3 = West 
void updateDirection(int LeftorRight) { //Left = 0, Right = 1
      if (currDirection == 1 && LeftorRight == 0) {	//if facing EAST and turn LEFT, new direction is NORTH 
        currDirection = 0;
      }
      else if (currDirection == 1 && LeftorRight == 1) {
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
  setupUltra();
  byte walls;
  byte mazePath[5][5] = {
    {0,0,0,0,0},
    {1,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0}
  };

  while (1) {
    Serial.println("looping...");
    walls = loopUltra();
    switch (walls) {
      case 0b000: // No walls detected. Go forward.
          goForwardAndThenStop();
	  updatePosition();
	  displayAnimation();
          break;
      case 0b001: // Right wall detected. Orient left and go forward.
          orientLeft();
	  updateDirection(0);
          delayMs(100);
          goForwardAndThenStop();
	  updatePosition();
	  displayAnimation();
          break;
      case 0b010: // Front wall detected. Orient left and go forward.
          orientLeft();
	  updateDirection(0);
          delayMs(100);
          goForwardAndThenStop();
	  updatePosition();
	  displayAnimation();
          break;
      case 0b011: // Front and right walls detected. Orient left and go forward.
          orientLeft();
	  updateDirection(0);
          delayMs(100);
          goForwardAndThenStop();
	  updatePosition();
	  displayAnimation();
          break;
      case 0b100: // Left wall detected. Go forward.
          goForwardAndThenStop();
	  updatePosition();
          delayMs(100);
	  displayAnimation();
          break;
      case 0b101: // Left and right walls detected. Go forward.
          goForwardAndThenStop();
	  updatePosition();
          delayMs(100);
	  displayAnimation();
          break;
      case 0b110: // Left and front walls detected. Orient right and go forward.
          orientRight();
	  updateDirection(1);
          delayMs(100);
          goForwardAndThenStop();
	  updatePosition();
	  displayAnimation();
          break;
      case 0b111: // Left, front, and right walls detetected. Turn around.
          turnAround();
	  updateDirection(0);
	  updateDirection(0);
	  displayAnimation();
		//update direction twice to simulate 2 left turns (180 degree turn)
          delayMs(100);
          break;
      default:
          goForwardAndThenStop();
          break;

	  mazePath[currentX][currentY] = 1;
	if (currentX == targetX && currentY == targetY) {
	break;
	}
      }

	  
	}
  return 0;
}

