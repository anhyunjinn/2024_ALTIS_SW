#include <SoftwareSerial.h>

SoftwareSerial gprsSerial(7, 8); // RX, TX (ìëì´ë¸ì 7ë² íì ëª¨ëì TXì ì°ê²°íê³  8ë² íì ëª¨ëì RXì ì°ê²°)

struct Sensor {
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
};
Sensor receivedSensor;

void setup() {
  Serial.begin(115200);
  gprsSerial.begin(9600);
}

void loop() {
  if (gprsSerial.available()) {
    // ë°ì´í°ë¥¼ ì½ê³  Sensor êµ¬ì¡°ì²´ë¡ ë³í
    gprsSerial.readBytes((byte *)&receivedSensor, sizeof(receivedSensor));

    // ìì í ë°ì´í°ë¥¼ ìë¦¬ì¼ ëª¨ëí°ë¡ ì¶ë ¥
    printReceivedData();
  }
}

void printReceivedData() {
  Serial.println("Received Data:");
  Serial.print("Acceleration (X, Y, Z): ");
  Serial.print(receivedSensor.ax);
  Serial.print(", ");
  Serial.print(receivedSensor.ay);
  Serial.print(", ");
  Serial.println(receivedSensor.az);
  
  Serial.print("Gyroscope (X, Y, Z): ");
  Serial.print(receivedSensor.gx);
  Serial.print(", ");
  Serial.print(receivedSensor.gy);
  Serial.print(", ");
  Serial.println(receivedSensor.gz);
  
  Serial.print("Altitude: ");
  Serial.println(receivedSensor.alt);
  
  Serial.print("GPS (Lat, Lng): ");
  Serial.print(receivedSensor.lat);
  Serial.print(", ");
  Serial.println(receivedSensor.lng);
  
  Serial.print("Yaw: ");
  Serial.println(receivedSensor.yaw);
  
  Serial.print("Pitch: ");
  Serial.println(receivedSensor.pitch);
  
  Serial.print("Time: ");
  Serial.println(receivedSensor.t);
}
