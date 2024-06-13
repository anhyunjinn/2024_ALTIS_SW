#include <Arduino.h>
#include "GPS.h"

GPS gps(Serial2,9600);  // GPS 객체 생성

void setup() {
  Serial.begin(115200);
  while(!gps.set()){  // GPS 초기화및 상태 확인
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
