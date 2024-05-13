#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SoftwareSerial.h>

#define SCL 8
#define SDA 9
#define CSB 10
#define SDO 11

Adafruit_BMP280 bmp(CSB, SDA, SDO, SCL);

MPU6050 mpu6050(Wire);

SoftwareSerial lora(2,3);

int Mosfet = 3;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lora.begin(9600);
  mpu6050.begin();
  mpu6050.setGyroOffsets(-1.98,-0.81,-1.24);

  pinMode(Mosfet,OUTPUT);
  
  if(!bmp.begin()){

    Serial.println(F("ì¼ìê° ì¸ìëì§ ìììµëë¤. ì°ê²° ìíë¥¼ íì¸í´ì£¼ì¸ì."));
    while(1);

  }
}

void loop() {
  getData();
  if(bmp.readAltitude() > 100){
    if(abs(mpu6050.getAngleX())*abs(mpu6050.getAngleX()) + abs(mpu6050.getAngleY())*abs(mpu6050.getAngleY()) > 6400){
      digitalWrite(Mosfet, HIGH);
      Serial.println("ejection");
    }
  }
  if(lora.available()){
    String data = lora.readStringUntil('\n');
    Serial.println(data);
    digitalWrite(Mosfet, HIGH);
  }
}

void getData(){
  mpu6050.update();
  Serial.println("===============");

  Serial.print(F("ì¨ë = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print(F("ê¸°ì = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print(F("ê³ ë = "));
  Serial.print(bmp.readAltitude());
  Serial.println(" m");

  Serial.print("angleX : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\tangleY : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\tangleZ : ");
  Serial.println(mpu6050.getAngleZ());
  
  Serial.println("===================");
}