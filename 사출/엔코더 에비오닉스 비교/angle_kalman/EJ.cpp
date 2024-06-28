#include "EJ.h"
//생성자
EJ::EJ(int motor_speed_pin,unsigned long timer,double sea_level_pressure){
    _motor_speed_pin = motor_speed_pin;
    _timer = timer;
    _sea_level_pressure = sea_level_pressure;
}

//사출
void EJ::set(){
    servo.attach(_motor_speed_pin);
}

void EJ::servomotor(){//서보모터 사출
    servo.write(0);
}

//사출조건
byte EJ::check(){//조건들 넣어야함
    // if (abs(roll) > 45 || abs(yaw) > 45)
    // {
    //     ej = ej | 0B00001000;
    // }
    // if (altitude > 10000)
    // {
    //     ej = ej | 0B00000100;
    // }
    // if (millis() > _timer)
    // {
    //     ej = ej | 0B00000010;
    // }

    // if(ej){
    //     servomotor();
    // }
    // return ej;
}

//고도
void EJ::Altitude(double* alt, double pressure){
    altitude = 44330.0 * (1.0 - pow(pressure / _sea_level_pressure, 0.1903));
    * alt = altitude;
}

//각도
void EJ::angle(float* _roll,float* _pitch,float* _yaw){
    roll = anglemapping(roll);
    pitch = anglemapping(pitch);
    yaw = anglemapping(yaw);
    *_roll = roll;
    *_pitch = pitch;
    *_yaw = yaw;
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
        dt * (p + q * sin(prevPhi) * tan(prevTheta) + r * cos(prevPhi) * tan(prevTheta)),
        dt * (q * cos(prevPhi) - r * sin(prevPhi)),
        dt * (q * sin(prevPhi) / cos(prevTheta) + r * cos(prevPhi) / cos(prevTheta))};

    roll = anglemapping(prevPhi + angle[0]);
    pitch = anglemapping(prevTheta + angle[1]);
    yaw = anglemapping(prevPsi + angle[2]);
    
    prevPhi = roll;
    prevTheta = pitch;
    prevPsi = yaw;
}

//파이/180 = 0.01745329251
//180/파이 = 57.2957795131
//1deg = 0.0174533rad
void EJ::GyrotoQuaternion(float _gx, float _gy, float _gz)
{
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
    //쿼터니언 to gyro
    roll = atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2)) * 57.2957795131;
    pitch = asin(2 * (q0 * q2 - q3 * q1)) * 57.2957795131;
    yaw = atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3)) * 57.2957795131;
}