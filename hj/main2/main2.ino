#include <Arduino.h>
#include "IMU.h"
#include "GPS.h"
#include "BM.h"
#include "EJ.h"
#include "DATA.h"
#include "interface.h"
#include "pinmap.h"

bool flight = false;
unsigned long flight_start_time = 0;

// 객체 생성
IMU imu(IMU_i2c, IMU_clock, IMU_int, IMU_scl, IMU_sda);
BM bm(BM_i2c, BM_clock, BM_int, BM_scl, BM_sda);
GPS gps(GPS_Serial, GPS_Baudrate);
EJ ej(EJ_motorpin, EJ_timer, Alt_Alpha, EJ_SeaPreasure);
DATA data(DATA_Serial, DATA_Aux, DATA_Baudrate, DATA_SPI, DATA_CS);

// 인터럽트 핸들러
void imu_handler() {
  imu.handler();
}
void bm_handler() {
  bm.handler();
}

//imu 데이터 핸들러
void handleGyroData(int type, float gx, float gy, float gz) {
  if (type == 1 || type == 3) {  // 자이로값 인터럽트
    data.packet.gx = gx;
    data.packet.gy = gy;
    data.packet.gz = gz;

    ej.GyrotoQuaternion(gx, gy, gz);
    ej.angle(&data.packet.angleX, &data.packet.angleY, &data.packet.angleZ);
  }
}

void handleAccelData(int type, float ax, float ay, float az) {
  if (type == 2 || type == 3) {  // 가속도 값 인터럽트
    data.packet.ax = ax;
    data.packet.ay = ay;
    data.packet.az = az;
    if (!flight && ax > 3.0) {
      flight_start_time = micros();
      ej.flight_start_time = flight_start_time;
      flight = true;
    }
  }
}

//barometer 핸들러
void handleBMData() {
  double p;
  if (bm.read(&p)) {  // 압력 측정되면 true 반환과 변수 p에 저장
    double alt;
    ej.Altitude(&alt, p);
    data.packet.p = p;
    data.packet.a = (float)alt;
  }
}

//gps 핸들러
void handleGPSData() {
  float latitude, longitude;
  if (gps.read(&latitude, &longitude)) {  // 좌표 측정되면 true 반환과 변수들에 저장
    data.packet.lat = latitude;
    data.packet.lon = longitude;
  }
}

//초기각도
void calibrateInitialAngles() {
  float sumY, sumZ = 0;

  for (int c = 0; c < 1000;) {
    interface(1);
    float ax, ay, az, gx, gy, gz;
    int type = imu.read(&ax, &ay, &az, &gx, &gy, &gz);
    if (type == 2 || type == 3) {  // 가속도값
      float Y, Z;
      ej.ini_angle(ax, ay, az, &Y, &Z);
      sumY += Y;
      sumZ += Z;
      c++;
    }
  }

  ej.ini_angleY = sumY / 1000;
  ej.ini_angleZ = sumZ / 1000;

  data.packet.angleY = ej.ini_angleY;
  data.packet.angleZ = ej.ini_angleZ;
}

//초기고도
void calibrateInitialAltitude() {
  double sum_alt = 0;

  for (int c = 0; c < 1000;) {
    double p;
    if (bm.read(&p)) {
      double alt;
      ej.Altitude(&alt, p);
      sum_alt += alt;
      c++;
    }
  }
  ej.cal_altitude = sum_alt / 1000;
}

void setup() {
  Serial.begin(115200);
  byte condition = 0;

  interface();  //LED 및 버저 핀 지정

  if (!gps.set())  //gps 설정 초기화
    condition |= 0B00010000;

  // 센서 초기화
  if (!imu.set())  //imu 설정 초기화
    condition |= 0B00000010;
  attachInterrupt(digitalPinToInterrupt(imu._interruptPin), imu_handler, RISING);

  if (!bm.set())  //bmp390 설정
    condition |= 0B00000100;
  attachInterrupt(digitalPinToInterrupt(bm._interruptPin), bm_handler, RISING);

  if (!data.set())  //aux핀 및 시리얼 초기화
    condition |= 0B00001000;

  ej.set();  //서보 핀 설정

  // 초기화 실패 시 상태 출력
  while (condition) {
    Serial.println(condition,BIN);
    Serial.println("데이터,bmp,bmi,gps");
  }

  char c;
  // 캘리브레이션 완료 대기
  while (c != 'r') {
    Serial.println("l:장전,c:체크,r:비행준비완료");
    c = data.read();

    if (c == 'l') {  // 사출 장치 장전
      ej.load();
    }

    if (c == 'c') {  // 사출 장치 장전
      ej.connection_check();
    }
  }

  // 초기 각도 설정
  calibrateInitialAngles();
  //초기 고도 설정
  calibrateInitialAltitude();

  interface(4);
}

void loop() {
  float ax, ay, az, gx, gy, gz;
  char c = data.read();

  if (c == 'c') { // 사출 핀 연결 확인
    ej.connection_check();
  }

  if (c == 'e') {  // 강제 사출
    interface(2);
    ej.ej |= 0B00000001;
    ej.servomotor();
  }

  int type = imu.read(&ax, &ay, &az, &gx, &gy, &gz);

  handleGyroData(type, gx, gy, gz);
  handleAccelData(type, ax, ay, az);
  handleBMData();
  handleGPSData();

  data.send();  // 데이터 송신
  data.packet.ej = ej.check();  // 사출 조건 확인

  if (flight) {
    data.packet.t = micros() - flight_start_time; // 시간 기록
    data.write();  // 비행중 SD 카드 저장
    data.done();  // SD 카드 기록 중지 조건 확인 + 기록 중지
  }

  // Serial.println(data.gather());
  Serial.println(data.gui());
}
