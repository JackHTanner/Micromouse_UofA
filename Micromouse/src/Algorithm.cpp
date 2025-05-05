#include "timer.h"
#include "Arduino.h"

#define leftSensorTriggerPin = 46; //PL3 
#define leftSensorEchoPin = 47; //PL2

#define frontSensorTriggerPin = 36; //PC1
#define frontSensorEchoPin = 38; //PD7

#define rightSensorTriggerPin = 23; //PA1
#define rightSensorEchoPin = 22; //PA0

unsigned long time1Left;
unsigned long time2Left;
unsigned long pulse_width_Left;
float readingLeft;

unsigned long time1Front;
unsigned long time2Front;
unsigned long pulse_width_Front;
float readingFront;

unsigned long time1Right;
unsigned long time2Right;
unsigned long pulse_width_Right;
float readingRight;

void goForward() {
    // Set motor to maximimum speed
    // Both wheels spin backward at max speed
}

void orientLeft() {
    // Set motor to half speed
    // Right wheel spins forward at half speed
    // Left wheel spins backward at half speed
    delayS(2);
}

void orientRight() {
    // Set motor to half speed
    // Right wheel spins backward at half speed
    // Left sheel spins forward at half speed
    delayS(2);
}

void turnAround() {
    // Set motor to half speed
    // Right wheel spins backward at half speed
    // Left sheel spins forward at half speed
    delayS(4);
}

void stop() {
    // Set motor to zero speed
    // Wheels stop spinning 
}

// Check whether wall is detected using threshold of 12 cm
int wallDetection (float reading) {
    if (reading <= 6.0) {
        return 1;
    }
    else {
        return 0;
    }
}

void setup() {
    // Setup the left sensor

    // Set the trigger pin to output
    DDRL |= (1 << DDL3);
    // Set trigger pin to low 
    PORTL &= ~(1 << 3);

    // Set the echo pin to input
    DDRL &= ~(1 << DDL2);
    // Enable pull-up resistor
    PORTL |= (1 << 2);


    // Setup the front sensor

    // Set the trigger pin to output
    DDRC |= (1 << DDC1);
    // Set trigger pin to low 
    PORTC &= ~(1 << 1);

    // Set the echo pin to input
    DDRD &= ~(1 << DDD7);
    // Enable pull-up resistor
    PORTD |= (1 << 7);

    // Setup the right sensor

    // Set the trigger pin to output
    DDRA |= (1 << DDA1);
    // Set trigger pin to low 
    PORTA &= ~(1 << 1);

    // Set the echo pin to input
    DDRA &= ~(1 << DDA0);
    // Enable pull-up resistor
    PORTA |= (1 << 0);

}


void loop() {
   /**********Polling the left sensor**********/

    // Hold the trigger pin high. Delay for 10 us. Then, hold trigger pin low.
    PORTL |= (1 << 3);
    delayMs(10);
    PORTL &= ~(1 << 3); 

    // Wait for pulse on echo pin
    while (PINL & (1 << PINL2) == 0 );

    // Measure how long the echo pin was held high (pulse width)
    time1Left = micros();
    while (PINL & (1 << PINL2) == 1);
    time2Left = micros();
    pulse_width_Left = time2Left - time1Left;

    // Calculate distance in centimeters. The constant
    // is found in the datasheet, and calculated from the assumed speed
    // of sound in air at sea level (~340 m/s).
    readingLeft = pulse_width_Left / 58.0;

    // Delay 60 milliseconds
    delayMs(60000);

    /**********Polling the front sensor**********/

    // Hold the trigger pin high. Delay for 10 us. Then, hold trigger pin low.
    PORTC |= (1 << 1);
    delayMs(10);
    PORTC &= ~(1 << 1); 

    // Wait for pulse on echo pin
    while (PIND & (1 << PIND7) == 0 );

    // Measure how long the echo pin was held high (pulse width)
    time1Front = micros();
    while (PIND & (1 << PIND7) == 1);
    time2Front = micros();
    pulse_width_Front = time2Front - time1Front;

    // Calculate distance in centimeters. The constant
    // is found in the datasheet, and calculated from the assumed speed
    // of sound in air at sea level (~340 m/s).
    readingFront = pulse_width_Front / 58.0;

    // Delay 60 milliseconds
    delayMs(60000);

    /**********Polling the right sensor**********/

    // Hold the trigger pin high. Delay for 10 us. Then, hold trigger pin low.
    PORTA |= (1 << 1);
    delayMs(10);
    PORTA &= ~(1 << 1); 

    // Wait for pulse on echo pin
    while (PINA & (1 << PINA0) == 0 );

    // Measure how long the echo pin was held high (pulse width)
    time1Right = micros();
    while (PINA & (1 << PINA0) == 1);
    time2Right = micros();
    pulse_width_Right = time2Right - time1Right;

    // Calculate distance in centimeters. The constant
    // is found in the datasheet, and calculated from the assumed speed
    // of sound in air at sea level (~340 m/s).
    readingRight = pulse_width_Right / 58.0;

    // Delay 60 milliseconds
    delayMs(60000);

    // Check whether a wall is detected
    // 1: Wall is detected
    // 0: No wall is detected
    int lSensor = wallDetection(readingLeft);
    int fSensor = wallDetection(readingFront);
    int rSensor = wallDetection(readingRight);

    byte walls = (lSensor << 2) | (fSensor << 1) | (rSensor << 0);

    // Tell the mouse to go forward, go left, go right, turn around,
    // or stop based on which sensors detect walls.
    // Default is for the mouse to stop.
    switch (walls) {
        case 0b000: // No walls detected. Go forward.
          //  goForward();
            break;
        case 0b001: // Right wall detected. Orient left and go forward.
          //  orientLeft();
            delayMs(100);
         //   goForward();
            break;
        case 0b010: // Front wall detected. Orient left and go forward.
        //    orientLeft();
            delayMs(100);
      //      goForward();
            break;
        case 0b011: // Front and right walls detected. Orient left and go forward.
    //        orientLeft();
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
         //   orientRight();
            delayMs(100);
            goForward();
            break;
        case 0b111: // Left, front, and right walls detetected. Turn around.
         //   turnAround();
            delayMs(100);
            break;
        default:
          //  stop();
            break;

        }
    
}
