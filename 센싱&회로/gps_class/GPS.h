#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>

class GPS {
public:
    void begin(HardwareSerial& serial,int baudrate);
    void read(float* lat , float* lng);
    bool ready();

private:
    HardwareSerial& _serial;
    TinyGPSPlus _gps;
};

#endif // GPS_H
