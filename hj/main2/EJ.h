// EJ.h
#ifndef EJ_H
#define EJ_H

#include <Arduino.h>
#include <Servo.h>

class EJ
{
private:
    
    int _motorpin;
    double _timer;
    double _sea_level_pressure;
    double _Alt_Alpha;
    double previousAltitude;
    double altitudeRate;


    Servo servo;

    float A_angleX;
    float A_angleY;
    float A_angleZ;

    // gyro to quarternion
    float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
    float sumGx=0.0f , sumGy=0.0f, sumGz=0.0f;

public:
    EJ(int motorpin,double timer,double Alt_Alpha,double sea_level_pressure);

    void set();
    void servomotor();
    void load();
    void connection_check();

    unsigned long flight_start_time; 

    double altitude;
    double cal_altitude;

    float angleX;
    float angleY;
    float angleZ;

    float ini_angleY;
    float ini_angleZ;
    float ini_angleX;

    void ini_angle(float _ax, float _ay, float _az, float *_angleY, float *_angleZ);

    void Altitude(double* alt, double pressure);
    void GyrotoQuaternion(float _gx, float _gy, float _gz);
    void angle(float *_roll, float *_yaw, float *_pitch);

    //Ejection check
    byte ej = 0; //8비트중 뒤에 4개 사출조건(각도,고도,타이머,강제) ex)00001000 이면 각도 조건 참
    byte check();
};
#endif