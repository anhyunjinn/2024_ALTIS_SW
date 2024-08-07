#ifndef pinmap_H
#define pinmap_H
//IMU imu(&Wire1, 1000000, 3, 16, 17); // i2c객체 , 클럭속도, 인터럽트 핀 scl, sda
#define IMU_i2c &Wire1
#define IMU_clock 1000000
#define IMU_int 3
#define IMU_scl 16
#define IMU_sda 17

//BM bm(&Wire, 1000000, 2, 19, 18);
#define BM_i2c &Wire
#define BM_clock 1000000
#define BM_int 2
#define BM_scl 19
#define BM_sda 18

// gps
//GPS gps(Serial3, 9600); // GPS 객체 생성
#define GPS_Serial Serial3
#define GPS_Baudrate 9600

// EJ
//EJ ej(21, 5 * 1000, 1007.8); // pwm핀,타이머,해면기압
#define EJ_motorpin 21 //시범발사핀
#define EJ_timer 9.0//(s)
#define EJ_SeaPreasure 1007.4
#define Alt_Alpha 0.9

// data
//DATA data(Serial2, 9, 115200, SPI, 10); // DATA(HardwareSerial& serial,int aux, int baudrate,SPIClass& spi,int cs)
#define DATA_Serial Serial2
#define DATA_Aux 9
#define DATA_Baudrate 115200
#define DATA_SPI SPI
#define DATA_CS 10

//interface
#define buzzer_pin 4
#define led1_pin 22
#define led2_pin 23

#endif