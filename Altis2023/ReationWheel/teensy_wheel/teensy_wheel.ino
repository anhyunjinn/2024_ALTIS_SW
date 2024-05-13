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

//PID constants
double kp = 0.01;
double ki = 0;
double kd = 0;
unsigned long previousTime;
double PID_previousError;
double PID_PError, PID_IError, PID_DError;

int t = 0;

void setup() {
  Serial.begin(115200);

  analogWriteFrequency(1, 50);
  analogWriteResolution(15);
  // put your main code here, to run repeatedly:
  // put your setup code here, to run once:
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), RPM, FALLING);
}

void loop() {
  if (Serial.available() > 0) {
    receivedNumber = (double)Serial.parseFloat();
    val = map(receivedNumber, -100, 100, 1638, 3277);
    Serial.println("tap");
  }

  analogWrite(1, val=val+PID(3000,rpm));
  Serial.println(val);
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
  delay(100);
  return out;
}
