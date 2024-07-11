#include <Arduino.h>
#include "IMU.h"
#include "GPS.h"
#include "BM.h"
#include "EJ.h"
#include "DATA.h"
#include "interface.h"
#include "pinmap.h"

// 객체 생성
IMU imu(IMU_i2c, IMU_clock, IMU_int, IMU_scl, IMU_sda);
BM bm(BM_i2c, BM_clock, BM_int, BM_scl, BM_sda);
GPS gps(GPS_Serial, GPS_Baudrate);
EJ ej(EJ_motorpin, EJ_timer, Alt_Alpha, EJ_SeaPreasure);
DATA data(DATA_Serial, DATA_Aux, DATA_Baudrate, DATA_SPI, DATA_CS);

// 인터럽트 핸들러
void imu_handler() { imu.handler(); }
void bm_handler() { bm.handler(); }


void calibrateInitialAngles()
{
    for (int c = 0; c < 1000;) {
        interface(1);
        float ax, ay, az, gx, gy, gz;
        int type = imu.read(&ax, &ay, &az, &gx, &gy, &gz);
        if (type == 2 || type == 3) { // 가속도값
            float Y, Z;
            ej.ini_angle(ax, ay, az, &Y, &Z);
            ej.ini_angleY += Y / 1000;
            ej.ini_angleZ += Z / 1000;
            c++;
        }
    }
    ej.angle(&ej.ini_angleX,&ej.ini_angleY,&ej.ini_angleY);
}

void handleGyroData(int type, float gx, float gy, float gz, bool flight, float &angleX, float &angleY, float &angleZ)
{
  if (type == 1 || type == 3)
  { // 자이로값 인터럽트

    data.packet.gx = gx;
    data.packet.gy = gy;
    data.packet.gz = gz;

    ej.GyrotoQuaternion(gx, gy, gz);
    ej.angle(&angleX, &angleY, &angleZ);
    data.packet.angleX = angleX;
    data.packet.angleY = angleY;
    data.packet.angleZ = angleZ;
    // Serial.print(angleY);
    // Serial.print(",");
    // Serial.println(angleZ);
  }
}

void handleAccelData(int type, float ax, float ay, float az, bool& flight, unsigned long& flight_start_time)
{
    if (type == 2 || type == 3) { // 가속도 값 인터럽트
        data.packet.ax = ax;
        data.packet.ay = ay;
        data.packet.az = az;
        if (!flight && ax > 3.0) {
            flight_start_time = micros();
            ej.flight_start_time = flight_start_time;
            flight = true;
            delay(1);
        }
    }
}

void handleBMData()
{
    double p;
    if (bm.read(&p)) { // 압력 측정되면 true 반환과 변수 p에 저장
        double alt;
        ej.Altitude(&alt, p);
        data.packet.p = p;
        data.packet.a = (float)alt;
    }
}

void handleGPSData()
{
    float latitude, longitude;
    if (gps.read(&latitude, &longitude)) { // 좌표 측정되면 true 반환과 변수들에 저장
        data.packet.lat = latitude;
        data.packet.lon = longitude;
    }
}


void setup()
{
    Serial.begin(115200);
    byte condition = 0;
    interface();

    // 센서 초기화
    if (!imu.set()) condition |= 0B00000010;
    attachInterrupt(digitalPinToInterrupt(imu._interruptPin), imu_handler, RISING);

    if (!bm.set()) condition |= 0B00000100;
    attachInterrupt(digitalPinToInterrupt(bm._interruptPin), bm_handler, RISING);

    if (!data.set()) condition |= 0B00001000;

    ej.set();

    // 초기화 실패 시 상태 출력
    while (condition) {
        Serial.println(condition);
    }

    // 캘리브레이션 완료 대기
    // while (data.read() != 'r') {
    //     Serial.println("캘리브레이션 완료");
    // }

    // 초기 각도 설정
    calibrateInitialAngles();
    data.packet.angleY = ej.ini_angleY;
    data.packet.angleZ = ej.ini_angleZ;
    interface(4);
}

void loop()
{
    static unsigned long flight_start_time = 0;
    static bool flight = true;

    float ax, ay, az, gx, gy, gz;
    float angleX, angleY, angleZ;

    int type = imu.read(&ax, &ay, &az, &gx, &gy, &gz);

    handleGyroData(type, gx, gy, gz, flight, angleX, angleY, angleZ);
    handleAccelData(type, ax, ay, az, flight, flight_start_time);

    handleBMData();
    handleGPSData();

    if (data.read() == 'e') { // 강제 사출
        ej.ej |= 0B00000001;
    }

    data.send();

    if (flight) {
        data.packet.ej = ej.check();
        data.write(); // 비행중 SD 카드 저장
        data.packet.t = micros() - flight_start_time;
    }
    //Serial.println(data.gather());
    Serial.println(data.gui());
}
