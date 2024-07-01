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

bool GPS::read(float *lat, float *lng)
{
    while (_serial.available() > 0)
    {
        if (_gps.encode(_serial.read()))
        {
            if (_gps.location.isValid())
            {
                *lat = _gps.location.lat();
                *lng = _gps.location.lng();
                return true;
            }
        }
    }
    return false;
}
