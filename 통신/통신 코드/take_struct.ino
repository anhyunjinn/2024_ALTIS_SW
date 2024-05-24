#include <SoftwareSerial.h>

SoftwareSerial lora(2, 3); // RX, TX

struct mystruct {
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
  Serial.begin(9600); // PC와 아두이노의 통신 라인
  lora.begin(9600); // 아두이노와 LoRa 모듈의 통신 라인
}

void loop() {
  if (lora.available() >= sizeof(mydata)) {
    lora.readBytes((byte*)&mydata, sizeof(mydata)); // 수신된 데이터를 구조체로 변환

    Serial.print("Received data: ");
    Serial.print("ax: "); Serial.print(mydata.ax);
    Serial.print(" ay: "); Serial.print(mydata.ay);
    Serial.print(" az: "); Serial.print(mydata.az);
    Serial.print(" z: "); Serial.print(mydata.z);
    Serial.print(" alt: "); Serial.print(mydata.alt);
    Serial.print(" lat: "); Serial.print(mydata.lat);
    Serial.print(" lon: "); Serial.println(mydata.lon);
    Serial.print(" h: "); Serial.print(mydata.h);
    Serial.print(" a: "); Serial.println(mydata.a);
  }
}
