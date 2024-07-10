#define RXD2 16
#define TXD2 17



struct mystruct {
    unsigned long t;

    float ax;
    float ay;
    float az;

    float gx;
    float gy;
    float gz;

    float angleX;
    float angleY;
    float angleZ;
    
    float a;

    float lat;
    float lon;

    byte ej;
};

mystruct mydata;


void setup() {
  Serial.begin(9600);                           //PC와 아두이노의 통신 라인
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  //아두이노와 LoRa 모듈의 통신 라인
  pinMode(18, INPUT);
}

void loop() {
  if (Serial2.available() >= sizeof(mydata)) {
    Serial2.readBytes((byte*)&mydata, sizeof(mydata)); // 수신된 데이터를 구조체로 변환

    Serial.print("Received data: ");
    Serial.print("time: "); Serial.print(mydata.t);
    Serial.print("ax: "); Serial.print(mydata.ax);
    Serial.print(" ay: "); Serial.print(mydata.ay);
    Serial.print(" az: "); Serial.print(mydata.az);
    Serial.print("gx: "); Serial.print(mydata.gx);
    Serial.print(" gy: "); Serial.print(mydata.gy);
    Serial.print(" gz: "); Serial.print(mydata.gz);
    Serial.print("angleX: "); Serial.print(mydata.angleX);
    Serial.print(" angleY: "); Serial.print(mydata.angleY);
    Serial.print(" angleZ: "); Serial.print(mydata.angleZ);
    Serial.print(" a: "); Serial.print(mydata.a);
    Serial.print(" latitude: "); Serial.print(mydata.lat);
    Serial.print(" longitude: "); Serial.println(mydata.lon);
    Serial.print(" ej: "); Serial.println(mydata.ej);
  }
  if(Serial.available() > 0 ){
    char c = Serial.read();
    Serial.print(c);
    Serial2.write(c);
  }
}
