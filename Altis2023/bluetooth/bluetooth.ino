#include <SoftwareSerial.h>
#include <Wire.h>

int Tx = 6; // ì ì¡ ë³´ë´ë í
int Rx = 5; // ìì  ë°ë í
SoftwareSerial BtSerial(Tx, Rx);

const int pwmPin = 9;
const int directionPin1 = 10;
const int directionPin2 = 8;

int speed;
String receivedData = "";

void setup() {
  Wire.begin();
  Serial.begin(9600);
  BtSerial.begin(9600);

  pinMode(pwmPin, OUTPUT);
  pinMode(directionPin1, OUTPUT);
  pinMode(directionPin2, OUTPUT);

}

void loop() {
  if (BtSerial.available() > 0) {
    int speed = BtSerial.parseInt();
    // ì ì´ê°ì´ -100ë¶í° 100 ì¬ì´ì ë²ìì ìëë¡ ì í
    speed = constrain(speed, -100, 100);
    Serial.println(speed);
    speed = map(speed, -100, 100, -255, 255);
    // ë°©í¥ ì ì´
    if (speed > 0) {
      digitalWrite(directionPin1, HIGH);
      digitalWrite(directionPin2, LOW);
    } else if (speed < 0) {
      digitalWrite(directionPin1, LOW);
      digitalWrite(directionPin2, HIGH);
    } else {
      digitalWrite(directionPin1, LOW);
      digitalWrite(directionPin2, LOW);
    }
    analogWrite(pwmPin, abs(speed));

  }
}