#include "Algorithm.h"
#include "StepperMotors.h"
#include "timer.h"

void turnOnMotors() {
    PORTC |= ((1 << PC4) | (1 << PC5)); // Turn on motors
}

void goForward() {     
     rightMotorForward();
     leftMotorForward(); 
     
     Serial.println("Moving forward");

     delayMs(1000);
}

void orientLeft() {
    rightMotorForward();
    leftMotorReverse();

    Serial.println("Turning left");
    
    delayMs(1000);
}

void orientRight() {
    leftMotorForward();
    rightMotorReverse();
    Serial.println("Turning right");
    
    delayMs(1000);
}
