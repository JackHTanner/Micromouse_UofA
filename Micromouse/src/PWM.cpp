#include "PWM.h"
#include "Arduino.h"
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
DDRE |= (1 << DDE4);
// set non-inverting mode - output starts high and then is low, 
//COM1A0 bit = 0
//COM1A1 bit =1
TCCR3A |= (1 << COM3B1);
TCCR3A &= ~(1 << COM3B0);

//  Use fast PWM mode 15 bit, top value is determined by Table 17-2 of 0x3FF (1023) 
//  which determines the PWM frequency.
// for Fast PWM 10bit mode # 14:
// WGM30 =0
// WGM31 =1
// WGM32 = 1
// WGM33 = 1
TCCR3A |=  (1 << WGM31);

TCCR3B |= (1 << WGM32);
TCCR3B |= (1 << WGM33); 

// PWM frequency calculation for FAST PWM mode on page 148 of datasheet
//frequency of PWM = (F_clk)/((Prescaler)* (1 +TOP))
// frequency of PWM = 16Mhz
// Prescaler = 1
// TOP value = 0x3FF = 1023 
// PWM frequency from calculation = 1.953 kHz



// set prescalar CSBits to prescaler of 64
//CS10 =1
//CS11 =0
//CS12 =0
TCCR3B |= (1 << CS31) | (1<<CS30);
TCCR3B &= ~(1 << CS32);


// the last thing is to set the duty cycle.     
// duty cycle is set by dividing output compare OCR1A value by 1 + TOP value
// the top value is (1 + OCR3A) = 1024
//  calculate OCR3A value => OCR3A = duty cycle(fractional number) * (1 + TOP) 
//OCR3A =  1023; //100%
ICR3 = 99; // new TOP
OCR3B = ICR3 / 2;
}

void turnOnDutyCycle(){
    TCCR3A |= (1 << COM3B1); // Reconnect OC3B to output pin
}

void turnOffDutyCycle(){
    TCCR3A &= ~(1 << COM3B1); // Disable OC3B output (disconnect from pin)
}