#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SDA 21  // I2C SDA í ë²í¸
#define BMP_SCL 22  // I2C SCL í ë²í¸

Adafruit_BMP280 bmp; // BMP280 ê°ì²´ ìì±

void setup() {
  Serial.begin(9600);
  if (!bmp.begin(0x76)) { // BMP280 ì´ê¸°í
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure() / 100.0F);
  Serial.println(" hPa");

  delay(1000); // 1ì´ë§ë¤ ë°ì´í° ì¸¡ì 
}
