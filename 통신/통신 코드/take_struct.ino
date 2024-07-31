#define RXD2 16
#define TXD2 17

struct mystruct {
  int t;
  float ax, ay, az;
  float angleX, angleY, angleZ;
  int a;
  byte ej;
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
    Serial.print(" t: "); Serial.print(mydata.t);
    Serial.print(" ax: "); Serial.print(mydata.ax);
    Serial.print(" ay: "); Serial.print(mydata.ay);
    Serial.print(" az: "); Serial.print(mydata.az);
    Serial.print(" a: "); Serial.print(mydata.a);
    Serial.print(" ej: "); Serial.println(mydata.ej);
  }
}
