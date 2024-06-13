#include "GPS.h"

void GPS::begin(HardwareSerial& serial,int baudrate) {
    TinyGPSPlus _gps = TinyGPSPlus();
    _serial = serial;
    _serial.begin(baudrate);
}
bool GPS::ready(){
    while(_serial.available()>0){
        if(_gps.encode(_serial.read()) && _gps.location.isValid()){
            return true;
        }
        return false;
    }
}

void GPS::read(float* lat , float* lng) 
{
    *lat = _gps.location.lat();
    *lng = _gps.location.lng();
}