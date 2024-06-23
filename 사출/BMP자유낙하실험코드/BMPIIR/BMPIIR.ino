#include <Wire.h>
#include "BM.h"
#include <SD.h>
#include <SPI.h>

BM bm(&Wire, 1000000, 2, 19, 18); // i2c 객체, 클럭 속도, 인터럽트 핀, scl, sda, 해면기압
#define SEA_LEVEL_PRESSURE 1002.0
#define CS_PIN 4
unsigned long before = 0;

File dataFile;
String fileName;

void bm_handler()
{
  bm.handler();
  // Serial.println("interrupt");
}

float calculateAltitude(float pressure)
{
  return (1.0 - pow((pressure / SEA_LEVEL_PRESSURE), 0.190284)) * 44330.77;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("setup");
  
  if (!bm.set())
  {
    Serial.print("에러");
    while (1);
  }
  Serial.println(bm._interruptPin);
  attachInterrupt(digitalPinToInterrupt(bm._interruptPin), bm_handler, RISING);

  // SD 카드 초기화
  if (!SD.begin(CS_PIN))
  {
    Serial.println("SD card initialization failed!");
    while (1);
  }

  // 고유한 파일 이름 생성
  int fileIndex = 0;
  while (true)
  {
    fileName = "IRR127a" + String(fileIndex) + ".txt";
    if (!SD.exists(fileName))
    {
      break;
    }
    fileIndex++;
  }

  dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile)
  {
    dataFile.println("Altitude\tDelta(ms)\tTimestamp(ms)");
    dataFile.close();
  }
  else
  {
    Serial.println("Error opening file!");
  }
}

void loop()
{
  double p;
  if (bm.ready())
  {
    bm.read(&p);
    double a = calculateAltitude(p);
    
    unsigned long now = millis();
    unsigned long delta = now - before;
    before = now;

    Serial.print(a);
    Serial.print("\t\t");
    Serial.print(delta);
    Serial.print("\t\t");
    Serial.println(now);

    // 데이터 파일에 기록
    dataFile = SD.open(fileName, FILE_WRITE);
    if (dataFile)
    {
      dataFile.print(now);
      dataFile.print("\t");
      dataFile.print(delta);
      dataFile.print("\t");
      dataFile.println(a);
      dataFile.close();
    }
    else
    {
      Serial.println("Error opening file!");
    }
  }
}
