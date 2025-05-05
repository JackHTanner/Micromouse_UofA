#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Ultra.h"


const int trigPin = 11;           //connects to the trigger pin on the distance sensor
const int echoPin = 12;           //connects to the echo pin on the distance sensor


float distance = 0;               //stores the distance measured by the distance sensor

void setupUltra()
{
    //set up a serial connection with the computer

  pinMode(trigPin, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(echoPin, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor

}

void loopUltra() {
 
  distance = getDistance();   //variable to store the distance measured by the sensor

  Serial.print(distance);     //print the distance that was measured
  Serial.println(" cm");      //print units after the distance

delay(50);    //delay 50ms between each reading
  
}

//------------------FUNCTIONS-------------------------------

//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance()
{
  float echoTime = 0;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance = 0;         //variable to store the distance calculated from the echo time
  
  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);  // Add timeout to prevent freezing    //use the pulsein command to see how long it takes for the
 

  calculatedDistance = echoTime / 58.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  

  
  return calculatedDistance;              //send back the distance that was calculated
}
