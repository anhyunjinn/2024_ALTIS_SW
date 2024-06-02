#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BMI270_Arduino_Library.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

/*BMI270,buzzer,led 김승창*/
bool set_imu(TwoWire *wire = &Wire1,int scl=16, int sda=17 ){//인수로 i2c 객체를 넣어서 초기화, bmi 설정값을 인수로 넣어도 댐,
  /*
  i2c 설정 : 클럭속도 bmi270 최대 클럭속도
  wire->begin();
  wire->setSCL(scl);
  wire->setSDA(sda);
  wire->setClock(1000000); // 1MHz 최대hz

  bmi 설정 : 전원모드, 주기, 필터, 인터럽트 .... 
  bmi 캘리브레이션 : 초기 각도 측정, 자이로 및 가속도 캘리브레이션
  return 값 : bmi 상태 반환 
  */
}
void read_imu(bool interrupt_bmi270 ){
  /*
  인터럽트 값에 따른 자이로 가속도 데이터 구조체 저장
  1. 통신 구조체에 저장
  2. 가속도 자이로 값 구조체 반환해도 댐
  */
}

void read_condition(int condition){
  /*
  인수값에 따른 버저 및 led
  1.imu 에러
  2.bmp 에러
  3.
  4.
  */
}
void set_interrupt(){ //인터럽트
  dataReady = true;
  return true;
}


/*BMP390 문기준*/
bool set_bmp(TwoWire *wire = &Wire,int INT=2){//인수로 i2c 객체를 넣어서 초기화, bmp 설정값을 인수로 넣어도 댐
  wire->begin();
  wire->setClock(1000000); // 1MHz 최대hz

  if (!bmp.begin_I2C()){
    Serial.println("연결 실패: BMP390");
    return read_condition(2); //bmp 에러
  }
  /*
  i2c 설정 : 클럭속도 bmp390 최대 클럭속도
  wire->begin();
  wire->setSCL(scl);
  wire->setSDA(sda);
  wire->setClock(1000000); // 1MHz 최대hz

  bmp 설정 : 전원모드,샘플링, 필터, 인터럽트 .... 
  return 값 : bmp 상태 반환 
  */

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    /*BMP3_OVERSAMPLING_2X, 
    BMP3_OVERSAMPLING_4X, 
    BMP3_OVERSAMPLING_8X,
    BMP3_OVERSAMPLING_16X, 
    BMP3_OVERSAMPLING_32X */

  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    /*BMP3_OVERSAMPLING_2X, 
    BMP3_OVERSAMPLING_4X, 
    BMP3_OVERSAMPLING_8X,
    BMP3_OVERSAMPLING_16X, 
    BMP3_OVERSAMPLING_32X*/

  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    /*BMP3_IIR_FILTER_DISABLE (no filtering), 
    BMP3_IIR_FILTER_COEFF_1,
    BMP3_IIR_FILTER_COEFF_3, 
    BMP3_IIR_FILTER_COEFF_7, 
    BMP3_IIR_FILTER_COEFF_15,
    BMP3_IIR_FILTER_COEFF_31, 
    BMP3_IIR_FILTER_COEFF_63, 
    BMP3_IIR_FILTER_COEFF_127*/

  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  /*BMP3_ODR_200_HZ, 
    BMP3_ODR_100_HZ,
    BMP3_ODR_50_HZ, 
    BMP3_ODR_25_HZ, 
    BMP3_ODR_12_5_HZ, 
    BMP3_ODR_6_25_HZ,
    BMP3_ODR_3_1_HZ, 
    BMP3_ODR_1_5_HZ, 
    BMP3_ODR_0_78_HZ, 
    BMP3_ODR_0_39_HZ,
    BMP3_ODR_0_2_HZ, 
    BMP3_ODR_0_1_HZ, 
    BMP3_ODR_0_05_HZ, 
    BMP3_ODR_0_02_HZ,
    BMP3_ODR_0_01_HZ, 
    BMP3_ODR_0_006_HZ, 
    BMP3_ODR_0_003_HZ, or BMP3_ODR_0_001_HZ*/
    
  pinMode(INT, INPUT_PULLUP);
  if(!attachInterrupt(digitalPinToInterrupt(INT), handleInterrupt, CHANGE)){
    return read_codition(2); //bmp 에러
  }
    
}
void read_bmp(bool interrupt_bmp390 ){
  /*
  인터럽트 값에 따른 고도 데이터 구조체 저장
  1. 통신 구조체에 저장
  2. 고도값 반환해도 댐
  압력 도 저장하고 싶으면 추가해도 댐
  */

  if (dataReady){
    dataReady = false;
    /*구조체 추가되고 나면 아래부분 수정 예정*/

    Serial.print("Temperature = ");
    Serial.print(bmp.temperature);
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bmp.pressure / 100.0);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.println();

    delay(2000);

  }
}

/*lora 노은지*/
/*
구조체 구조 선언(
  시간(millis),가속도(float),자이로(float),고도(float),사출조건(0은 사출x, 1은 각도, 2는 고도, 3은 타이머, 4는 강제사출..),각도(float),경도(float),위도(float)....
  )
*/

bool set_lora(HardwareSerial& serial = Serial2 ,int baudrate = 9600){
  /*
  시리얼 설정
  serial.begin(baudrate);
  aux핀 설정
  pinMode(lora_aux 핀, INPUT);
  
  통신 test 코드
  예시)
  지상국에서 신호를 계속 보내고 신호를 받으면
  지상국에 통신을 받았다고 답변을 보내서 통신 테스트
  */
}
void update_lora(bool lora_aux){
  /*
  lora aux 핀의 신호에 따라 데이터를 보낼 준비가 되었는지 확인
  확인후 구조체 통신
  */
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
void set_sd(){
  /*
  sd카드를 사용하기 위한 세팅
  */
}

void write_sd(){
  /*
  데이터 구조체를 sd카드에 저장
  */
}

void print_data(){
  /*
  시리얼창에 데이터 출력
  */
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
}