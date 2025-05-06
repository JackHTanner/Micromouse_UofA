#include <Arduino.h>
#include <avr/io.h>
#include "timer.h"
#include "SPI.h"
#include "StepperMotors.h"
#include "Algorithm.h"
#include <avr/interrupt.h>
#include <Wire.h>
#include "OPT3101A.h" 
#include "TWI.h"

// I²C address of your OPT3101 (A2–A0 = 000 → 0x58)
#define OPT3101_I2C_ADDR  0x58

OPT3101 sensor;  // your library’s default-address constructor

  bool  getBoolLeft () {
    float d_m = sensor.distanceMillimeters / 1000.0f;
    sensor.setChannel(0);
    sensor.readOutputRegs();
    Serial.println(d_m);
    return (d_m < 0.12) ? true : false;
  }
  
  bool  getBoolFront () {
    float d_m = sensor.distanceMillimeters / 1000.0f;
    sensor.setChannel(1);
    sensor.readOutputRegs();
    Serial.println(d_m);
    return (d_m < 0.12) ? true : false;
  }
  
  bool  getBoolRight () {
    float d_m = sensor.distanceMillimeters / 1000.0f;
    sensor.setChannel(2);
    sensor.readOutputRegs();
    Serial.println(d_m);
    return (d_m < 0.12) ? true : false;
  }
  



  //Eryc Setup function
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

void initializeMotors() {
  // Set the motor control pins as output
  DDRC |= (1 << PC4) | (1 << PC5); // Set PC4 and PC5 as output for motor control
  PORTC &= ~((1 << PC4) | (1 << PC5)); // Initialize motors to off state
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
initMotorOutputs();
init();    // Initialize the Arduino system
Serial.begin(9600);
Wire.begin();
 sensor.init();
 sensor.resetAndWait();
 sensor.configureDefault();
 initializeMotors();

 //Continuous mode @ 512 sub-frames (~130 ms/frame)
  sensor.setContinuousMode();
  sensor.setFrameTiming(512);
  sensor.enableTimingGenerator();

  bool leftWall = getBoolLeft();
  delay(150);
  bool frontWall = getBoolFront();
  delay(150);
  bool rightWall = getBoolRight();
  delay(150);

 


while (!Serial) { /* wait for USB Serial to connect */ }

  //initTimer1();
  //initTimer2();
  //setupUltra();
  initMAX7219();

while(1) {

  Serial.println("Looping...");

  delay(100);


Serial.println("Left: ");
Serial.println(leftWall);
Serial.println("Front: ");
Serial.println(frontWall);
Serial.println("Right: ");
Serial.println(rightWall);

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
delay(150);
bool frontWall = getBoolFront();
delay(150);
bool rightWall = getBoolRight();
delay(150);

//saveMazeFrame(currentStep);
//currentStep++;

}

return 0;
}



