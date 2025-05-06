#include <Arduino.h>
#include "timer.h"
#include "PWM.h"
#include "SPI.h"
#include "Ultra.h"
#include "Algorithm.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int currentX = 1;
int currentY = 0;

int targetX = 0;
int targetY = 0;

byte mazeFrames[30][5][5] = {0};  //save maze frames to later animate
byte mazePath[5][5] = {
  {0,0,0,0,0},
  {1,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0}
};

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

void saveMazeFrame(int currentFrame) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      mazeFrames[currentFrame][i][j] = mazePath[i][j];
    }
  }
}


int main () {
  initTimer1();
  initTimer2();
  initMAX7219();
  
  //int currentStep = 0; //current step in the maze
  
  //displayAnimation();
  //orientLeft();
  //orientRight();
  //initialize sensor data
  bool leftWall = getBoolLeft();
  bool frontWall = getBoolFront();
  bool rightWall = getBoolRight();

  while (1) {
    if (leftWall == 0 && frontWall == 0 && rightWall == 0) { //if no walls are detected
      goForward();  //reminder: we need 0.635 of a rotation to advance one cell
      displayForwardAnimation();
    }

    else if (leftWall == 0 && frontWall == 0 && rightWall == 1) { //right wall is detected
      orientLeft();
      displayLeftAnimation();
      goForward();
      displayForwardAnimation();
    }

    else if (leftWall == 0 && frontWall == 1 && rightWall == 0) { //front wall is detected
      orientLeft();
      displayLeftAnimation();
      goForward();
      displayForwardAnimation();
    }

    else if (leftWall == 0 && frontWall == 1 && rightWall == 1) { //front and right walls detected
      orientLeft();
      displayLeftAnimation();
      goForward();
      displayForwardAnimation();
    }

    else if (leftWall == 1 && frontWall == 0 && rightWall == 0) { //left wall is detected
      goForward();
      displayForwardAnimation();
    }

    else if (leftWall == 1 && frontWall == 0 && rightWall == 1) { //left and right walls detected
      goForward();
      displayForwardAnimation();
    }

    else if (leftWall == 1 && frontWall == 1 && rightWall == 0) { //left and front walls detected
      orientRight();
      displayRightAnimation();
      goForward();
      displayForwardAnimation();
    }

    else if (leftWall == 1 && frontWall == 1 && rightWall == 1) { //dead end (all walls detected)
      turnAround();
      goForward();
      displayForwardAnimation();
    }

    bool leftWall = getBoolLeft();
    bool frontWall = getBoolFront();
    bool rightWall = getBoolRight();

  //saveMazeFrame(currentStep);
  //currentStep++;

  displaySolution(mazeFrames);
  }
  return 0;

}



