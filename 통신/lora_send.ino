#define RXD2 16
#define TXD2 17

struct mystruct {
  unsigned long t;

  float ax;
  float ay;
  float az;

  float angleX;
  float angleY;
  float angleZ;

  int a;

  byte ej;
};

mystruct mydata;

void setup() {
  Serial.begin(9600);                           //PC와 아두이노의 통신 라인
  Serial2.begin(9600);//, SERIAL_8N1, RXD2, TXD2);  //아두이노와 LoRa 모듈의 통신 라인
  pinMode(18, INPUT);
}

void loop() {
  Serial.println(digitalRead(20));
  if (digitalRead(18) == HIGH) {
    static unsigned long before = 0;
    unsigned long now = millis();
    Serial.println(now- before);

    if ((now -before) > 10) {
      mydata.t = millis();
      mydata.ax = randon(10);
      mydata.ay = randon(10);
      mydata.az = randon(10);
      mydata.angleX = randon(100);
      mydata.angleY = randon(100);
      mydata.angleZ = randon(100);
      mydata.a = randon(10);
      mydata.ej = randon(10);
      Serial2.write((byte*)&mydata, sizeof(mydata));
      before = now;
    }
  }
}
