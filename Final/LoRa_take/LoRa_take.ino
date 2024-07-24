#define RXD2 16
#define TXD2 17

struct LORA {
  unsigned long t;

  float ax;
  float ay;
  float az;

  float angleX;
  float angleY;
  float angleZ;

  float a;

  byte ej;
};

// 데이터 패킷을 구조체로 변환하는 함수
bool parseDataPacket(const char* buffer, int bufferSize, LORA& data) {
  if (bufferSize < sizeof(data) + 2 || buffer[0] != '#' || buffer[bufferSize - 1] != '@') {
    return false;  // 패킷의 크기가 맞지 않거나 시작/끝 표시가 잘못된 경우
  }

  // 데이터 구조체로 변환
  memcpy(&data, &buffer[1], sizeof(data));
  return true;
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  // PC로부터의 데이터 수신 및 Serial2로 송신
  if (Serial.available() > 0) {
    char c = Serial.read();
    Serial.print(c);
    Serial2.write(c);
  }

  // Serial2로부터의 데이터 수신 및 처리
  if (Serial2.available()) {
    // 데이터 버퍼 초기화
    char tDataBuf[40] = { 0 };
    int bufIndex = 0;

    // 데이터 수신
    while (Serial2.available()) {
      char c = Serial2.read();
      tDataBuf[bufIndex++] = c;

      // 버퍼가 가득 찼거나 패킷 끝 표시를 만난 경우
      if (bufIndex >= sizeof(tDataBuf) || c == '@') {
        break;
      }
    }

    // 수신된 데이터가 패킷인지 확인하고 구조체로 변환
    LORA data;
    if (parseDataPacket(tDataBuf, bufIndex, data)) {
      // 수신된 데이터를 처리 (예: 출력)
      Serial.print(data.t);
      Serial.print(",");
      Serial.print(data.ax);
      Serial.print(",");
      Serial.print(data.ay);
      Serial.print(",");
      Serial.print(data.az);
      Serial.print(", ");
      Serial.print(data.angleX);
      Serial.print(",");
      Serial.print(data.angleY);
      Serial.print(",");
      Serial.print(data.angleZ);
      Serial.print(",");
      Serial.print(data.a);
      Serial.print(",");
      Serial.println(data.ej);
    } else {
      // 수신된 데이터가 유효하지 않은 경우
      Serial.println("Invalid data packet received.");
    }
  }
}
