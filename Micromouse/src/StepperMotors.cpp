#include "StepperMotors.h"
#include "timer.h"
#include <avr/io.h>

void initMotorOutputs(){
    DDRL |= ((1<<DDL4) | (1<<DDL5) | (1<<DDL3) | (1<<DDL2) | (1<<DDL6) | (1<<DDL7));
    DDRG |= ((1<<DDG0) | (1<<DDG1));

}

void rightMotorForward(){
    uint8_t stepIndex = 0;
    const uint8_t halfStepSeq[8][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1}
    };

for (int i = 0; i < 2601; i++){
    //CLOCKWISE:
    // IN4 (PL2)
    if (halfStepSeq[stepIndex][0] == 0){
      PORTL &= ~(1<<PORTL2);
    } else {
      PORTL |= (1<<PORTL2);
    }

    // IN3 (PL3)
    if (halfStepSeq[stepIndex][1] == 0){
      PORTL &= ~(1<<PORTL3);
    } else {
      PORTL |= (1<<PORTL3);
    }

    // IN2 (PL4)
    if (halfStepSeq[stepIndex][2] == 0){
      PORTL &= ~(1<<PORTL4);
    } else {
      PORTL |= (1<<PORTL4);
    }


    // IN1 (PL5)
    if (halfStepSeq[stepIndex][3] == 0){
      PORTL &= ~(1<<PORTL5);
    } else {
      PORTL |= (1<<PORTL5);
    }

    stepIndex = (stepIndex + 1) % 8;
    delayMs(1); // ~125 RPM
  }
}

void rightMotorReverse(){
    uint8_t stepIndex = 0;
    const uint8_t halfStepSeq[8][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1}
    };

    for (int i = 0; i < 2601; i++){
    // COUNTER CLOCKWISE:
    // IN1 (PL5)
    if (halfStepSeq[stepIndex][0] == 0){
      PORTL &= ~(1<<PORTL5);
    } else {
      PORTL |= (1<<PORTL5);
    }

    // IN2 (PL4)
    if (halfStepSeq[stepIndex][1] == 0){
      PORTL &= ~(1<<PORTL4);
    } else {
      PORTL |= (1<<PORTL4);
    }

    // IN3 (PL3)
    if (halfStepSeq[stepIndex][2] == 0){
      PORTL &= ~(1<<PORTL3);
    } else {
      PORTL |= (1<<PORTL3);
    }

    // IN4 (PL2)
    if (halfStepSeq[stepIndex][3] == 0){
      PORTL &= ~(1<<PORTL2);
    } else {
      PORTL |= (1<<PORTL2);
    }

    stepIndex = (stepIndex + 1) % 8;
    delayMs(1); // ~125 RPM
  }
}

void leftMotorReverse(){
    uint8_t stepIndex = 0;
const uint8_t halfStepSeq[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

for (int i = 0; i < 2601; i++){
    //CLOCKWISE:
    // IN1 (PG1) => pin 40
    if (halfStepSeq[stepIndex][0] == 0){
      PORTG &= ~(1<<PORTG1);
    } else {
      PORTG |= (1<<PORTG1);
    }

    // IN2 (PG0) => pin 41
    if (halfStepSeq[stepIndex][1] == 0){
      PORTG &= ~(1<<PORTG0);
    } else {
      PORTG |= (1<<PORTG0);
    }

    // IN3 (PL7) => pin 42
    if (halfStepSeq[stepIndex][2] == 0){
      PORTL &= ~(1<<PORTL7);
    } else {
      PORTL |= (1<<PORTL7);
    }


    // IN4 (PL6) => pin 43
    if (halfStepSeq[stepIndex][3] == 0){
      PORTL &= ~(1<<PORTL6);
    } else {
      PORTL |= (1<<PORTL6);
    }

    stepIndex = (stepIndex + 1) % 8;
    delayMs(1); // ~125 RPM
  }
}

void leftMotorForward(){
    uint8_t stepIndex = 0;
const uint8_t halfStepSeq[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};
for (int i = 0; i < 2601; i++){
    //COUNTER CLOCKWISE:
    // IN4 (PL6) => pin 43
    if (halfStepSeq[stepIndex][0] == 0){
      PORTL &= ~(1<<PORTL6);
    } else {
      PORTL |= (1<<PORTL6);
    }

    // IN3 (PL7) => pin 42
    if (halfStepSeq[stepIndex][1] == 0){
      PORTL &= ~(1<<PORTL7);
    } else {
      PORTL |= (1<<PORTL7);
    }


    // IN2 (PG0) => pin 41
    if (halfStepSeq[stepIndex][2] == 0){
      PORTG &= ~(1<<PORTG0);
          } else {
            PORTG |= (1<<PORTG0);
          }

    //IN1 (PG1) => pin 40
    if (halfStepSeq[stepIndex][3] == 0){
      PORTG &= ~(1<<PORTG1);
    } else {
      PORTG |= (1<<PORTG1);
    }

    stepIndex = (stepIndex + 1) % 8;
    delayMs(1); // ~125 RPM
  }
}