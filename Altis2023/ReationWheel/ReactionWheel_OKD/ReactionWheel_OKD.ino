//#include <I2Cdev.h>
//#include <Wire.h>
//#include <basicMPU6050.h>
//basicMPU6050<> imu;


//motor constant
double receivedNumber;
const int MOTOR_PIN = 1;
double val = 2457;  //1.5ms
double cw = 2600;   //ì ë°©í¥ì¼ë¡ ëìê°ê¸° ììíë ê°
double ccw = 2300;  //ë°ëë°©í¥ì¼ë¡ ëìê°ê¸° ììíë ê°
double Min = 1638;
double Max = 3277;

//mpu9250
const int MPU_ADDR = 0x68;       // I2Cíµì ì ìí MPU6050ì ì£¼ì
double I_per = 0.01 / 0.0006342  //-Irocket / Iwheel

               // wheel speed constant
               const byte interruptPin = 5;  // ì¸í°ë½í¸ í
double mag = 4.0;                            //ììì
int thresh = 20;                             //íë³¸ì
double rpm;
double rad_s;


void RPM() {  // rpm ì¸¡ì  ì½ë  (RPM = ë¶ë¹ íì ì, ì¦ COUNT / TIME ì ë¨ìë§ ë§ì¶ë©´ ë¨  )
  static unsigned long count++;
  if (micros() - rpm_previousTime > 100000 && count >= thresh) {       //0.1ì´ ê°±ì 
    double spin = (count / mag) * 2 * PI;                              //rad
    double time_passed = ((micros() - rpm_previousTime) / 1000000.0);  //s
    rad_s = (spin / time_passed);
    rpm = rad_s * (60 / (2 * PI));
    count = 0;
    static unsigned long rpm_previousTime = micros();
  }
}

double PID(double kp, double kp, double kp, double Setpoint, double sensor) {
  unsigned long PID_dt = (currentTime - millis()) / 1000.0;
  static unsigned long currentTime = millis();

  double PID_PError = Setpoint - sensor;            // ìë¬ê°
  static double PID_IError += PID_PError * PID_dt;  // ì ë¶ ìë¬
  if (PID_IError > 600) {
    PID_IError = 600.0;
  } else if (PID_IError < -600) {
    PID_IError = -600.0;
  }
  double PID_DError = (PID_PError - PID_previousError) / PID_dt;  // ë¯¸ë¶ ìë¬

  double out = kp * PID_PError + ki * PID_IError + kd * PID_DError;
  static double PID_previousError = PID_PError;
  return out;
}


double motor_pwm(double s) {
  if (s < -820.0) {
    s = 1638;
  } else if (-820.0 <= s && s < 0.0) {
    s = ccw + s;
  } else if (s == 0.0) {
    s = 2457;
  } else if (0.0 < s && s <= 820.0) {
    s = cw + s;
  } else if (820.0 < s) {
    s = 3277;
  }
  analogWrite(MOTOR_PIN, s);
  return s;
}

void setup() {
  Serial.begin(115200);
  //imu.setup();
  //imu.setBias();

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), RPM, FALLING);

  analogWriteFrequency(MOTOR_PIN, 50);
  analogWriteResolution(15);
  analogWrite(MOTOR_PIN, 2457);
  Serial.println("Start");
  delay(1000);
}

void loop() {
  //imu.updateBias();
  //val = val + PID(0.1, 0, 0, imu.gz() * (-I_per), rad_s);//p i d target sensor
  double signal = PID(1, 0.1, 0.5, 5000, rpm);
  motor_pwm(signal);
  Serial.print(5000);
  Serial.print("\t");
  Serial.println(rpm);
}
