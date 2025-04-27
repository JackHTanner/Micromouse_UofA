#include "timer.h"
#include <avr/io.h>

/* Initialize timer 1. Using CTC mode  .*/
void initTimer1(){
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS10);
    OCR1A = 15;
}

void initTimer2() {
    TCCR2A = 0; // Normal mode
    TCCR2B &= ~((1 << CS22) | (1 << CS21));  // Clear CS22 and CS21
    TCCR2B |= (1 << CS22) | (1 << CS21);     // Set CS22 and CS21
    TIMSK2 = (1 << TOIE2); // Enable overflow interrupt
    TCNT2 = 0;
}


/* This delays the program an amount of microsecond specified by unsigned int delay.
* Used for timer 1. 
*/
void delayMs(unsigned int delay){
    unsigned int delaycount = 0;

    TCNT1 = 0; // Start the timer at 0
    TIFR1 |= (1 << OCF1A); // Set the compare flag to start the timer 

    while (delaycount < delay) {
        // If timer has reached the max value of OCR1A,
        // increment delaycount. 
        if (TIFR1 & (1 << OCF1A)) {
            delaycount++;
            TIFR1 |= (1 << OCF1A); // Restart the timer
        }
    }
}

void delayS(unsigned int delay){
    unsigned int count = 0;

    //want a time delay of 1 us, so count = (Time delay * frequency of clock)/Prescalar
    //Count (M) = (1s * 16MHz)/1024 - 1 = 15,624

    //Need to interface a 16 bit module, which means writing the high byte first, then the low byte
    //16 in hex is 00 10
    OCR1A = 15624;
    

    //Turn on timer with prescalar 1024 (bits 1 0 1)
    TCCR1B &= ~(1 << CS11);
    TCCR1B |= ((1 << CS10) | (1 << CS12)); //by setting CS12 CS11 CS10 to 001, turn on clock

    while(count < delay){ //while count is less than delay
        
        //Sets the timer flag to 1
        TIFR1 |= (1 << OCF1A); //logic 1 is flag down
        
        //Set the timer register to 0
        TCNT1 = 0;

        //while flag is down do not do anything
        while(!(TIFR1 & (1 << OCF1A))); //!number is equivalent to number != 0
        //when while loop breaks, (0 & 1) = 0, which IS equal to 0.

        count++; //1 microsecond delay as been achieved, go to next microsecond
    }

    //turn timer off
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));

}