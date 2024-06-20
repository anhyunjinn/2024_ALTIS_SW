/***
bmi270 설정및 인터럽트 데이터
***/
#include <Wire.h>
#include "BM.h"

BM bm(&Wire, 1000000, 2, 19, 18,1005.7); // i2c객체 , 클럭속도, 인터럽트 핀 scl, sda, 해면기압
// unsigned long before = 0;

void bm_handler()
{
  bm.handler();
  // Serial.println("interrupt");
}

void setup()
{
  Serial.begin(115200);
  Serial.println("setup");
  if (!bm.set())
  {
    Serial.print("에러");
    while(1);
  }
  Serial.println(bm._interruptPin);
  attachInterrupt(digitalPinToInterrupt(bm._interruptPin), bm_handler, RISING);
}

void loop()
{
  double p, a;
  if (bm.ready())
  {
    // unsigned long now = millis();
    // bm.read(&p,&a);
    // Serial.print(p);
    // Serial.print("\t\t");
    // Serial.println(a);
    // Serial.println(now -before);
    // before = now ;
  }
}