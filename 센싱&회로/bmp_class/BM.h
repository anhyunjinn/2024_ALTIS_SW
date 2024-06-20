#ifndef BM_H
#define BM_H

#include <Arduino.h>
#include <Wire.h>
#include "SparkFunBMP384.h"


class BM {
public:
    BM(TwoWire* wire, int clock, int interruptPin, int scl, int sda,float SEALEVELPRESSURE_HPA);
    int _interruptPin;
    bool set();
    void read(double* p, double* a);
    bool ready();
    void handler();

private:
    TwoWire &_Wire;
    int _clock;
    int _scl;
    int _sda;
    volatile bool _interruptOccurred;
    float _SEALEVELPRESSURE_HPA;
    BMP384 pressureSensor;
    bmp3_data data;
    double readAltitude(double seaLevel);
};

#endif // BM_H
