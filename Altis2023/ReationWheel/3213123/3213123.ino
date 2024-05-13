
/* motor */
double receivedNumber = 0;
const int MOTOR_PIN = 15;
int val= 4915; //1.5ms
//3277 1ms
//6554 2ms

/* rpm */
const byte interruptPin = 13;  // ì¸í°ë½í¸ í
float mag = 4.0;              //ììì
int count = 0;
int thresh = 20;  //íë³¸ì
unsigned long rpm_previousTime;
double rpm;

/* PID */
double kp = 2;
double ki = 0;
double kd = 0;
unsigned long previousTime;
double PID_previousError;
double PID_PError, PID_IError, PID_DError;

int a= 0;

void IRAM_ATTR ISR() {  // rpm ì¸¡ì  ì½ë  (RPM = ë¶ë¹ íì ì, ì¦ COUNT / TIME ì ë¨ìë§ ë§ì¶ë©´ ë¨  )
Serial.println("ì¸í°ë½í¸");
  // count++;

  // if (count >= thresh) {  //íë³¸ ì(thresh)
  //   if(double time_passed = ((micros() - rpm_previousTime) / 60000000.0)>0){//ìê°ì°¨ì´ (min)
  //   double spin = count / mag; //íì ì
  //   rpm = spin / time_passed; //rpm = íì ì / ìê°
  //   }
  //   Serial.print("ìë ¥ë°ì ì«ì: ");
  //   Serial.print(receivedNumber);
  //   Serial.print(" rpm: ");
  //   Serial.println(rpm);
  //   count = 0;
  //   rpm_previousTime = micros();
  //   a++;
  //   if(a > 3){
  //   //receivedNumber += 1;
  //   a=0;
  //   }
  // }
}

void setup() {
  Serial.begin(115200); 

  //motor
  ledcSetup(1, 50, 16); //ì±ë ì¤ì  (ì±ë , ì£¼íì , ë¶í´ë¥)
  ledcAttachPin(MOTOR_PIN, 1); // íìê² ì±ë ë¶ì¬

  //rpm
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    receivedNumber = (double) Serial.parseFloat();
    val = map(receivedNumber,-100,100,3277,6554);
    ledcWrite(1,val);
  }

  // Serial.println(receivedNumber);
  // Serial.println(val);
  // ledcWrite(0, val+pid(2000,rpm_previousTime));
}


int PID(double Setpoint, double sensor) {  //pidì½ë
  unsigned long currentTime = millis();
  unsigned long PID_dt = currentTime - previousTime;  //ìê°ì°¨

  PID_PError = Setpoint - sensor;                          // ìë¬ê°
  PID_IError += PID_PError * PID_dt;                       // ì ë¶ ìë¬
  PID_DError = (PID_PError - PID_previousError) / PID_dt;  // ë¯¸ë¶ ìë¬

  int out = kp * PID_PError + ki * PID_IError + kd * PID_DError;
  PID_previousError = PID_PError;  //ìë¬ ì ë¶, ë¯¸ë¶ì ìí´ ì ì¥
  previousTime = currentTime;      //ìë¬ ì ë¶, ë¯¸ë¶ì ìí´ ì ì¥

  return out;
}
