#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200); // ìë¦¬ì¼ ëª¨ëí°ë¥¼ ìí ì´ê¸°í
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // ìë¦¬ì¼2 ì´ê¸°í
  
  Serial.println("ìë¦¬ì¼2 ë¬´ì  íµì  ìì");
}

void loop() {
  if (Serial2.available()) { // ìë¦¬ì¼2ë¡ë¶í° ë°ì´í°ê° ìì ëë©´
    char receivedChar = Serial2.read(); // ìì ë ë°ì´í°ë¥¼ ì½ì
    Serial.print("ìì ë ë°ì´í°: ");
    Serial.println(receivedChar); // ìë¦¬ì¼ ëª¨ëí°ì ìì ë ë°ì´í° ì¶ë ¥
  }
  
  if (Serial.available()) { // ìë¦¬ì¼ ëª¨ëí°ë¡ë¶í° ë°ì´í°ê° ìë ¥ëë©´
    char sendData = Serial.read(); // ìë ¥ë ë°ì´í°ë¥¼ ì½ì
    Serial2.write(sendData); // ìë¦¬ì¼2ë¡ ë°ì´í° ì ì¡
  }
}