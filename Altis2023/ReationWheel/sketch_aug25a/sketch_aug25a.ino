/* 
íì§
*/
#include <Servo.h>

//motor constants
Servo motor;
const int MAX_SIGNAL = 2300;
const int CENTER_SIGNAL = 1500;
const int MIN_SIGNAL = 700;
const int MOTOR_PIN = 5;
int receivedNumber = 1500;

// rpm constants
const byte interruptPin = 2;  // ì¸í°ë½í¸ í
float mag = 4.0;              //ììì
int count = 0;
int thresh = 20;  //íë³¸ì
unsigned long rpm_previousTime;
double rpm;

//PID constants
double kp = 2;
double ki = 5;
double kd = 1;
unsigned long previousTime;
double PID_previousError;
double PID_PError, PID_IError, PID_DError;

int a = 0;

void setup() {  //ìë¦¬ì¼
  Serial.begin(9600);
  //ëª¨í°
  motor.attach(MOTOR_PIN);
  // rpm
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), RPM, FALLING);
  Serial.println("start");
}

void loop() {
  if (Serial.available() > 0) {
    if (receivedNumber >= 700 && receivedNumber <= 2300) {
      receivedNumber = Serial.parseInt();
      Serial.println(receivedNumber);  // ìë ¥ë°ì ì«ìë¥¼ ìë¦¬ì¼ ëª¨ëí°ì ì¶ë ¥
    }
  }
  motor.writeMicroseconds(receivedNumber);
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
    Serial.print("ìë ¥ë°ì ì«ì: ");
    Serial.print(receivedNumber);
    Serial.print(" rpm: ");
    Serial.println(rpm);
    count = 0;
    rpm_previousTime = micros();
    a++;
    if(a > 3){
    receivedNumber += 1;
    a=0;
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

  return out;
}