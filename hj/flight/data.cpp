#include "DATA.h"
bool DATA::set()
{
    _serial.begin(_baudrate); // PC와 아두이노의 통신 라인
    pinMode(_aux, INPUT);

    if (!SD.begin(_cs))
    {
        return false;
    }
    return true;
}

void DATA::send() {
    if (digitalRead(_aux) == HIGH) {
            struct_lora();
            char tDataBuf[40] = {0};
            int bufIndex = 0;

            // 패킷 시작 표시
            tDataBuf[bufIndex++] = '#';

            // LORA 구조체를 버퍼로 복사
            memcpy(&(tDataBuf[bufIndex]), &lora, sizeof(lora));
            bufIndex += sizeof(lora);

            // 패킷 끝 표시
            tDataBuf[bufIndex++] = '@';

            // 데이터 전송
            _serial.write(tDataBuf, bufIndex);
    }
}

char DATA::read()
{
    if (_serial.available() > 0)
    {
        return (char)_serial.read();
    }
    if (Serial.available() > 0)
    {
        return (char)Serial.read();
    }
    return 0;
}

void DATA::struct_lora(){
lora.t = packet.t;
lora.ax = packet.ax;
lora.ay = packet.ay;
lora.az = packet.az;
lora.angleX = packet.angleX;
lora.angleY = packet.angleY;
lora.angleZ = packet.angleZ;
lora.a = packet.a;
lora.ej = packet.ej;
}

String DATA::gui(){
    dataString = "";
    dataString += String(packet.t);
    dataString += ",";

    dataString += String(packet.ax);
    dataString += ",";
    dataString += String(packet.ay);
    dataString += ",";
    dataString += String(packet.az);
    dataString += ",";

    dataString += String(packet.angleX);
    dataString += ",";
    dataString += String(packet.angleY);
    dataString += ",";
    dataString += String(packet.angleZ);
    dataString += ",";

    dataString += String(packet.a);
    dataString += ",";

    dataString += String(packet.ej, BIN);
    return dataString;
}

String DATA::gather(){
    dataString = "";
    dataString += String(packet.t);
    dataString += ",";

    dataString += String(packet.ax);
    dataString += ",";
    dataString += String(packet.ay);
    dataString += ",";
    dataString += String(packet.az);
    dataString += ",";

    dataString += String(packet.gx);
    dataString += ",";
    dataString += String(packet.gy);
    dataString += ",";
    dataString += String(packet.gz);
    dataString += ",";

    dataString += String(packet.angleX);
    dataString += ",";
    dataString += String(packet.angleY);
    dataString += ",";
    dataString += String(packet.angleZ);
    dataString += ",";

    dataString += String(packet.p);
    dataString += ",";
    dataString += String(packet.a);
    dataString += ",";

    dataString += String(packet.lat);
    dataString += ",";
    dataString += String(packet.lon);
    dataString += ",";

    dataString += String(packet.ej, BIN);
    return dataString;
}

void DATA::write()
{
    File file = SD.open("Flight_Data.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (file)
    {
        file.println(gather());
        file.close();
    }
}
