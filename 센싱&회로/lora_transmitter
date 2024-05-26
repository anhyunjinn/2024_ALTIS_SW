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

mystruct mydata; //구조체를 변수 선언
void setup() {
  Serial.begin(9600); // serial 속도
  lora.begin(9600); //로라 통신 속도
}

void loop() {
  // 송신할 데이터
  mydata.ax = 0.1;
  mydata.ay = 0.2;
  mydata.az = 0.3;
  mydata.z = 0.4;
  mydata.alt = 0.5;
  mydata.lat = 25.3;
  mydata.lon = 125.2;
  mydata.h = 30;
  mydata.a = 10; //송신부 구조체. 수신부도 구조체 똑같이 할 것 일부만 달라도 오류X, 데이터가 수신안됨

  lora.write((byte*)&mydata,sizeof(mydata)); //mydata구조체의 데이터를 바이트 단위로 접근해 전체 구조체를 로라 모듈 통해 송신
  delay(2000);
}
