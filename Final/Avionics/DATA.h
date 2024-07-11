#ifndef DATA_H
#define DATA_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SD.h>
#include <SPI.h>


struct Packet
{
    unsigned long t;

    float ax;
    float ay;
    float az;

     float gx;
     float gy;
     float gz;

    float angleX;
    float angleY;
    float angleZ;

    float p;
    float a;

    float lat;
    float lon;

    byte ej;
};

struct LORA
{
    unsigned long t; //4

    float ax; //4
    float ay; //4
    float az; //4

    float angleX; //4
    float angleY; //4
    float angleZ; //4
    
    float a; //4

    byte ej; //1
    //4*8 + 1 =33
};

class DATA
{
private:

    File file;
    HardwareSerial& _serial;
    int _aux;
    int _baudrate;
    SPIClass& _spi;
    int _cs;
    void struct_lora();
    LORA lora;

public:
    DATA(HardwareSerial& serial,int aux, int baudrate,SPIClass& spi,int cs)    : _serial(serial),_aux(aux), _baudrate(baudrate), _spi(spi),_cs(cs) {}
    bool set();
    void send();
    char read();
    Packet packet;
    void write();
    String dataString;
    String gather();
    String gui();
};

#endif