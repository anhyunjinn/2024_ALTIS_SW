#include "DATA.h"
bool DATA::set()
{
  _serial.begin(_baudrate);                           //PC와 아두이노의 통신 라인
  pinMode(_aux, INPUT);

    if (!SD.begin(_cs)) {
    return false;
  }
}

void DATA::send()
{
    if (digitalRead(_aux) == HIGH)
    {
        static unsigned long before = 0;
        unsigned long now = millis();
        unsigned long dt =now - before;
        if (dt > 100)
        {
            struct_lora();
            _serial.write((byte *)&packet, sizeof(packet));
            before = now;
        }
    }
}

char DATA::read()
{
    if (_serial.available() > 0)
    {
        return (char)_serial.read();
    }
    else
    {
        return '0';
    }
}

void DATA::struct_lora(){
    lora.t = packet.t;
    lora.ax =packet.ax;
    lora.ay = packet.ay;
    lora.az =packet.az;
    lora.angleX = packet.angleX;
    lora.angleY = packet.angleY;
    lora.angleZ= packet.angleZ;
    lora.a = packet.a;
    lora.ej = packet.ej;
}

void DATA::write()
{
    file = SD.open("Flight_Data.txt", FILE_WRITE);
    if (file)
    {
        file.print(packet.t);
        file.print(",");

        file.print(packet.ax);
        file.print(",");
        file.print(packet.ay);
        file.print(",");
        file.print(packet.az);
        file.print(",");

        file.print(packet.gx);
        file.print(",");
        file.print(packet.gy);
        file.print(",");
        file.print(packet.gz);
        file.print(",");

        file.print(packet.angleX);
        file.print(",");
        file.print(packet.angleY);
        file.print(",");
        file.print(packet.angleZ);
        file.print(",");

        file.print(packet.p);
        file.print(",");
        file.print(packet.a);
        file.print(",");

        file.print(packet.lat);
        file.print(",");
        file.print(packet.lon);
        file.print(",");
        
        file.println(packet.ej);
        // close the file:

        file.close();
    }
}
