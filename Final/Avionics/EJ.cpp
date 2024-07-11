#include "EJ.h"
//생성자
EJ::EJ(int motorpin,unsigned long timer,double Alt_Alpha,double sea_level_pressure){
    _motorpin = motorpin;
    _timer = timer;
    _sea_level_pressure = sea_level_pressure;
    _Alt_Alpha =Alt_Alpha;
}

//사출
void EJ::set(){
    servo.attach(_motorpin);
}

void EJ::servomotor(){//서보모터 사출
    servo.write(0);
}

//사출조건
byte EJ::check(){//조건들 넣어야함
    if (abs(angleY) > 45 || abs(angleZ) > 45)
    {
        ej = ej | 0B00001000;
    }
    if (altitudeRate < -2 && altitude > 250)
    {
        ej = ej | 0B00000100;
    }
    if ((micros()-flight_start_time)/1000000 > _timer)
    {
        ej = ej | 0B00000010;
    }
    if(ej){
        servomotor();
    }
    return ej;
}

// 고도
void EJ::Altitude(double *alt, double pressure)
{   static unsigned long before = 0;
    double dt = (micros()-before)/1000000;
    altitude = 44330.0 * (1.0 - pow(pressure / _sea_level_pressure, 0.1903));
    altitude = _Alt_Alpha * altitude + (1 - _Alt_Alpha) * previousAltitude;
    altitudeRate = (altitude - previousAltitude) / dt;
    previousAltitude = altitude;
    before = micros();
    *alt = altitude;
}

// 각도
void EJ::angle(float *_angleX, float *_angleY, float *_angleZ)
{
    // angleX = anglemapping(angleX);
    // angleY = anglemapping(angleY+ini_angleY);
    // angleZ = anglemapping(angleZ+ini_angleZ);
    angleX = angleX;
    angleY = angleY + ini_angleY;
    angleZ = angleZ + ini_angleZ;
    *_angleX = angleX;
    *_angleY = angleY;
    *_angleZ = angleZ;
}

float EJ:: anglemapping(float angle)
{
    if (angle > 180.0)
    {
        return angle -= 360.0;
    }
    else if (angle < -180.0)
    {
        return angle += 360.0;
    }
    return angle;
}

void EJ:: GyrotoEuler(float p, float q, float r)
{
    unsigned long currentTime = micros();
    float dt = (currentTime - prevTime) / 1000000.0;
    prevTime = currentTime;
/*
    float rotationMat[3][3] = {
        {1, sin(prevPhi) * tan(prevTheta), cos(prevPhi) * tan(prevTheta)},
        {0, cos(prevPhi), -sin(prevPhi)},
        {0, sin(prevPhi) / cos(prevTheta), cos(prevPhi) / cos(prevTheta)}};
*/
    float angle[3] = {
        dt * (p + q * sin(angleX) * tan(angleY) + r * cos(angleX) * tan(angleY)),
        dt * (q * cos(angleX) - r * sin(angleX)),
        dt * (q * sin(angleX) / cos(angleY) + r * cos(angleX) / cos(angleY))};

    angleX = (angleX + angle[0]);
    angleY = (angleY + angle[1]);
    angleZ = (angleZ + angle[2]);
}

// 파이/180 = 0.01745329251
// 180/파이 = 57.2957795131
// 1deg = 0.0174533rad
void EJ::GyrotoQuaternion(float raw_gx, float raw_gy, float raw_gz)
{
    static float alpha = 0.2;
    static float _gx, _gy, _gz;
    _gx = alpha * raw_gx + (1 - alpha) * _gx;
    _gy = alpha * raw_gy + (1 - alpha) * _gy;
    _gz = alpha * raw_gz + (1 - alpha) * _gz;

    float q0_dot, q1_dot, q2_dot, q3_dot;
    float q_norm;

    unsigned long currentTime = micros();
    float dt = (currentTime - prevTime) / 1000000.0;
    prevTime = currentTime;

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

    // 쿼터니언 to euler
    angleX = atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2)) * 57.2957795131;
    angleY = asin(2 * (q0 * q2 - q3 * q1)) * 57.2957795131;
    angleZ = atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3)) * 57.2957795131;
}

void EJ:: ini_angle(float _ax, float _ay, float _az ,float* _angleY,float* _angleZ){
    AcctoAngle(_ax,_ay,_az);
    *_angleY = A_angleY;
    *_angleZ = A_angleZ;
}


void EJ:: AcctoAngle(float _ax, float _ay, float _az){
     // 중력 가속도 벡터를 단위 벡터로 정규화
  A_angleZ = atan2(_ay, sqrt(_ax * _ax + _az * _az)) * 180 / PI;
  A_angleY = atan2(-_az, sqrt(_ay * _ay + _ax * _ax)) * 180 / PI;
}

void EJ:: GyrotoAngleRate(float _gx, float _gy, float _gz){
    static unsigned long before = 0;
    double dt = (micros() - before)/1000000.0;
    G_angleXRate = _gx*dt;
    G_angleYRate = _gx*dt;
    G_angleZRate = _gx*dt;
}

void EJ::Complementary()
{
  angleX = angleX + G_angleXRate;
  angleY = C_Alpha * (angleY + G_angleYRate) + (1 - C_Alpha) * A_angleX;
  angleZ = C_Alpha * (angleX + G_angleZRate) + (1 - C_Alpha) * A_angleX;
}