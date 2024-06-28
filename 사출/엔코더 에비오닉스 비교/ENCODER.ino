#include <Arduino.h>

const int encoderPin1 = 34;
const int encoderPin2 = 35; 

void setup() {
  Serial.begin(9600);
  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT); 
}

void loop() {
  float encoderValue1 = analogRead(encoderPin1);
  float angle1 = encoderValue1 / 4095 * 360; 

  float encoderValue2 = analogRead(encoderPin2);
  float angle2 = encoderValue2 / 4095 * 360; 

  Serial.print(angle1);
  Serial.print(",");
  Serial.println(angle2);
  }
