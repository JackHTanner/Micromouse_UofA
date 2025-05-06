#include "Algorithm.h"
#include "timer.h"
#include "PWM.h"

void goForward() {

    PORTC |= (1 << PC4) | (1 << PC5);
    // changeDutyCycle1(1023);
    // changeDutyCycle2(1023);
    Serial.println("Moving forward");

    PORTC &= ~((1 << PC4) | (1 << PC5));
    // Set motor to maximimum speed
    // Both wheels spin forward at max speed
   //delayS(1);
    // Set motor to zero speed
    // Wheels stop spinning
}

void orientLeft() {
PORTC |= (1 << PC4) | (0 << PC5);
    // Set motor to half speed
    // Right wheel spins forward at half speed
    // Left wheel spins backward at half speed
    delayS(1);
    PORTC &= ~((1 << PC4) | (1 << PC5));
}

void orientRight() {
    PORTC |= (1 << PC4) | (0 << PC5);
    // Set motor to half speed
    // Right wheel spins backward at half speed
    // Left sheel spins forward at half speed
    delayS(1);
    PORTC &= ~((1 << PC4) | (1 << PC5));
}

void turnAround() {
    PORTC |= (1 << PC4) | (0 << PC5);
    // Set motor to half speed
    // Right wheel spins backward at half speed
    // Left sheel spins forward at half speed
    delayS(4);
    PORTC &= ~((1 << PC4) | (1 << PC5));
}
