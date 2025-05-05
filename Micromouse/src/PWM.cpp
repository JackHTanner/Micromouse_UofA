#include "PWM.h"

#include <avr/io.h>

// function that produces an output PWM signal with a frequency and duty cycle. For this example
// I am generating a PWM frequency of 15.625 kHz with a 60% duty cycle.
// I will use Timer 1 in the fast PWM mode #7 in Table 17-2, where my TOP value is 0x3FF (1023)
// In this mode I have a frequency that is fixed by the top value. The only setting that
// I can change is the prescaler to lower the frequency to a specific value.
// Use Fast PWM Mode #14 if you need more flexibilty on the frequency value.

// I will have a prescaler of 1.  The calculation of OCR1A are shown below:
void initPWMTimer3()  {

 //set header pin 2 to output
  DDRC |= (1 << DDC4) | (1 << DDC5);
  // set non-inverting mode - output starts high and then is low, 
  //COM1A0 bit = 0
  //COM1A1 bit =1
  TCCR5A |= (1 << COM5B1);
  TCCR5A &= ~(1 << COM5B0);

  //  Use fast PWM mode 15 bit, top value is determined by Table 17-2 of 0x3FF (1023) 
  //  which determines the PWM frequency.
// for Fast PWM 10bit mode # 15:
// WGM10 =1
// WGM11 =1
// WGM12 = 1
// WGM13 = 1
TCCR5A |=  (1 << WGM50) | (1 << WGM51);

TCCR5B |= (1 << WGM52);
TCCR5B |= (1 << WGM53); 

// PWM frequency calculation for FAST PWM mode on page 148 of datasheet
  //frequency of PWM = (F_clk)/((Prescaler)* (1 +TOP))
  // frequency of PWM = 16Mhz
  // Prescaler = 1
  // TOP value = 0x3FF = 1023 
  // PWM frequency from calculation = 15.625 kHz



// set prescalar CSBits to prescaler of 1
//CS10 =1
//CS11 =0
//CS12 =0
TCCR5B |= (1 << CS50);
TCCR5B &= ~((1 << CS51)  | (1 << CS52));


// the last thing is to set the duty cycle.     
// duty cycle is set by dividing output compare OCR1A value by 1 + TOP value
// the top value is (1 + OCR3A) = 1024
//  calculate OCR3A value => OCR3A = duty cycle(fractional number) * (1 + TOP) 
 OCR5A =  1023; //100%
}

// Output on pin 45 (right motor)
void changeDutyCycle1(uint16_t adcValue){
    // cw
    if (adcValue > 512){
      PORTA |= (1<<PORTA4);
      PORTA &= ~(1<<PORTA5);
      OCR5B = (adcValue - 512) * 2;
    }

    //ccw
    else if (adcValue < 512){
      PORTA &= ~(1<<PORTA4);
      PORTA |= (1<<PORTA5);
      OCR5B = (512 - adcValue) * 2 + 20;
      if (OCR5B > 1023)
        OCR5B = 1023;
    }

    //0
    else{
      PORTA &= ~(1<<PORTA4);
      PORTB &= ~(1<<PORTA5);
      OCR5B = 0;
    }
}

// Output on pin 44 (left motor)
void changeDutyCycle2(uint16_t adcValue){
    // cw
    if (adcValue > 512){
      PORTA |= (1<<PORTA6);
      PORTA &= ~(1<<PORTA7);
      OCR5C = (adcValue - 512) * 2;
    }

    //ccw
    else if (adcValue < 512){
      PORTA &= ~(1<<PORTA6);
      PORTA |= (1<<PORTA7);
      OCR5C = (512 - adcValue) * 2 + 20;
      if (OCR5C > 1023)
        OCR5C = 1023;
    }

    //0
    else{
      PORTA &= ~(1<<PORTA6);
      PORTB &= ~(1<<PORTA7);
      OCR5C = 0;
    }
}