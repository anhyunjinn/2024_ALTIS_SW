#include <HardwareSerial.h>
 
void setup() {
  Serial.begin(115200);
  Serial.println("Start GPS... ");
  Serial1.begin(9600,SERIAL_8N1,14,27);
}
 
void loop() {
  if(Serial1.available())
  {
    Serial.write(Serial1.read());
  }
}
