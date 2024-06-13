#include <Arduino.h>
#include "GPS.h"

GPS gps;  // GPS 객체 생성, Serial1 포트 사용

void setup() {
  Serial.begin(115200);  // 시리얼 통신 초기화
  gps.begin(Serial1, 9600);  // GPS 초기화, 9600 baud 사용
}

void loop() {
  if (gps.ready()) {  // GPS 데이터 준비 완료 확인
    float latitude, longitude;
    gps.read(&latitude, &longitude);  // GPS 데이터 읽기

    // 읽은 GPS 데이터 출력
    Serial.print("Latitude: ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
    Serial.println();
  }
}
