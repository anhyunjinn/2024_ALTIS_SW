// Prac4 - 수동 버튼 추가
// 버튼에는 10kΩ 저항 추가할것

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define ENCODER1_ADDRESS 0x40
#define ENCODER2_ADDRESS 0x41

#define MOTOR_PIN1 2
#define MOTOR_PIN2 3
#define MOTOR_ENA 9
#define BUTTON_PIN 10

Adafruit_BMP280 bmp;
unsigned long startTime;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  startTime = millis();
  
  if (!bmp.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, Adafruit_BMP280::FILTER_OFF);

  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(BUTTON_PIN, INPUT); // 버튼 PIN을 입력 모드로 설정
}

void loop() {
  float altitude = bmp.readAltitude(1013.25);
  int angleX = readEncoder(ENCODER1_ADDRESS);
  int angleY = readEncoder(ENCODER2_ADDRESS);
  double overallAngle = calculateOverallAngle(angleX, angleY);

  Serial.print("X Axis Angle: ");
  Serial.print(angleX);
  Serial.print(" degrees, Y Axis Angle: ");
  Serial.print(angleY);
  Serial.print(" degrees, Overall Tilt: ");
  Serial.print(overallAngle);
  Serial.print(" degrees, Altitude: ");
  Serial.print(altitude);
  Serial.println(" meters");

  if (millis() - startTime > 5000) {
    bool buttonPressed = digitalRead(BUTTON_PIN);
    if (overallAngle >= 45.0 || buttonPressed) { // 조건 수정: 각도가 45도 이상이거나 버튼이 눌린 경우
      rotateMotor(10); // 모터 회전
      Serial.println("Condition met. Motor rotated.");
    }
  }
  delay(500);
}

int readEncoder(int address) {
  Wire.beginTransmission(address);
  Wire.write(0xFE);
  Wire.endTransmission();
  Wire.requestFrom(address, 2);

  if (Wire.available() == 2) {
    unsigned int angle = Wire.read() << 8;
    angle |= Wire.read();
    return angle;
  }
  return 0;
}

double calculateOverallAngle(int angleX, int angleY) {
  return sqrt(angleX * angleX + angleY * angleY);
}

void rotateMotor(int revolutions) {
  digitalWrite(MOTOR_PIN1, HIGH);
  digitalWrite(MOTOR_PIN2, LOW);
  analogWrite(MOTOR_ENA, 128);
  
  long rotationDelay = 200 * revolutions; // 각 회전에 200ms 
  delay(rotationDelay);

  digitalWrite(MOTOR_ENA, LOW); // 모터 정지
}
