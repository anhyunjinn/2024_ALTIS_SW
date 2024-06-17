/***
bmi270 설정및 인터럽트 데이터
***/
#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BMI270_Arduino_Library.h"
#include "IMU.h"
IMU imu(&Wire1, 1000000, 3, 16, 17); // i2c객체 , 클럭속도, 인터럽트 핀 scl, sda

void imu_handler()
{
  imu.handler();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("setup");
  if (!imu.set())
  {
    Serial.print("에러");
    while(1);
  }
  attachInterrupt(digitalPinToInterrupt(imu._interruptPin), imu_handler, RISING);
}

void loop()
{
  float ax, ay, az, gx, gy, gz;
  if (imu.ready())
  {
    imu.read(&ax, &ay, &az, &gx, &gy, &gz);
    Serial.println(ax);
  }
}