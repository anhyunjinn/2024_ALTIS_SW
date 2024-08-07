#include "EJ.h"
// 생성자
EJ::EJ(int motorpin, double timer, double Alt_Alpha, double sea_level_pressure)
{
    _motorpin = motorpin;
    _timer = timer;
    _sea_level_pressure = sea_level_pressure;
    _Alt_Alpha = Alt_Alpha;
}

// 사출
void EJ::set()
{
    servo.attach(_motorpin);
}

void EJ::servomotor()
{ // 서보모터 사출
    servo.write(0);
}

void EJ::load()
{ // 서보모터 장전
    servo.write(180);
    delay(500);
    servo.write(90);
}

void EJ::connection_check()
{ // 서보모터 연결 확인
    servo.write(180);
    delay(100);
    servo.write(90);

    delay(100);
    servo.write(0);
    delay(100);
    servo.write(90);
}

// 사출조건
byte EJ::check()
{ // 조건들 넣어야함
    if (flight_start_time != 0 && altitude > 150.0f)
    {
        if (abs(angleY) > 45 || abs(angleZ) > 45)
        {
            ej = ej | 0B00001000;
        }

        if (altitudeRate < 2)
        {
            ej = ej | 0B00000100;
        }

        if ((micros() - flight_start_time) >= _timer*1000000.0)
        {
            ej = ej | 0B00000010;
        }
    }

    if (ej != 0)
    {
        servomotor();
    }
    
    return ej;
}

// 고도
void EJ::Altitude(double *alt, double pressure)
{
    static unsigned long before = 0;
    double dt = (micros() - before) / 1000000.0;
    altitude = 44330.0 * (1.0 - pow(pressure / _sea_level_pressure, 0.1903)) - cal_altitude;
    altitude = _Alt_Alpha * altitude + (1 - _Alt_Alpha) * previousAltitude;
    altitudeRate = (altitude - previousAltitude) / dt;
    previousAltitude = altitude;
    before = micros();
    *alt = altitude;
}

// 각도
void EJ::angle(float *_angleX, float *_angleY, float *_angleZ)
{
    *_angleX = angleX;
    *_angleY = angleY;
    *_angleZ = angleZ;
}

// 파이/180 = 0.01745329251
// 180/파이 = 57.2957795131
// 1deg = 0.0174533rad
void EJ::GyrotoQuaternion(float _gx, float _gy, float _gz)
{
    float q0_dot, q1_dot, q2_dot, q3_dot;
    float q_norm;
    static long prevTime = 0;
    unsigned long currentTime = micros();
    float dt = (currentTime - prevTime) / 1000000.0;
    
    if(dt < 1.0){
    // 각속도를 라디안/초 단위로 변환
    float gx_rad = _gx * 0.01745329251;
    float gy_rad = _gy * 0.01745329251;
    float gz_rad = _gz * 0.01745329251;

    // 쿼터니언 미분 방정식 계산
    q0_dot = 0.5f * (-q1 * gx_rad - q2 * gy_rad - q3 * gz_rad);
    q1_dot = 0.5f * (q0 * gx_rad - q3 * gy_rad + q2 * gz_rad);
    q2_dot = 0.5f * (q3 * gx_rad + q0 * gy_rad - q1 * gz_rad);
    q3_dot = 0.5f * (-q2 * gx_rad + q1 * gy_rad + q0 * gz_rad);

    // 쿼터니언 적분
    q0 += q0_dot * dt;
    q1 += q1_dot * dt;
    q2 += q2_dot * dt;
    q3 += q3_dot * dt;

    // 쿼터니언 정규화
    q_norm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 /= q_norm;
    q1 /= q_norm;
    q2 /= q_norm;
    q3 /= q_norm;

    // 쿼터니언 to 오일러 각도 변환
    angleX = atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2)) * 57.2957795131;
    angleY = asin(2 * (q0 * q2 - q3 * q1)) * 57.2957795131 + ini_angleY;
    angleZ = atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3)) * 57.2957795131 + ini_angleZ;
    }
}

void EJ::ini_angle(float _ax, float _ay, float _az, float *_angleY, float *_angleZ)
{
    A_angleZ = atan2(_ay, sqrt(_ax * _ax + _az * _az)) * 180 / PI;
    A_angleY = atan2(-_az, sqrt(_ay * _ay + _ax * _ax)) * 180 / PI;
    *_angleY = A_angleY;
    *_angleZ = A_angleZ;
}