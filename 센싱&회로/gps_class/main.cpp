#include <Arduino.h>
#include "GPS.h"

GPS gps;  // GPS 객체 생성

void setup() {
  Serial.begin(115200);
  while(!gps.set(Serial1, 9600)){  // GPS 초기화, 시리얼 객체, 보드레이트
  Serial.println("gps 에러");
  }
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
