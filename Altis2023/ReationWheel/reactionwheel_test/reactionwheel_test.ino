//#include <I2Cdev.h>
#include <Wire.h>
#include <basicMPU6050.h>

basicMPU6050<> imu;

double receivedNumber;
const int MOTOR_PIN = 1;
double val = 2457;  //1.5ms

//mpu9250
const int MPU_ADDR = 0x68;  // I2Cíµì ì ìí MPU6050ì ì£¼ì

double rocketRotationSpeed, wheelRotationSpeed, targetWheelSpeed = 0;
double Irocket = 0.01;
double Iwheel = 0.0006342;
int mode = 2;

// rpm constants
const byte interruptPin = 5;  // ì¸í°ë½í¸ í
float mag = 4.0;              //ììì
int count = 0;
int thresh = 20;  //íë³¸ì
unsigned long rpm_previousTime;
double rpm;
double rad_s;

//PID constants
double kp = 0.5;
double ki = 0;
double kd = 0;
unsigned long previousTime;
double PID_previousError;
double PID_PError, PID_IError, PID_DError;

int t = 0;

void setup() {
  Serial.begin(115200);
  imu.setup();
  imu.setBias();
  analogWriteFrequency(1, 50);
  analogWriteResolution(15);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), RPM, FALLING);
  analogWrite(1, 2457);
  Serial.println("Start");
  delay(3000);
}

void loop() {
  imu.updateBias();
  wheelRotationSpeed = rpm * PI / 60.0;  //ë¦¬ì¡ìí  ê°ìë (rpm->rad/s)
  rocketRotationSpeed = imu.gz();        //ë¡ì¼ ê°ìë (rad/s)
  targetWheelSpeed = rocketRotationSpeed * (-Irocket / Iwheel);

  val = val - PID(targetWheelSpeed, rocketRotationSpeed);
  val = constrain(val, 1638, 3277);
  motor_pwm(val);
}

void RPM() {  // rpm ì¸¡ì  ì½ë  (RPM = ë¶ë¹ íì ì, ì¦ COUNT / TIME ì ë¨ìë§ ë§ì¶ë©´ ë¨  )
  count++;
  if (micros() - rpm_previousTime > 100000 && count >= thresh) {      //0.1ì´ ê°±ì 
    float spin = (count / mag) * 2 * PI;                              //rad
    float time_passed = ((micros() - rpm_previousTime) / 1000000.0);  //s
    rad_s = (spin / time_passed);
    rpm = rad_s * (60 / (2 * PI));
    count = 0;
    rpm_previousTime = micros();
  }
}
 // ë»ìí
double PID(double Setpoint, double sensor) {  //ê¸°ë³¸ì ì¸ pidì½ë pidê°ì 700~2300ì ë²ìë¡ ì ííë ì½ë ì gainê° ìºë¦¬ë¸ë ì´ì íì
  unsigned long currentTime = millis();
  double PID_dt = (currentTime - previousTime);  //ìê°ì°¨

  PID_PError = Setpoint - sensor;                          // ìë¬ê°
  PID_IError += PID_PError * PID_dt;                       // ì ë¶ ìë¬
  PID_DError = (PID_PError - PID_previousError) / PID_dt;  // ë¯¸ë¶ ìë¬

  double out = kp * PID_PError + ki * PID_IError + kd * PID_DError;
  PID_previousError = PID_PError;  //ìë¬ ì ë¶, ë¯¸ë¶ì ìí´ ì ì¥
  previousTime = currentTime;      //ìë¬ ì ë¶, ë¯¸ë¶ì ìí´ ì ì¥
  return out;
}

double motor_pwm(double s) {
  if (s < -820) {
    s = 1638;
  } else if (-820 <= s < 0) {
    s = ccw + s;
  } else if (s == 0) {
    s = 2457;
  } else if (0 < s <= 820) {
    s = cw + s;
  } else if (820 < s) {
    s = 3277;
  }
  analogWrite(1, s);
  return s;
}