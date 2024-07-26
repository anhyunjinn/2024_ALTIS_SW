// IMU.h
#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BMI270_Arduino_Library.h"

class LPF {
  private:
    float alpha;         // 필터 계수
    float filteredValue; // 필터링된 값

  public:
    // 생성자
    LPF(float alpha) {
      this->alpha = alpha;
      this->filteredValue = 0.0;
    }

    // 필터링 함수
    float filter(float raw) {
      filteredValue = alpha * raw + (1 - alpha) * filteredValue;
      return filteredValue;
    }
};


class IMU
{
private:
    TwoWire &_Wire;
    int _scl;
    int _sda;
    uint8_t i2cAddress = BMI2_I2C_PRIM_ADDR; // 0x68
    // uint8_t i2cAddress = BMI2_I2C_SEC_ADDR; // 0x69
    LPF Fax;
    LPF Fay;
    LPF Faz;

    LPF Fgx;
    LPF Fgy;
    LPF Fgz;
    
    bool _interruptOccurred;
    BMI270 _imu;
    int _clock;
    byte interrupt_type;
    void calibrate();
    float ax,ay,az,gx,gy,gz;

public:
    IMU(TwoWire* wire, int clock, int interruptPin, int scl, int sda);
    bool set();
    int read(float *ax, float *ay, float *az,float *gx, float *gy, float *gz);
    void handler();
    int _interruptPin;
    float cal_ax,cal_ay,cal_az,cal_gx,cal_gy,cal_gz; 
};



#endif