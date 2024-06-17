#include <Arduino.h>
#include "TinyGPSPlus.h"
#include "GPS.h"

GPS gps(Serial3, 9600); // GPS 객체 생성

void setup()
{
  Serial.begin(115200);
  if (!gps.set())
  { // GPS 초기화및 상태 확인
    while (1)
    {
      Serial.println("gps 에러");
      delay(1000);
    }
  }
}

void loop()
{
  if (gps.ready())
  {
    float latitude, longitude;
    gps.read(&latitude, &longitude);
    Serial.print("Latitude: ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
  }
}
