//#include <I2Cdev.h>
#include <Wire.h>
#include <basicMPU6050.h>

basicMPU6050<> imu;

double receivedNumber;
const int MOTOR_PIN = 1;
double val = 2457;  //1.5ms

//mpu9250
const int MPU_ADDR = 0x68;  // I2Cíµì ì ìí MPU6050ì ì£¼ì

double rocketRotationSpeed, wheelRotationSpeed, targetWheelSpeed =0;
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
    //targetWheelSpeed = rocketRotationSpeed * (-Irocket / Iwheel);
    targetWheelSpeed=0;
    Serial.print("imu:");
    Serial.print(rocketRotationSpeed);
    Serial.print("\t");
    Serial.print("wheel");
    Serial.print(wheelRotationSpeed);
    Serial.print("\t");  //rad/s ë¨ì
    val = val - PID(targetWheelSpeed, rocketRotationSpeed);
    val=constrain(val,1638,3277);
    Serial.print("pid PWM");
    Serial.print(val);
    Serial.print("\t"); 
    Serial.print("rpm");
    Serial.println(rpm);
    analogWrite(1, val);
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
    // Serial.print(" rpm: ");
    // Serial.println(rpm);
    count = 0;
    rpm_previousTime = micros();
  }
}

double PID(double Setpoint, double sensor) {  //ê¸°ë³¸ì ì¸ pidì½ë pidê°ì 700~2300ì ë²ìë¡ ì ííë ì½ë ì gainê° ìºë¦¬ë¸ë ì´ì íì
  unsigned long currentTime = millis();
  double PID_dt = (double)(currentTime - previousTime);  //ìê°ì°¨

  PID_PError = Setpoint - sensor;                          // ìë¬ê°
  PID_IError += PID_PError * PID_dt;                       // ì ë¶ ìë¬
  PID_DError = (PID_PError - PID_previousError) / PID_dt;  // ë¯¸ë¶ ìë¬

  double out = kp * PID_PError + ki * PID_IError + kd * PID_DError;
  PID_previousError = PID_PError;  //ìë¬ ì ë¶, ë¯¸ë¶ì ìí´ ì ì¥
  previousTime = currentTime;      //ìë¬ ì ë¶, ë¯¸ë¶ì ìí´ ì ì¥
  delay(100);
  return out;
}