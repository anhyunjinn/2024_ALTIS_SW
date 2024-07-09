/***
bmi270 설정및 인터럽트 데이터
***/
#include <Arduino.h>
#include "IMU.h"
#include "GPS.h"
#include "BM.h"
#include "EJ.h"
#include "DATA.h"
#include "interface.h"
#include "pinmap.h"

// imu
IMU imu(IMU_i2c, IMU_clock, IMU_int, IMU_scl, IMU_sda); // i2c객체 , 클럭속도, 인터럽트 핀 scl, sda
void imu_handler()
{
  imu.handler();
}
// bm
BM bm(BM_i2c, BM_clock, BM_int, BM_scl, BM_sda); // i2c객체 , 클럭속도, 인터럽트 핀 scl, sda,
void bm_handler()
{
  bm.handler();
}

// gps
GPS gps(GPS_Serial, GPS_Baudrate); // GPS 객체 생성
// EJ
EJ ej(EJ_motorpin, EJ_timer, Alt_Alpha, EJ_SeaPreasure); // pwm핀,타이머,해면기압
// data
DATA data(DATA_Serial, DATA_Aux, DATA_Baudrate, DATA_SPI, DATA_CS); // DATA(HardwareSerial& serial,int aux, int baudrate,SPIClass& spi,int cs)

void setup()
{
  Serial.begin(115200);
  byte condition = 0;
  interface(); //    
  /* 0: 버저 + LED1 + LED2 
  1: LED1 
  2: LED2 
  3: 버저 
  4: 끄기
*/
interface(1);
  // if (!gps.set())
  // {
  //   condition = condition | 0B00000001;
  // }
  delay(1000);
interface(2);
  if (!imu.set()) // 캘리브레이션 때문에 수평상태로 둬야해서 delay 1000줌
  {
    condition = condition | 0B00000010;
  }
  attachInterrupt(digitalPinToInterrupt(imu._interruptPin), imu_handler, RISING);
interface(1);
  if (!bm.set())
  {
    condition = condition | 0B00000100;
  }
  attachInterrupt(digitalPinToInterrupt(bm._interruptPin), bm_handler, RISING);
interface(2);
  if (!data.set())
  {
    condition = condition | 0B00001000;
  }
interface(1);
  ej.set();
interface(2);
  // while (condition)
  // {
  //   Serial.println(condition);
  //   interface(0);
  // }

  while (data.read() != 'r')
  {
    interface(1);
    delay(100);
    interface(2);
    delay(100);
  }

  for (int c = 0; c < 1000;)
  {
    // 초기각도 설정
    interface(1);
    float ax, ay, az, gx, gy, gz;
    int type = imu.read(&ax, &ay, &az, &gx, &gy, &gz);
    if (type == 2 || type == 3) // 가속도값
    {
      static float X, Y, Z;
      ej.AcctoAngle(ax, ay, az);
      ej.angle(&X, &Y, &Z);
      ej.angleX += X / 1000;
      ej.angleY += Y / 1000;
      ej.angleZ += Z / 1000;
      c++;
    }
  }
  interface(2);
}

void loop()
{
  static unsigned long flight_start_time = 0;
  static bool flight = 0;

  data.packet.t = micros() - flight_start_time; // 71분 오버플로우

  float ax, ay, az, gx, gy, gz;
  float angleX, angleY, angleZ;

  int type = imu.read(&ax, &ay, &az, &gx, &gy, &gz);

  if (type == 1 || type == 3)
  { // 자이로값 인터럽트
    data.packet.gx = gx;
    data.packet.gy = gy;
    data.packet.gz = gz;

    if (flight == 1)
    { // 비행중
      ej.GyrotoQuaternion(gx, gy, gz);
      ej.angle(&angleX, &angleY, &angleZ);
      data.packet.angleX = angleX;
      data.packet.angleY = angleY;
      data.packet.angleZ = angleZ;
    }
  }

  if (type == 2 || type == 3)
  { // 가속도 값 인터럽트
    data.packet.ax = ax;
    data.packet.ay = ay;
    data.packet.az = az;
    if (ax > 2)
    {
      flight_start_time = micros();
      ej.flight_start_time = flight_start_time;
      flight = 1;
    }
  }

  double p;
  bool bmp = bm.read(&p); // 압력 측정되면 true 반환과 변수 p에저장
  if (bmp)
  {
    double alt;
    ej.Altitude(&alt, p);
    data.packet.p = p;
    data.packet.a = (float) alt;
  }

  float latitude, longitude;
  bool pos = gps.read(&latitude, &longitude); // 좌표측정되면 true 반환 과 변수들에 저장
  if (pos)
  {
    data.packet.lat = latitude;
    data.packet.lon = longitude;
  }

  if (data.read() == 'e')
  { // 강제 사출
    ej.ej = ej.ej | 0B00000001;
  }

  data.packet.ej = ej.check();
  data.send(); // 무선통신

  if (flight == 1)
    data.write(); // 비행중 sd카드 저장
}
