#define RXD2 16
#define TXD2 17

struct LORA {
  unsigned long t;
  float ax, ay, az;
  float angleX, angleY, angleZ;
  int a;
  byte ej;
};

LORA data;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  if (Serial2.available()) {
    char c = Serial2.read();
    if (c == 'l') Serial.println("장전");
    if (c == 'e') Serial.println("강제 사출");
  }

  data.t = millis();
  data.ax = random(10);
  data.ay = random(10);
  data.az = random(10);
  data.angleX = random(100);
  data.angleY = random(100);
  data.angleZ = random(100);
  data.a = random(10);
  data.ej = random(10);

  char tDataBuf[sizeof(data) + 2] = {0};
  int bufIndex = 0;
  tDataBuf[bufIndex++] = '#';
  memcpy(&(tDataBuf[bufIndex]), &data, sizeof(data));
  bufIndex += sizeof(data);
  tDataBuf[bufIndex++] = '@';

  Serial2.write((byte*)tDataBuf, bufIndex);

  Serial.print(data.t); Serial.print(",");
  Serial.print(data.ax); Serial.print(",");
  Serial.print(data.ay); Serial.print(",");
  Serial.print(data.az); Serial.print(",");
  Serial.print(data.angleX); Serial.print(",");
  Serial.print(data.angleY); Serial.print(",");
  Serial.print(data.angleZ); Serial.print(",");
  Serial.print(data.a); Serial.print(",");
  Serial.println(data.ej);

  delay(100); // 1초마다 데이터 전송
}
