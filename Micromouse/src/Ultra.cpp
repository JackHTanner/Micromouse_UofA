#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Ultra.h"
#include "timer.h"


volatile int distance0 = 0;             //stores the distance measured by the distance sensor
volatile int distance1 = 0;             //stores the distance measured by the distance sensor
volatile int distance2 = 0;             //stores the distance measured by the distance sensor

void setupUltra()
{
   // Setup the left sensor

    // Set the trigger pin to output
    DDRL |= (1 << DDL3);
    // Set trigger pin to low 
    //PORTL &= ~(1 << PL3);

    // Set the echo pin to input
    DDRL &= ~(1 << DDL2);
    // Enable pull-up resistor
    PORTL |= (1 << PL2);            //  FIXME


    // Setup the front sensor

    // Set the trigger pin to output
    DDRC |= (1 << DDC1);
    // Set trigger pin to low 
    PORTC &= ~(1 << PC1);

    // Set the echo pin to input
    DDRD &= ~(1 << DDD7);
    // Enable pull-up resistor
    PORTD |= (1 << PD7);

    // Setup the right sensor

    // Set the trigger pin to output
    DDRA |= (1 << DDA1);
    // Set trigger pin to low 
    PORTA &= ~(1 << PA1);

    // Set the echo pin to input
    DDRA &= ~(1 << DDA0);
    // Enable pull-up resistor
    PORTA |= (1 << PA0);
}

byte loopUltra() {
 
  distance0 = getDistance0();   //variable to store the distance measured by the sensor
 distance1 = getDistance1();   //variable to store the distance measured by the sensor
distance2 = getDistance2();   //variable to store the distance measured by the sensor
 //print the distance that was measured
  Serial.print("Distance: "); //print the distance that was measured
  Serial.print(distance0);     //print the distance that was measured
  Serial.print(" left cm");        //print units after the distance
  Serial.print(" ");         //print a space between the two distances
  Serial.print(distance1);     //print the distance that was measured
  Serial.print(" Center cm");        //print units after the distance
  Serial.println(" ");      //print units after the distance
  Serial.print(distance2);     //print the distance that was measured
  Serial.print(" Right cm");        //print units after the distance
  Serial.println("!");           //move to the next line for clarity

  delayMs(10000);   //delay 50ms between each reading

  if (distance0 < 5 && distance1 < 5 && distance2 < 5) {
    return 0b00000111; // All walls detected
  } else if (distance0 < 5 && distance1 < 5) {
    return 0b00000110; // Left and front walls detected
  } else if (distance0 < 5 && distance2 < 5) {
    return 0b00000101; // Left and right walls detected
  } else if (distance1 < 5 && distance2 < 5) {
    return 0b00000011; // Front and right walls detected
  } else if (distance0 < 5) {
    return 0b00000100; // Left wall detected
  } else if (distance1 < 5) {
    return 0b00000010; // Front wall detected
  } else if (distance2 < 5) {
    return 0b00000001; // Right wall detected
  } else {
  
    Serial.println("No wall detected.");
    return 0b00000000;
  }
  
  
}

//------------------FUNCTIONS-------------------------------

//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR


int getDistance1()
{
  float echoTime = 0;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance = 0;         //variable to store the distance calculated from the echo time
  
  //send out an ultrasonic pulse that's 10ms long
  PORTC |= (1 << PC1);
  delayMs(10000);
  PORTC &= ~(1 << PC1);

  echoTime = pulseIn(38, HIGH);  // Add timeout to prevent freezing    //use the pulsein command to see how long it takes for the
 

  calculatedDistance = echoTime / 58.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  

  
  return calculatedDistance;              //send back the distance that was calculated
}

int getDistance0()
{
  Serial.println("getDistance0() called"); // Debugging line to check if the function is being called
  float echoTime = 0;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance = 0;         //variable to store the distance calculated from the echo time
  
  //send out an ultrasonic pulse that's 10ms long
  PORTL |= (1 << PL3);
  delayMs(10000);
  PORTL &= ~(1 << PL3);

  echoTime = pulseIn(47, HIGH);  // Add timeout to prevent freezing    //use the pulsein command to see how long it takes for the
 

  calculatedDistance = echoTime / 58.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  

  Serial.print("Fully calculated distance"); // line to check the calculated distance");
  return calculatedDistance;              //send back the distance that was calculated
}


int getDistance2()
{
  float echoTime = 0;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance = 0;         //variable to store the distance calculated from the echo time
  
  //send out an ultrasonic pulse that's 10ms long
  PORTA |= (1 << PA1);
  delayMs(10000);
  PORTA &= ~(1 << PA1);


  echoTime = pulseIn(22, HIGH);  // Add timeout to prevent freezing    //use the pulsein command to see how long it takes for the
 

  calculatedDistance = echoTime / 58.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  

  
  return calculatedDistance;              //send back the distance that was calculated
}