#include "DATA.h"
bool DATA::set()
{
    _serial.begin(_baudrate); // PC와 아두이노의 통신 라인
    pinMode(_aux, INPUT);

    if (!SD.begin(_cs))
    {
        return false;
    }
    int fileCount = 0;
    fileName = "data" + String(fileCount) + ".txt";

  // 파일 존재 여부 확인
    while (SD.exists(fileName.c_str())) {
    fileCount++; // 파일 번호 증가
    fileName = "data" + String(fileCount) + ".txt"; // 새로운 파일 이름 생성
    }

    File file = SD.open(fileName.c_str(), FILE_WRITE);

    return true;
}

void DATA::send() {
    static unsigned long prev = 0;
    unsigned long dt = millis() - prev;
    
    if (digitalRead(_aux) == HIGH && dt > 100) {
        prev = millis();
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

// SD 카드에 데이터 기록
void DATA::write()
{
  if (file)
  {
    file.println(gather());
  }
}

// 데이터 기록 중지
void DATA::done()
  // 최고 고도 이후 0.5초마다 고도가 안달라질시 데이터 수집 종료
  static unsigned long prev = 0;
  unsigned long dt = millis() - prev;

  if (dt > 500) {
    prev = millis();
    static float prev_a = 0;

    if(data.packet.t > EJ_timer * 1000.0 && abs(data.packet.a - prev_a) < 1.0f && data.packet.a < 5.0f)
    {
      static float prev_a = data.packet.a;
        
      flie.println("Flight Done.");
      file.close();
      flight = false;
    }
  }
}