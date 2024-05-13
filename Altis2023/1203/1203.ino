#include <SoftwareSerial.h>
#include <PID_v1.h>
#include <Wire.h>
#include <basicMPU6050.h>
#include <SPI.h>
#include <SD.h>

basicMPU6050<> imu;

int Tx = 6; // ì ì¡ ë³´ë´ë í  
int Rx = 5; // ìì  ë°ë í
SoftwareSerial BtSerial(Tx, Rx);

const int MPU_addr = 0x68;
const int pwmPin = 9;
const int directionPin1 = 10;
const int directionPin2 = 8;

double Setpoint, Input, Output;
float rpm = 0.0;
float kp = 0.0;
float ki = 0.0;
float kd = 0.0;
String receivedData = ""; // ë°ì ë°ì´í°ë¥¼ ì ì¥í  ë³ì

const byte interruptPin = 5;
double rocketRotationSpeed;
double wheelRotationSpeed;
float mag = 4.0;              
int count = 0;
int thresh = 20;  //íë³¸ì
int a = 0;
unsigned long rpm_previousTime;

PID pid(&Input, &Output, &Setpoint, kp, ki, kd, DIRECT);

#define SD_CS_PIN 4
File dataFile;

void setup() {
  Wire.begin();
  imu.setup();

  // ë¸ë£¨í¬ì¤ íµì  ì´ê¸°í
  Serial.begin(9600);
  BtSerial.begin(9600);

  // ëª¨í° ëë¼ì´ë² í ì´ê¸°í
  pinMode(pwmPin, OUTPUT);
  pinMode(directionPin1, OUTPUT);
  pinMode(directionPin2, OUTPUT);

  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(-255, 255);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pwmPin), RPM, FALLING);

  //SD ì¹´ë ì´ê¸°í
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD ì¹´ë ì´ê¸°í ì¤í¨");
    return;
  }
  Serial.println("SD ì¹´ë ì´ê¸°í ì±ê³µ");

  // íì¼ ì´ê¸°(ìì¼ë©´ ìì±)
  dataFile = SD.open("data.txt", FILE_WRITE);

  // íì¼ì´ ì´ë¦¬ì§ ìì ê²½ì°
  if (!dataFile) {
    Serial.println("íì¼ ì´ê¸° ì¤í¨");
    return;
  }
  Serial.println("íì¼ ì´ê¸° ì±ê³µ");

  // íì¼ì ë°ì´í° ì°ê¸°
  dataFile.print("rpm: ");
  dataFile.print(rpm);
  dataFile.print("gZ: ");
  dataFile.print(imu.gz());
  dataFile.print("aZ: ");
  dataFile.println(imu.az());
  dataFile.close();
  Serial.println("ë°ì´í°ê° íì¼ì ì ì¥ëììµëë¤.");  
}

void loop() { 
  imu.updateBias();
  
  if (BtSerial.available()) {
    char receivedChar = (char)BtSerial.read();
    
    if (receivedChar != '\n') { // ê°í ë¬¸ìê° ìë ê²½ì°ìë§ ì¶ê°
      receivedData += receivedChar;
    } else {
      // ëì´ì°ê¸°ë¥¼ ê¸°ì¤ì¼ë¡ ë¬¸ìì´ ë¶ë¦¬
      int spaceIndex1 = receivedData.indexOf(' ');
      int spaceIndex2 = receivedData.indexOf(' ', spaceIndex1 + 1);

      if (spaceIndex1 != -1 && spaceIndex2 != -1) {
        String str1 = receivedData.substring(0, spaceIndex1);
        String str2 = receivedData.substring(spaceIndex1 + 1, spaceIndex2);
        String str3 = receivedData.substring(spaceIndex2 + 1);

        kp = str1.toFloat();
        ki = str2.toFloat();
        kd = str3.toFloat();

        Serial.print("kp: ");
        Serial.println(kp);
        Serial.print("ki: ");
        Serial.println(ki);
        Serial.print("kd: ");
        Serial.println(kd);

        pid.SetTunings(kp, ki, kd);
      }
      receivedData = ""; // ë°ì´í°ë¥¼ ì²ë¦¬í í ë³ì ì´ê¸°í
    }
  }

  Input = imu.gz();
  
  pid.Compute();

  int pidOutput = (int)Output;
  int motorSpeed = constrain(motorSpeed, -100, 100);
  motorSpeed = map((int)pidOutput, -255, 255, -100, 100); // PWM ë²ìì ë§ê² ë³í

  if(pidOutput > 0){
    digitalWrite(directionPin1, HIGH);
    digitalWrite(directionPin2, LOW);
  }
  else if (pidOutput < 0) {
    digitalWrite(directionPin1, LOW);
    digitalWrite(directionPin2, HIGH);
  }
  else{
    digitalWrite(directionPin1, LOW);
    digitalWrite(directionPin2, LOW);
  }  
  analogWrite(pwmPin, abs(motorSpeed));
}

void RPM() {  // rpm ì¸¡ì  ì½ë  (RPM = ë¶ë¹ íì ì, ì¦ COUNT / TIME ì ë¨ìë§ ë§ì¶ë©´ ë¨  )
  count++;

  if (count >= thresh) {  //íë³¸ ì(thresh)
    //ìê°ì°¨ì´ (min)
    float time_passed = ((micros() - rpm_previousTime) / 60000000.0);
    //íì ì mag count
    float spin = count / mag;
    //íì ì / ìê°ì°¨ì´ = rpm
    rpm = spin / time_passed;

    Serial.print(" rpm: ");
    Serial.println(rpm);
    count = 0;
    rpm_previousTime = micros();
    a++;
    if (a > 3) {
      a = 0;
    }
  }
}