#include <SD.h>
#include <SPI.h>

File file;

// êµ¬ì¡°ì²´ ì ì
struct Data {
  int value;
};

Data myData;

void setup() {
  Serial.begin(115200);

  if (!SD.begin(5)) {
    Serial.println("SD ì¹´ë ì´ê¸°í ì¤í¨");
    return;
  }
  Serial.println("SD ì¹´ë ì´ê¸°í ì±ê³µ");

  // íì¼ ì´ê¸° (ê¸°ì¡´ íì¼ì ì´ ë ì¬ì©)
  file = SD.open("/data.txt", FILE_WRITE);
  if (file) {
    Serial.println("data.txt íì¼ì ì´ììµëë¤.");
    file.close();
  } else {
    Serial.println("data.txt íì¼ì ì´ ì ììµëë¤.");
  }

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; i++) {
      // ë¬´ìì ì«ì ìì± ë° ì ì¥
      myData.value = random(1, 101);  // 1ìì 100 ì¬ì´ì ë¬´ìì ì«ì ìì±

      // íì¼ ì´ê¸° (ìë¡ì´ íì¼ì ìì±í  ë ì¬ì©)
      file = SD.open("/data.txt", FILE_APPEND);  // FILE_APPEND ìµìì ì¬ì©íì¬ íì¼ ëì ë°ì´í°ë¥¼ ì¶ê°í©ëë¤.
      if (file) {
        // ë°ì´í°ë¥¼ íì¼ì ìëë¤. '/'ë¡ ë°ì´í°ë¥¼ êµ¬ë¶í©ëë¤.
        file.print(myData.value);
        file.print('/');
        file.close();
        Serial.println("ë°ì´í°ë¥¼ íì¼ì ì ì¥íìµëë¤.");
      } else {
        Serial.println("data.txt íì¼ì ì´ ì ììµëë¤.");
      }
    }
    file.print('\n');
  }


  // íì¼ ì½ê¸°
  file = SD.open("/data.txt");
  if (file) {
    Serial.println("data.txt íì¼ ë´ì©:");
    while (file.available()) {
      char c = file.read();
      Serial.print(c);
    }
    file.close();
  } else {
    Serial.println("data.txt íì¼ì ì´ ì ììµëë¤.");
  }
}

void loop() {
  // ì¬ê¸°ì ì¶ê° ììì ìíí  ì ììµëë¤.
}
