/***
bmi270 설정및 인터럽트 데이터
***/
#include <Arduino.h>
#include "IMU.h"
#include "GPS.h"
#include "BM.h"
#include "EJ.h"
// imu
IMU imu(&Wire1, 1000000, 3, 16, 17); // i2c객체 , 클럭속도, 인터럽트 핀 scl, sda
void imu_handler()
{ 
  // static unsigned long before = 0;
  // unsigned long now = micros();
  // Serial.print("gyro");
  // Serial.println(now - before);
  // before = now;
  imu.handler();
}
// bm
BM bm(&Wire, 1000000, 2, 19, 18); // i2c객체 , 클럭속도, 인터럽트 핀 scl, sda,
void bm_handler()
{
  // static unsigned long before = 0;
  // unsigned long now = millis();
  // Serial.print("bmp");
  // Serial.println(now - before);
  // before = now;
  bm.handler();
}

//gps
GPS gps(Serial3, 9600); // GPS 객체 생성
//EJ
EJ ej(21,5*1000,1003.6);//pwm핀,타이머,해면기압

void setup()
{
    if (!gps.set())
  {
    Serial.println("gps 에러");
    while (1);
  }

  if (!imu.set())
  {
    Serial.print("bmi270 에러");
    while (1);
  }
  attachInterrupt(digitalPinToInterrupt(imu._interruptPin), imu_handler, RISING);
  if (!bm.set())
  {
    Serial.print("bmp380 에러");
    while (1);
  }
  attachInterrupt(digitalPinToInterrupt(bm._interruptPin), bm_handler, RISING);
  ej.set();

}

void loop()
{
  float ax, ay, az, gx, gy, gz;
  int type = imu.read(&ax, &ay, &az, &gx, &gy, &gz);
  if (type == 1 || type == 3)
  {
    Serial.println("자이로값 갱신");
    float x, y, z;
    ej.GyrotoQuaternion(gx, gy, gz);
    ej.angle(&x, &y, &z);
    Serial.print(gx);
    Serial.print("\t");
    Serial.print(gy);
    Serial.print("\t");
    Serial.print(gz);
    Serial.println("\t");
  }
  if(type == 2 || type == 3){
        Serial.println("가속도값 갱신");
    Serial.print(ax);
    Serial.print("\t");
    Serial.print(ay);
    Serial.print("\t");
    Serial.print(az);
    Serial.println("\t");
  }

    double p;
    bool bmp = bm.read(&p);
    if(bmp){
      double alt;
      ej.Altitude(&alt, p);
    }

    float latitude, longitude;
    bool pos = gps.read(&latitude, &longitude);
    if (pos)
    {
      Serial.print(latitude);
      Serial.print("\t");
      Serial.print(longitude);
    }

    if(ej.check()){
      Serial.println("사출!");
    }
}
