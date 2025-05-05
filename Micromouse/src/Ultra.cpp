#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Ultra.h"






const int trigPin2 = 23;           //connects to the trigger pin on the distance sensor
const int echoPin2 = 22;           //connects to the echo pin on the distance sensor





float distance0 = 0;             //stores the distance measured by the distance sensor
float distance1 = 0;             //stores the distance measured by the distance sensor
float distance2 = 0;             //stores the distance measured by the distance sensor

void setupUltra()
{
    //set up a serial connection with the computer

  pinMode(trigPin0, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(echoPin0, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor

  pinMode(trigPin1, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(echoPin1, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor

  pinMode(trigPin2, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(echoPin2, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor

}

byte loopUltra() {
 
  distance0 = getDistance0();   //variable to store the distance measured by the sensor
  distance1 = getDistance1();   //variable to store the distance measured by the sensor
  distance2 = getDistance2();   //variable to store the distance measured by the sensor

  Serial.print(distance0);     //print the distance that was measured
  Serial.print(" left cm");        //print units after the distance
  Serial.print(" ");         //print a space between the two distances
  Serial.print(distance1);     //print the distance that was measured
  Serial.print(" Center cm");        //print units after the distance
  Serial.println(" ");      //print units after the distance
  Serial.print(distance2);     //print the distance that was measured
  Serial.print(" Right cm");        //print units after the distance
  Serial.println("!");           //move to the next line for clarity

  delayMicroseconds(10);   //delay 50ms between each reading

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
  }
  
  
}

//------------------FUNCTIONS-------------------------------

//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR


float getDistance1()
{
  float echoTime = 0;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance = 0;         //variable to store the distance calculated from the echo time
  
  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  echoTime = pulseIn(echoPin1, HIGH);  // Add timeout to prevent freezing    //use the pulsein command to see how long it takes for the
 

  calculatedDistance = echoTime / 58.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  

  
  return calculatedDistance;              //send back the distance that was calculated
}

float getDistance0()
{
  float echoTime = 0;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance = 0;         //variable to store the distance calculated from the echo time
  
  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin0, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin0, LOW);

  echoTime = pulseIn(echoPin0, HIGH);  // Add timeout to prevent freezing    //use the pulsein command to see how long it takes for the
 

  calculatedDistance = echoTime / 58.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  

  
  return calculatedDistance;              //send back the distance that was calculated
}


float getDistance2()
{
  float echoTime = 0;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance = 0;         //variable to store the distance calculated from the echo time
  
  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  echoTime = pulseIn(echoPin2, HIGH);  // Add timeout to prevent freezing    //use the pulsein command to see how long it takes for the
 

  calculatedDistance = echoTime / 58.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  

  
  return calculatedDistance;              //send back the distance that was calculated
}