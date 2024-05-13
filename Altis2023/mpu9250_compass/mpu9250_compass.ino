#include <Wire.h>

const int MPU_addr = 0x68; // MPU-9250ì I2C ì£¼ì

void setup() {\imgres
  Wire.begin();
  Serial.begin(9600);
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 ë ì§ì¤í° ì£¼ì
  Wire.write(0);    // 0ì ì ì¡íì¬ ì ì ê´ë¦¬ ë ì§ì¤í°ë¥¼ ì´ê¸°í
  Wire.endTransmission(true);

  Serial.println("MPU-9250 ì´ê¸°í ìë£");
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); // ê°ìëê³ ë°ì´í°ì ìì ì£¼ì
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true); // 6ë°ì´í¸ ì½ê¸° (ê°ìëê³ì ìì´ë¡ì¤ì½í ë°ì´í°)

  int16_t AcX = Wire.read()<<8|Wire.read(); // Xì¶ ê°ìë
  int16_t AcY = Wire.read()<<8|Wire.read(); // Yì¶ ê°ìë
  int16_t AcZ = Wire.read()<<8|Wire.read(); // Zì¶ ê°ìë

  // ëì¹¨ë° ê° ê³ì°
  float heading = atan2(AcY, AcX);
  if (heading < 0) {
    heading += 2 * PI;
  }

  // ë¼ëì ê°ì ë ë¨ìë¡ ë³í
  heading *= RAD_TO_DEG;

  Serial.println("Compass Heading: " + String(heading));
  
  delay(1000);
}