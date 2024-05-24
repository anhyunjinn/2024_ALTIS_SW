#include <SoftwareSerial.h>

SoftwareSerial lora(2,3);

struct mystruct{
  float ax;
  float ay;
  float az;
  float z;
  float alt;
  float lat;
  float lon;
  int h;
  int a;
};

mystruct mydata;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //PC와 아두이노의 통신 라인
  lora.begin(9600); //아두이노와 LoRa 모듈의 통신 라인
}

void loop() {
  // put your main code here, to run repeatedly:
  mydata.ax = 0.1;
  mydata.ay = 0.2;
  mydata.az = 0.3;
  mydata.z = 0.4;
  mydata.alt = 0.5;
  mydata.lat = 25.3;
  mydata.lon = 125.2;
  mydata.h = 30;
  mydata.a = 10;

  lora.write((byte*)&mydata,sizeof(mydata));
  delay(2000);
}
