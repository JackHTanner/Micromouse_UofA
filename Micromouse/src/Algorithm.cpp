#include "timer.h"
#include "Arduino.h"

void setup() {
    // Set pins D11, D12, and D13 as inputs 
    DDRB &= ~((1 << DDB5) | (1 << DDB6) | (1 << DDB7));
    PORTB |= (1 << PORTB5) | (1 << PORTB6) | (1 << PORTB7);

    Serial.begin(9600);
}

void loop() {
    while (true) {
    // Read the value of D11 and assign it to the left sensor
    int lSensor = PINB & (1 << PINB5);
    // Read the value of D12  and assign it to the right sensor
    int rSensor = PINB & (1 << PINB6);
    // Read the value of D13 and assign it to the front sensor
    int fSensor = PINB & (1 << PINB7);

    // Assign the composite sensor value to 3 bit number
    // 0 : no wall detected
    // 1 : wall detected
    int compositeSensor = (lSensor << 2) | (fSensor << 1) | (rSensor << 0);

    // Tell the mouse to go forward, go left, go right, turn around,
    // or stop based on which sensors detect walls.
    // Default is for the mouse to stop.
    switch (compositeSensor) {
        case 0b000: // No walls detected. Go forward.
            goForward();
            break;
        case 0b001: // Right wall detected. Orient left and go forward.
            orientLeft();
            delayMs(100);
            goForward();
            break;
        case 0b010: // Front wall detected. Orient left and go forward.
            orientLeft();
            delayMs(100);
            goForward();
            break;
        case 0b011: // Front and right walls detected. Orient left and go forward.
            orientLeft();
            delayMs(100);
            goForward();
            break;
        case 0b100: // Left wall detected. Go forward.
            goForward();
            delayMs(100);
            break;
        case 0b101: // Left and right walls detected. Go forward.
            goForward();
            delayMs(100);
            break;
        case 0b110: // Left and front walls detected. Orient right and go forward.
            orientRight();
            delayMs(100);
            goForward();
            break;
        case 0b111: // Left, front, and right walls detetected. Turn around.
            turnAround();
            delayMs(100);
            break;
        default:
            stop();
            break;

        }
    }
}

void goForward() {
    // Set motor to maximimum speed
    // Both wheels spin backward at max speed
}

void orientLeft() {
    // Set motor to half speed
    // Right wheel spins forward at half speed
    // Left wheel spins backward at half speed
    delayS(1);
}

void orientRight() {
    // Set motor to half speed
    // Right wheel spins backward at half speed
    // Left sheel spins forward at half speed
    delayS(1);
}

void turnAround() {
    // Set motor to half speed
    // Right wheel spins backward at half speed
    // Left sheel spins forward at half speed
    delayS(2);
}

void stop() {
    // Set motor to zero speed
    // Wheels stop spinning 
}
