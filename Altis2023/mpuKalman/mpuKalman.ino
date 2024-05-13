#include <SoftwareSerial.h>

SoftwareSerial IMU(2, 3);  // RX, TX

float awa[9], T;
unsigned char buf[9];
unsigned long decimalValue;
String calibration;

void setup() {
  Serial.begin(115200);  // ìë¦¬ì¼ íµì ì ììí©ëë¤.
  IMU.begin(115200);  // Bluetooth íµì ì ììí©ëë¤.
}

void loop() {
  if (Serial.available()) {    
    calibration = Serial.readStringUntil('\n');
    decimalValue = strtoul(calibration.c_str(), NULL, 16);
    IMU.write(0XFF);
    IMU.write(0xAA);
    IMU.write((uint8_t)decimalValue);
  }

  if (IMU.available() > 10) { // 55 header
    if (IMU.find(0x55)) {
      for (int i = 0; i < 10; i++) {
        buf[i] = IMU.read();
      }
      DecodeIMU(buf);
    }
  }
}

void DecodeIMU(unsigned char packet[]) {//51 ,52 ,53 ac gy angle
  switch (packet[0]) {
    case 0x51:
      awa[0] = ((short)(packet[2] << 8 | packet[1])) / 32768.0 * 16;
      awa[1] = ((short)(packet[4] << 8 | packet[3])) / 32768.0 * 16;
      awa[2] = ((short)(packet[6] << 8 | packet[5])) / 32768.0 * 16;
      T = ((short)(packet[8] << 8 | packet[7])) / 340.0 + 36.25;
      break;
    case 0x52:
      awa[3] = ((short)(packet[2] << 8 | packet[1])) / 32768.0 * 2000;
      awa[4] = ((short)(packet[4] << 8 | packet[3])) / 32768.0 * 2000;
      awa[5] = ((short)(packet[6] << 8 | packet[5])) / 32768.0 * 2000;
      T = ((short)(packet[8] << 8 | packet[7])) / 340.0 + 36.25;
      break;
    case 0x53:
      awa[6] = ((short)(packet[2] << 8 | packet[1])) / 32768.0 * 180;
      awa[7] = ((short)(packet[4] << 8 | packet[3])) / 32768.0 * 180;
      awa[8] = ((short)(packet[6] << 8 | packet[5])) / 32768.0 * 180;
      T = ((short)(packet[8] << 8 | packet[7])) / 340.0 + 36.25;
      for (int i = 0; i < 9; i++) {
        Serial.print(awa[i]);
        Serial.print("\t");
      }
      Serial.println();
      break;
  }
}