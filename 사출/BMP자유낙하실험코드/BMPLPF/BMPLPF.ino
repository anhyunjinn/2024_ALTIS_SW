#include <Wire.h>
#include "BM.h"
#include <SD.h>
#include <SPI.h>

BM bm(&Wire, 1000000, 2, 19, 18); // i2c객체 , 클럭속도, 인터럽트 핀 scl, sda, 해면기압
#define SEA_LEVEL_PRESSURE 1002.0
#define CS_PIN 4
unsigned long before = 0;

// 이전 고도 값을 저장할 변수
float previousAltitude1 = 0.0;
float previousAltitude2 = 0.0;
float previousAltitude3 = 0.0;
float previousAltitude4 = 0.0;
float previousAltitude5 = 0.0;
float previousAltitude6 = 0.0;
float previousAltitude7 = 0.0;

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
    fileName = "FLPF" + String(fileIndex) + ".txt";
    if (!SD.exists(fileName))
    {
      break;
    }
    fileIndex++;
  }

  dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile)
  {
    dataFile.println("Time(ms)\tDelta(ms)\tNo Filter\tAlpha2\tAlpha3\tAlpha4\tAlpha5\tAlpha6\tAlpha7");
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

    // 로우 패스 필터 적용
    float alpha1 = 1; // 가중치1 (노필터)
    float filteredAltitude1 = alpha1 * a + (1 - alpha1) * previousAltitude1;
    previousAltitude1 = filteredAltitude1; // 이전 측정값 업데이트

    float alpha2 = 0.95; // 가중치0.95
    float filteredAltitude2 = alpha2 * a + (1 - alpha2) * previousAltitude2;
    previousAltitude2 = filteredAltitude2; // 이전 측정값 업데이트

    float alpha3 = 0.9; // 가중치0.9
    float filteredAltitude3 = alpha3 * a + (1 - alpha3) * previousAltitude3;
    previousAltitude3 = filteredAltitude3; // 이전 측정값 업데이트

    float alpha4 = 0.8; // 가중치0.8
    float filteredAltitude4 = alpha4 * a + (1 - alpha4) * previousAltitude4;
    previousAltitude4 = filteredAltitude4; // 이전 측정값 업데이트

    float alpha5 = 0.6; // 가중치0.6
    float filteredAltitude5 = alpha5 * a + (1 - alpha5) * previousAltitude5;
    previousAltitude5 = filteredAltitude5; // 이전 측정값 업데이트

    float alpha6 = 0.4; // 가중치0.4
    float filteredAltitude6 = alpha6 * a + (1 - alpha6) * previousAltitude6;
    previousAltitude6 = filteredAltitude6; // 이전 측정값 업데이트

    float alpha7 = 0.2; // 가중치0.2
    float filteredAltitude7 = alpha7 * a + (1 - alpha7) * previousAltitude7;
    previousAltitude7 = filteredAltitude7; // 이전 측정값 업데이트

    Serial.print(now);
    Serial.print("\t");
    Serial.print(now - before);
    before = now;
    Serial.print("\t");

    // 필터링된 고도 출력
    Serial.print(F("no filter"));
    Serial.print("\t");
    Serial.print(filteredAltitude1);
    Serial.print(F(""));
    Serial.print("\t");

    Serial.print(F("alpha2"));
    Serial.print("\t");
    Serial.print(filteredAltitude2);
    Serial.print(F(""));
    Serial.print("\t");

    Serial.print(F("alpha3"));
    Serial.print("\t");
    Serial.print(filteredAltitude3);
    Serial.print(F(""));
    Serial.print("\t");

    Serial.print(F("alpha4"));
    Serial.print("\t");
    Serial.print(filteredAltitude4);
    Serial.print(F(""));
    Serial.print("\t");

    Serial.print(F("alpha5"));
    Serial.print("\t");
    Serial.print(filteredAltitude5);
    Serial.print(F(""));
    Serial.print("\t");

    Serial.print(F("alpha6"));
    Serial.print("\t");
    Serial.print(filteredAltitude6);
    Serial.print(F(""));
    Serial.print("\t");

    Serial.print(F("alpha7"));
    Serial.print("\t");
    Serial.print(filteredAltitude7);
    Serial.print(F(""));
    Serial.println("\t");

    // 데이터 파일에 기록
    dataFile = SD.open(fileName, FILE_WRITE);
    if (dataFile)
    {
      dataFile.print(now);
      dataFile.print("\t");
      dataFile.print(now - before);
      dataFile.print("\t");
      dataFile.print(filteredAltitude1);
      dataFile.print("\t");
      dataFile.print(filteredAltitude2);
      dataFile.print("\t");
      dataFile.print(filteredAltitude3);
      dataFile.print("\t");
      dataFile.print(filteredAltitude4);
      dataFile.print("\t");
      dataFile.print(filteredAltitude5);
      dataFile.print("\t");
      dataFile.print(filteredAltitude6);
      dataFile.print("\t");
      dataFile.print(filteredAltitude7);
      dataFile.println();
      dataFile.close();
    }
    else
    {
      Serial.println("Error opening file!");
    }
  }
}
