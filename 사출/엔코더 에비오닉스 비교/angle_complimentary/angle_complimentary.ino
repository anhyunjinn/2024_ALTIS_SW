#include <Arduino.h>
#include "IMU.h"
#include "EJ.h"
#include "MovingAverageFilter.h"

// IMU
IMU imu(&Wire1, 1000000, 3, 16, 17);  // I2C 객체, 클럭 속도, 인터럽트 핀 SCL, SDA

// 상보 필터 상수
const float alpha = 0.99;
float angle = 0.0;  // 결합된 각도 값

// 엔코더 핀과 이동 평균 필터
const int encoderPin2 = A7;

float aangle2;
MovingAverageFilter movingAverageFilter(10);  // 이동 평균 필터 객체 생성 (10개의 데이터 이동 평균)

void imu_handler() {
  imu.handler();
}

// EJ
EJ ej(21, 5 * 1000, 1003.6);  // PWM 핀, 타이머, 해면기압

void setup() {
  Serial.begin(115200);

  if (!imu.set()) {
    Serial.print("bmi270 에러");
    while (1)
      ;
  }
  attachInterrupt(digitalPinToInterrupt(imu._interruptPin), imu_handler, RISING);
  ej.set();

  // 엔코더 초기화
  pinMode(encoderPin2, INPUT);
  analogReadResolution(12);
  for (int i = 0; i < 100; i++) {
    float encoderValue2 = analogRead(encoderPin2);
    float angle2 = encoderValue2 / 4095 * 360;
    aangle2 += angle2;
    delay(10);  // 데이터 수집 주기 설정 (10ms)
  }
  aangle2 = aangle2 / 100;
}

void loop() {
  static unsigned long prevTime = 0;


  float ax, ay, az, gx, gy, gz;
  int type = imu.read(&ax, &ay, &az, &gx, &gy, &gz);

  // 엔코더 값 읽기
  float encoderValue2 = analogRead(encoderPin2);
  float angle2 = encoderValue2 / 4095 * 360;

  // 이동 평균 필터 적용
  angle2 = movingAverageFilter.filter(angle2);

  if (type == 2 || type == 3) {
    // 각속도를 각도로 변환
    float rollRate = gx;  // 자이로 데이터를 각속도로 변환

    // 가속도를 이용한 각도 계산
    float accelAngle = atan2(ay, az) * 57.2957795131;  // 라디안을 도로 변환

    // 상보 필터 적용
      unsigned long currentTime = micros();
    float dt = (currentTime - prevTime) / 1000000.0;
    angle = alpha * (angle + rollRate * dt) + (1 - alpha) * accelAngle;

    prevTime = currentTime;

    // IMU x축 각도값 및 엔코더 각도값 출력 (시리얼 플로터용)
    //Serial.print("IMU Angle: ");
    Serial.print(micros());
    Serial.print(",");
    Serial.print(-(angle-90));
    Serial.print(" , ");
    //Serial.print(", Encoder Angle: ");
    Serial.println(angle2 - aangle2);
  }
}
