// GPS.h
#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>

class GPS {
public:
    GPS(HardwareSerial& serial, int baudrate)    : _serial(serial), _baudrate(baudrate) {}

    bool set();
    bool read(float* lat, float* lng);

private:
    HardwareSerial& _serial;
    TinyGPSPlus _gps;
    int _baudrate;
};

#endif // GPS_H
