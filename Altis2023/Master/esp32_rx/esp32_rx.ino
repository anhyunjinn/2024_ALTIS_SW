#include <HardwareSerial.h>

// êµ¬ì¡°ì²´ ì ì
struct MyStruct {
  //imu
  float ax, ay, az;
  float gx, gy, gz;
  //alt
  float alt;
  //gps
  float lat;
  float lng;
  //angle
  float yaw;
  float pitch;
  //time
  unsigned long t;
  bool ej;
};

void setup() {
  Serial.begin(115200);                     // ìë¦¬ì¼ íµì  ìì
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // UART2 ì´ê¸°í, RXí: 16, TXí: 17
}
MyStruct receivedData;

void loop() {
  if (Serial2.available() >= sizeof(MyStruct)) {
    // UART2ìì ë°ì´í°ë¥¼ ë°ì ì ìë ë§í¼ ë°ì´í°ê° ëì°©íë©´ ì½ê¸°

    Serial2.readBytes((char *)&receivedData, sizeof(receivedData));

    // ë°ì ë°ì´í°ë¥¼ ì²ë¦¬
    display();
  }

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');  // ìë¦¬ì¼ë¡ë¶í° ì¤ ë°ê¿ ë¬¸ìê¹ì§ì ë¬¸ìì´ ì½ê¸°
    bluetooth.println(data);  // ë¸ë£¨í¬ì¤ë¡ ë°ì´í° ì ì¡
  }
}

void display() {
  Serial.print("ax: ");
  Serial.print(receivedData.ax);
  Serial.print("\tay: ");
  Serial.print(receivedData.ay);
  Serial.print("\taz: ");
  Serial.print(receivedData.az);
  Serial.print("\tgx: ");
  Serial.print(receivedData.gx);
  Serial.print("\tgy: ");
  Serial.print(receivedData.gy);
  Serial.print("\tgz: ");
  Serial.print(receivedData.gz);
  Serial.print("\talt: ");
  Serial.print(receivedData.alt);
  Serial.print("\tlat: ");
  Serial.print(receivedData.lat, 6);  // ììì  6ìë¦¬ê¹ì§ ì¶ë ¥
  Serial.print("\tlng: ");
  Serial.print(receivedData.lng, 6);  // ììì  6ìë¦¬ê¹ì§ ì¶ë ¥
  Serial.print("\tyaw: ");
  Serial.print(receivedData.yaw);
  Serial.print("\tpitch: ");
  Serial.print(receivedData.pitch);
  Serial.print("\tt: ");
  Serial.print(receivedData.t);
  Serial.print("\tej: ");
  Serial.println(receivedData.ej);
}
