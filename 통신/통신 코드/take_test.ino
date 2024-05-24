#include <SoftwareSerial.h>

SoftwareSerial lora(2,3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //PC하고 아두이노하고의 통신라인!
  lora.begin(9600); //아두이노하고 로라모듈하고의 통신라인!
}

void loop() {
  // put your main code here, to run repeatedly:
  if(lora.available()){
    String data = lora.readStringUntil('\n'); //보내는쪽에서 println으로 보냈기 때문
    Serial.println(data);
  }
}
