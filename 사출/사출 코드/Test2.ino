// 짐벌 각도 추정코드 : 시작 5초 후 통신

#include <Wire.h>

// AS6500 엔코더의 I2C 주소
#define ENCODER1_ADDRESS 0x40
#define ENCODER2_ADDRESS 0x41

unsigned long startTime; // 프로그램 시작 시간을 저장할 변수

void setup() {
  Serial.begin(9600);       // 시리얼 통신 시작
  Wire.begin();             // I2C 통신 시작
  startTime = millis();     // 시작 시간 초기화
}

void loop() {
  if (millis() - startTime > 5000) { // 프로그램 시작 후 5초가 지났는지 확인
    int angle1 = readEncoder(ENCODER1_ADDRESS); // 첫 번째 엔코더의 각도 읽기
    int angle2 = readEncoder(ENCODER2_ADDRESS); // 두 번째 엔코더의 각도 읽기

    // 읽은 각도 값 시리얼 모니터에 출력
    Serial.print("Encoder 1 Angle: ");
    Serial.print(angle1);
    Serial.print(" degrees, Encoder 2 Angle: ");
    Serial.print(angle2);
    Serial.println(" degrees");
  }

  delay(10); // 10ms 대기
}

// 엔코더로부터 각도를 읽는 함수
int readEncoder(int address) {
  Wire.beginTransmission(address);
  Wire.write(0xFE); // 각도를 읽기 위한 레지스터
  Wire.endTransmission();
  Wire.requestFrom(address, 2); // 2바이트 읽기

  if (Wire.available() == 2) {
    unsigned int angle = Wire.read() << 8; // 상위 바이트
    angle |= Wire.read();                  // 하위 바이트와 결합
    return angle;
  }
  return 0; // 에러 시 0 반환
}
