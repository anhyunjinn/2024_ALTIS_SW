// AS5600 예제 코드

#include <Wire.h>

#define AS5600_ADDRESS 0x36 // AS5600의 I2C 주소

void setup() {
  Serial.begin(9600);
  Wire.begin(); // I2C 초기화
}

void loop() {
  Wire.beginTransmission(AS5600_ADDRESS);
  Wire.write(0x0C); // 각도 측정치 시작 레지스터
  Wire.endTransmission();
  Wire.requestFrom(AS5600_ADDRESS, 2);

  if (Wire.available() == 2) {
    unsigned int angle = Wire.read() << 8; // 상위 바이트 읽기
    angle |= Wire.read(); // 하위 바이트와 결합
    Serial.print("Angle: ");
    Serial.println(angle * 0.08789); // 값 변환 (360도 / 4096 단계)    
  }

  delay(200);
}
