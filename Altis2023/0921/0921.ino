#include <SoftwareSerial.h>

#define BT_RXD 7
#define BT_TXD 8
SoftwareSerial hc06(BT_RXD, BT_TXD);


#include <Wire.h>
#include <basicMPU6050.h>
basicMPU6050<> imu;

double receivedNumber = 0;
const int MOTOR_PIN = 1;
int val = 4915;  //1.5ms

// rpm constants
const byte interruptPin = 5;  // ì¸í°ë½í¸ í
float mag = 4.0;              //ììì
int count = 0;
int thresh = 20;  //íë³¸ì
unsigned long rpm_previousTime;
double rpm;
int a = 0;
double rocketRotationSpeed;
double wheelRotationSpeed;
//PID constants
double kp = 0.01;
double ki = 0;
double kd = 0;
unsigned long previousTime;
double PID_previousError;
double PID_PError, PID_IError, PID_DError;
int mode = 9;
int t = 0;
String btdata = "";

void setup() {
  Serial.begin(9600);
  hc06.begin(9600);
  Wire.begin();
  imu.setup();
  analogWriteFrequency(1, 50);
  analogWriteResolution(15);
  // put your main code here, to run repeatedly:
  // put your setup code here, to run once:
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), RPM, FALLING);
}

void loop() {
  imu.updateBias();

  // if (Serial.available() > 0) {
  //   receivedNumber = (double)Serial.parseFloat();
  //   val = map(receivedNumber, -100, 100, 1638, 3277);
  //   Serial.println("tap");
  // }
  rocketRotationSpeed = imu.gz();            //ë¡ì¼ ê°ìë (rad/s)
  wheelRotationSpeed = rpm * 2 * PI / 60.0;  //ë¦¬ì¡ìí  ê°ìë (rpm->rad/s)

  //targetWheelSpeed = rocketRotationSpeed * (-Irocket / Iwheel);
  analogWrite(1, val = val + PID(3000, rpm));
  // Serial.println(val);
  if (hc06.available()) {
    btdata = hc06.readStringUntil('\n');
    mode = btdata.substring(0, 1).toInt();
    kp = btdata.substring(1, 6).toFloat();
    ki = btdata.substring(6, 11).toFloat();
    kd = btdata.substring(11, 16).toFloat();
    Serial.print("kp : ");
    Serial.print(kp);
    Serial.print("ki : ");
    Serial.print(ki);
    Serial.print("kd : ");
    Serial.println(kd);
  }
  // Serial.print("rocketRotation : ");
  // Serial.print(rocketRotationSpeed);
  // Serial.print("Wheel : ");
  // Serial.println(wheelRotationSpeed);

  if (Serial.available()) {
    hc06.print("imu : ");
    hc06.print(rocketRotationSpeed);
    hc06.print("\t");
    hc06.print("wheel : ");
    hc06.println(wheelRotationSpeed);
  }
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
    // Serial.print("ìë ¥ë°ì ì«ì: ");
    // Serial.print(receivedNumber);
    Serial.print(" rpm: ");
    Serial.println(rpm);
    count = 0;
    rpm_previousTime = micros();
    a++;
    if (a > 3) {
      receivedNumber += 1;
      a = 0;
    }
  }
}

int PID(double Setpoint, double sensor) {  //ê¸°ë³¸ì ì¸ pidì½ë pidê°ì 700~2300ì ë²ìë¡ ì ííë ì½ë ì gainê° ìºë¦¬ë¸ë ì´ì íì
  unsigned long currentTime = millis();
  double PID_dt = (double)(currentTime - previousTime);  //ìê°ì°¨

  PID_PError = Setpoint - sensor;                          // ìë¬ê°
  PID_IError += PID_PError * PID_dt;                       // ì ë¶ ìë¬
  PID_DError = (PID_PError - PID_previousError) / PID_dt;  // ë¯¸ë¶ ìë¬

  int out = kp * PID_PError + ki * PID_IError + kd * PID_DError;
  PID_previousError = PID_PError;  //ìë¬ ì ë¶, ë¯¸ë¶ì ìí´ ì ì¥
  previousTime = currentTime;      //ìë¬ ì ë¶, ë¯¸ë¶ì ìí´ ì ì¥
  delay(100);
  return out;
}
