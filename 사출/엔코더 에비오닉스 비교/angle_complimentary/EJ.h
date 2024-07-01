// EJ.h
#ifndef EJ_H
#define EJ_H

#include <Arduino.h>
#include <Servo.h>

class EJ
{
private:
    //data,constructor
    double altitude;
    float pitch;
    float yaw;
    float roll;
    
    int _motor_speed_pin;
    int _motor_direct_pin;
    unsigned long _timer;
    double _sea_level_pressure;
    Servo servo;
    
    //angle range(-180~180)
    float anglemapping(float angle);

    // gyro to euler
    unsigned long prevTime = 0;
    float prevPhi, prevTheta, prevPsi;

    // gyro to quarternion
    float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
    

public:
    EJ(int motor_speed_pin,unsigned long timer,double sea_level_pressure);
    void set();
    void servomotor();

    void Altitude(double* alt, double pressure);

    bool ready(float _ax, float _ay, float _az);
    void GyrotoQuaternion(float _gx, float _gy, float _gz);
    void GyrotoEuler(float p, float q, float r);
    void angle(float *_roll, float *_yaw, float *_pitch);
    //Ejection check
    byte ej = 0; //8비트중 뒤에 4개 사출조건(각도,고도,타이머,강제) ex)00001000 이면 각도 조건 참
    byte check();
};

#endif