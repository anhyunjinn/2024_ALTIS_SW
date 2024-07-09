// EJ.h
#ifndef EJ_H
#define EJ_H

#include <Arduino.h>
#include <Servo.h>

class EJ
{
private:
    
    int _motorpin;
    unsigned long _timer;
    double _sea_level_pressure;
    double _Alt_Alpha;
    double previousAltitude;
    double altitudeRate;


    Servo servo;
    
    //angle range(-180~180)
    float anglemapping(float angle);

    // gyro to euler
    unsigned long prevTime = 0;

    double C_Alpha;
    float A_angleX;
    float A_angleY;
    float A_angleZ;

    float G_angleXRate;
    float G_angleYRate;
    float G_angleZRate;

    // gyro to quarternion
    float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
    

public:
    EJ(int motorpin,unsigned long timer,double Alt_Alpha,double sea_level_pressure);
    void set();
    void servomotor();
    unsigned long flight_start_time; 
    double altitude;
    float angleX;
    float angleY;
    float angleZ;

    void Altitude(double* alt, double pressure);
    void GyrotoQuaternion(float _gx, float _gy, float _gz);
    void GyrotoEuler(float p, float q, float r);
    void angle(float *_roll, float *_yaw, float *_pitch);
    void AcctoAngle(float _ax,float _ay,float _az);
    void GyrotoAngleRate(float _gx,float _gy,float _gz);
    void Complementary();
    //Ejection check
    byte ej = 0; //8비트중 뒤에 4개 사출조건(각도,고도,타이머,강제) ex)00001000 이면 각도 조건 참
    byte check();
};

#endif