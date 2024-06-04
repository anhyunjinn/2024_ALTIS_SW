#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BMI270_Arduino_Library.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <SoftwareSerial.h>

/*lora 노은지*/
/*
구조체 구조 선언(
  시간(millis),가속도(float),자이로(float),고도(float),사출조건(0은 사출x, 1은 각도, 2는 고도, 3은 타이머, 4는 강제사출..),각도(float),경도(float),위도(float)....
  )
*/


// LoRa 모듈 핀 정의
SoftwareSerial lora(7, 8); // RX, TX
const int lora_aux = 9; // AUX 핀

// 데이터 구조체 정의
struct MyData {
  float millis;
  float ax;
  float ay;
  float az;
  float z;
  float alt;
  float lat;
  float lon;
  int h;
  int a;
} mydata;

byte send_buff[3];
byte recv_buff[4];
byte myheader[] = {0x00, 0x02, 0x47};

// 설정 값 읽기 함수
byte get_config(byte input[]) {
  byte recv_buff[4];
  lora.write(input, 3);
  lora.readBytes(recv_buff, 4);

  if (recv_buff[0] == input[0] && recv_buff[1] == input[1] && recv_buff[2] == input[2]) {
    return recv_buff[3];
  } else {
    return 0xFF;
  }
}

// LoRa 설정 및 통신 테스트 함수
bool set_lora(HardwareSerial& serial = Serial, int baudrate = 115200) {
  /*
  시리얼 설정
  */
  serial.begin(baudrate);
  lora.begin(9600); // LoRa 모듈의 통신 속도 설정

  pinMode(lora_aux, INPUT);

  /*
  통신 테스트 코드
  */
  Serial.println("E220-900T30D의 설정값입니다");

  Serial.print("ADDH=");
  send_buff[0] = 0xC1;
  send_buff[1] = 0x00;
  send_buff[2] = 0x01;
  Serial.println(get_config(send_buff), HEX);

  delay(100);
  Serial.print("ADDL=");
  send_buff[0] = 0xC1;
  send_buff[1] = 0x01;
  send_buff[2] = 0x01;
  Serial.println(get_config(send_buff), HEX);

  delay(100);
  Serial.print("CH=");
  send_buff[0] = 0xC1;
  send_buff[1] = 0x04;
  send_buff[2] = 0x01;
  Serial.print(850.125 + get_config(send_buff));
  Serial.println("MHz");

  delay(100);
  Serial.print("mode=");
  send_buff[0] = 0xC1;
  send_buff[1] = 0x05;
  send_buff[2] = 0x01;
  Serial.println(bitRead(get_config(send_buff), 6) ? "Fixed Transmission" : "Transparent Transmission");

  delay(1000);
  Serial.println("송수신모드로 전환합니다");
  delay(100);

  return true; // 설정 완료

  // 송신 함수
  void send_data() {
    // 구조체 데이터 송신
    mydata.millis = millis();
    mydata.ax = 1.23;
    mydata.ay = 4.56;
    mydata.az = 7.89;
    mydata.z = 0.12;
    mydata.alt = 345.67;
    mydata.lat = 12.345678;
    mydata.lon = 98.765432;
    mydata.h = 123;
    mydata.a = 456;

    lora.write(myheader, 3);
    lora.write((byte*)&mydata, sizeof(mydata));
    delay(2000);
  }

  // 수신 함수
  void receive_data() {
    if (lora.available() >= sizeof(mydata)) {
      lora.readBytes((byte*)&mydata, sizeof(mydata)); // 수신된 데이터를 구조체 형태로 변환
      Serial.print("Received data: ");
      Serial.print(" millis: "); Serial.print(mydata.millis);
      Serial.print(" ax: "); Serial.print(mydata.ax);
      Serial.print(" ay: "); Serial.print(mydata.ay);
      Serial.print(" az: "); Serial.print(mydata.az);
      Serial.print(" z: "); Serial.print(mydata.z);
      Serial.print(" alt: "); Serial.print(mydata.alt);
      Serial.print(" lat: "); Serial.print(mydata.lat);
      Serial.print(" lon: "); Serial.print(mydata.lon);
      Serial.print(" h: "); Serial.print(mydata.h);
      Serial.print(" a: "); Serial.println(mydata.a);
    }
  }
}//write_sd 함수를 호출하여 sd카드 저장, 시도해보기


void update_lora(bool lora_aux) {
  /*
  lora aux 핀의 신호에 따라 데이터를 보낼 준비가 되었는지 확인
  확인 후 구조체 통신
  */
  if (digitalRead(lora_aux) == HIGH) {
    Serial.println("준비완료");

    // 구조체 데이터 송신
    mydata.millis = millis();
    mydata.ax = 1.23;
    mydata.ay = 4.56;
    mydata.az = 7.89;
    mydata.z = 0.12;
    mydata.alt = 345.67;
    mydata.lat = 12.345678;
    mydata.lon = 98.765432;
    mydata.h = 123;
    mydata.a = 456;

    lora.write(myheader, 3);
    lora.write((byte*)&mydata, sizeof(mydata));

    // SD 카드에 데이터 저장
    write_sd();
  } else {
    Serial.println("준비 중");
  }

  // 수신된 데이터를 시리얼 창에 출력
  print_data();
}

int get_lora(){
/*
지상국에서 신호를 보냈을때(if문)
보내는 신호에 따라 어떤 기능을 할지 
1. 강제 사출 : 
2. 준비 모드 : 준비모드는 발사대에 로켓을 장착하고나서 발동(각도 추정은 시간이 갈수록 정확성이 떨어짐)
3.
}
*/
}

void set_sd() {
  /*
  SD 카드를 사용하기 위한 세팅
  */
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1) {
      // No SD card, so don't do anything more - stay stuck here
    }
  } else {
    Serial.println("SD 카드 초기화 완료.");
  }
}
void write_sd(){
  /*
  데이터 구조체를 sd카드에 저장
  */
  myFile = SD.open("data.txt", FILE_WRITE);  // data.txt 파일을 쓰기 모드로 열기

  if (myFile) {   // 파일이 열리면
    myFile.print("Millis: "); myFile.println(mydata.millis);
    myFile.print("AX: "); myFile.println(mydata.ax);
    myFile.print("AY: "); myFile.println(mydata.ay);
    myFile.print("AZ: "); myFile.println(mydata.az);
    myFile.print("Z: "); myFile.println(mydata.z);
    myFile.print("Alt: "); myFile.println(mydata.alt);
    myFile.print("Lat: "); myFile.println(mydata.lat);
    myFile.print("Lon: "); myFile.println(mydata.lon);
    myFile.print("H: "); myFile.println(mydata.h);
    myFile.print("A: "); myFile.println(mydata.a);
    myFile.println("-----------------------------------------");
    myFile.println();
    myFile.println(); // 데이터를 구분하기 위해 빈 줄 추가

    myFile.close();   // 파일 닫기
    Serial.println("데이터 저장 완료.");
  } else {
    // 파일을 여는데 실패하면 에러 메시지 출력
    Serial.println("error opening data.txt");
  }
}

void print_data(){
  /*
  시리얼창에 데이터 출력
  */
  if (lora.available() >= sizeof(mydata)) { 
    lora.readBytes((byte*)&mydata, sizeof(mydata)); // 수신된 데이터를 구조체 형태로 변환
    Serial.print("Received data: ");
    Serial.print(" millis: "); Serial.print(mydata.millis);
    Serial.print(" ax: "); Serial.print(mydata.ax);
    Serial.print(" ay: "); Serial.print(mydata.ay);
    Serial.print(" az: "); Serial.print(mydata.az);
    Serial.print(" z: "); Serial.print(mydata.z);
    Serial.print(" alt: "); Serial.print(mydata.alt);
    Serial.print(" lat: "); Serial.print(mydata.lat);
    Serial.print(" lon: "); Serial.print(mydata.lon);
    Serial.print(" h: "); Serial.print(mydata.h);
    Serial.print(" a: "); Serial.println(mydata.a);
  }
}


void setup(){
  /*
  while(!set_lora()){
    read_condition(1)
  }
  while(!set_imu()){
    read_condition(2)
  }
  while(!set_bmp()){
    read_condition(3)
  }
  while(set_sd()){
    read_condition(4)
  }
  read_condition(0) // 정상작동 모드
  */
   Serial.begin(115200);
  while (!Serial);
  set_sd();
  set_lora(); //lora설정
}

void loop(){
  /*
  static int state = 0;
  if(int command = get_lora()>0){
    state = command;
  }

  if(state == 1){
    //구조체.사출조건 = 1
  }
  if(state == 2){
    //준비모드
    read_imu,read_bmp,update_lora,write_sd,print_data
  }
  */
   update_lora();
   delay(1000); // 주기적으로 업데이트
}
