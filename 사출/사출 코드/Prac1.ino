// 짐벌 코드 : 실시간 고도 출력, 로켓 각도 45도 일시 문구 출력

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// AS6500 엔코더의 I2C 주소
#define ENCODER1_ADDRESS 0x40
#define ENCODER2_ADDRESS 0x41
// BMP280 인스턴스 생성
Adafruit_BMP280 bmp;

unsigned long startTime; // 프로그램 시작 시간을 저장할 변수

void setup() {
  Serial.begin(9600);         // 시리얼 통신 시작
  Wire.begin();               // I2C 통신 시작
  startTime = millis();       // 시작 시간 초기화
  
  if (!bmp.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,      /* 모드 */
			  Adafruit_BMP280::FILTER_OFF);     	/* 필터링 */
}

void loop() {
  float altitude = bmp.readAltitude(1013.25); // 기준 해수면 대기압을 설정하여 고도 측정

  if (millis() - startTime > 5000) { // 프로그램 시작 후 5초가 지났는지 확인
    int angleX = readEncoder(ENCODER1_ADDRESS); // 첫 번째 엔코더의 각도 읽기 (X축)
    int angleY = readEncoder(ENCODER2_ADDRESS); // 두 번째 엔코더의 각도 읽기 (Y축)

    // 전체 각도 계산
    double overallAngle = calculateOverallAngle(angleX, angleY);

    // 읽은 고도, 각도 값 및 전체 각도 시리얼 모니터에 출력
    Serial.print(" degrees, Overall Tilt: ");
    Serial.print(overallAngle);
    Serial.print(" degrees, Altitude: ");
    Serial.print(altitude);
    Serial.println(" meters");

    // 특정 각도 조건 검사
    if (overallAngle >= 45.0) {
      Serial.println("The model is sufficiently tilted.");
    }
  }

  delay(500); // 500ms 대기
}

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

// 전체 기울기 각도를 계산하는 함수
double calculateOverallAngle(int angleX, int angleY) {
  return sqrt(angleX * angleX + angleY * angleY);
}
