#ifndef DATA_H
#define DATA_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class DATA
{
    private:
    File File;
    int _chipSelect;



    public:
    DATA(int chipSelect);


};
#endif