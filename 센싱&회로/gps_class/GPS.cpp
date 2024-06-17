#include "GPS.h"

bool GPS::set()
{
    _serial.begin(_baudrate);
    if (!_serial)
    {
        return false;
    }
    if (millis() > 5000 && _gps.charsProcessed() < 10)
    {
        return false;
    }
    return true;
}

bool GPS::ready()
{
    bool isValid = false;
    while (_serial.available() > 0)
    {
        if (_gps.encode(_serial.read()))
        {
            if (_gps.location.isValid())
            {
                isValid = true;
            }
        }
    }
    return isValid;
}

void GPS::read(float *lat, float *lng)
{
    *lat = _gps.location.lat();
    *lng = _gps.location.lng();
}
