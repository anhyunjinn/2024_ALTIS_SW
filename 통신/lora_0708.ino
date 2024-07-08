#define RXD2 16
#define TXD2 17

int count = 0;


struct mystruct {
  unsigned long t;

  float ax;
  float ay;
  float az;

  // float gx;
  // float gy;
  // float gz; //넣기

  float angleX;
  float angleY;
  float angleZ;

  int a;

 // float lat;
 // float lon;//gps빼기

  byte ej;
};

mystruct mydata;


void setup() {
  Serial.begin(115200);                           //PC와 아두이노의 통신 라인
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);  //아두이노와 LoRa 모듈의 통신 라인
  pinMode(18, INPUT);
}

void loop() {
  if (Serial2.available() >= sizeof(mydata)) {
    Serial2.readBytes((byte*)&mydata, sizeof(mydata));  // 수신된 데이터를 구조체로 변환
                                                        //Serial2.println(digitalRead(18));
                                                        //Serial.println(digitalRead(18));

  //  Serial.print("Received data: ");
 //   Serial.print("time: ");
    Serial.print(mydata.t);
    Serial.print(",");
  //  Serial.print("ax: ");
    Serial.print(mydata.ax);
    Serial.print(",");
  //  Serial.print(" ay: ");
    Serial.print(mydata.ay);
    Serial.print(",");
  //  Serial.print(" az: ");
    Serial.print(mydata.az);
    Serial.print(",");
    // Serial.print("gx: ");
    // Serial.print(mydata.gx);
    // Serial.print(" gy: ");
    // Serial.print(mydata.gy);
    // Serial.print(" gz: ");
    // Serial.print(mydata.gz);
  //  Serial.print("angleX: ");
    Serial.print(mydata.angleX);
    Serial.print(",");
   // Serial.print(" angleY: ");
    Serial.print(mydata.angleY);
    Serial.print(",");
   // Serial.print(" angleZ: ");
    Serial.print(mydata.angleZ);
    Serial.print(",");
   // Serial.print(" a: ");
    Serial.print(mydata.a);
    Serial.print(",");
   // Serial.print(" latitude: ");
    //Serial.print(mydata.lat);
    //Serial.print(" longitude: ");
    //Serial.print(mydata.lon);
   // Serial.print(" ej: ");
    Serial.println(mydata.ej);
    count++;


  }

  // static unsigned long before = 0;
  // unsigned long now = millis();
  // if (now-before > 1000){
  //   Serial.println(count);
  //   count =0;
  //   before = now;
  // }


  if (Serial.available() > 0) {
    char c = Serial.read();
    Serial.print(c);
    Serial2.write(c);
  }
}
