#include "GPS.h"

bool GPS::set() {
    TinyGPSPlus _gps = TinyGPSPlus();
    _serial.begin(_baudrate);
    if(millis() > 5000 &&_gps.charsProcessed() < 10){
        return false;
    }
    return true;
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


