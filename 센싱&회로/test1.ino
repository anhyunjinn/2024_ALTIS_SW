//print with BMI270, BMP390

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include "SparkFun_BMI270_Arduino_Library.h"

//BMP390와 esp32 연결 핀 지정 (INT)
int BMP_INT = 2;

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;

//BMI270
BMI270 imu;
#define IMU_SCL 16
#define IMU_SDA 17

int interruptPin = 3;

volatile boolean dataReady = false;

uint8_t i2cAddress = BMI2_I2C_PRIM_ADDR; // 0x68

void handleInterrupt() {
  dataReady = true;
}

bool BMP_INI(TwoWire *wire, int INT){
  wire->begin();
  wire->setClock(1000000); // 1MHz 최대hz

  if (!bmp.begin_I2C(0x77)){
    Serial.println("연결 실패: BMP390");
    return false;
  }

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

  // 인터럽트 핀 설정
  pinMode(INT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BMP_INT), handleInterrupt, CHANGE);

  Serial.println("BMP390 센서 초기화 완료");
  return true;
}

bool IMU_INI(TwoWire *wire, int scl, int sda)//bmi270 초기화 및 설정
{
  wire->begin();
  wire->setSCL(scl);
  wire->setSDA(sda);
  wire->setClock(1000000); // 1MHz 최대hz
  if (imu.beginI2C(i2cAddress, *wire) != BMI2_OK)
  {
    Serial.println("연결 실패: BMI270");
    return false;
  }

    // 가속도 전원모드
    /*
    ///     BMI2_POWER_OPT_MODE 0.78Hz to 400Hz
    ///     BMI2_PERF_OPT_MODE 12.5Hz to 1600Hz
    */
  imu.setAccelPowerMode(BMI2_PERF_OPT_MODE);
  // 가속도 주기
  /*
/     BMI2_ACC_ODR_0_78HZ
/     BMI2_ACC_ODR_1_56HZ
/     BMI2_ACC_ODR_3_12HZ
/     BMI2_ACC_ODR_6_25HZ
/     BMI2_ACC_ODR_12_5HZ
/     BMI2_ACC_ODR_25HZ
/     BMI2_ACC_ODR_50HZ
/     BMI2_ACC_ODR_100HZ (default)
/     BMI2_ACC_ODR_200HZ
/     BMI2_ACC_ODR_400HZ
/     BMI2_ACC_ODR_800HZ
/     BMI2_ACC_ODR_1600HZ
  */

  imu.setAccelODR(BMI2_ACC_ODR_1600HZ);
  // 가속도 필터 설정
  /*
  /     BMI2_ACC_OSR4_AVG1// Oversampling Ratio
  /     BMI2_ACC_OSR2_AVG2
  /     BMI2_ACC_NORMAL_AVG4 (default)
  /     BMI2_ACC_CIC_AVG8//Cascaded Integrator-Comb
  /     BMI2_ACC_RES_AVG16//Resolution Enhancement
  /     BMI2_ACC_RES_AVG32
  /     BMI2_ACC_RES_AVG64
  /     BMI2_ACC_RES_AVG128
  */

  imu.setAccelFilterBandwidth(BMI2_ACC_NORMAL_AVG4);

  // 자이로 전원모드
  /*
  /     BMI2_POWER_OPT_MODE
  /     BMI2_PERF_OPT_MODE (default)

  /     BMI2_POWER_OPT_MODE
  /     BMI2_PERF_OPT_MODE (default)
  */
  imu.setGyroPowerMode(BMI2_PERF_OPT_MODE, BMI2_PERF_OPT_MODE);
  // 자이로 주기 설정
  /*
  /     BMI2_GYR_ODR_25HZ
  /     BMI2_GYR_ODR_50HZ
  /     BMI2_GYR_ODR_100HZ
  /     BMI2_GYR_ODR_200HZ (default)
  /     BMI2_GYR_ODR_400HZ
  /     BMI2_GYR_ODR_800HZ
  /     BMI2_GYR_ODR_1600HZ
  /     BMI2_GYR_ODR_3200HZ
  */
  imu.setGyroODR(BMI2_GYR_ODR_3200HZ);
  // 자이로 필터 설정
  /*
  /     BMI2_GYR_OSR4_MODE // Oversampling Ratio 4
  /     BMI2_GYR_OSR2_MODE // Oversampling Ratio 2 3200/2 Hz

  /     BMI2_GYR_NORMAL_MODE (default) //오버샘플링 없이 센서 데이터를 그대로 출력합니다.
  /     BMI2_GYR_CIC_MODE // Cascaded Integrator-Comb
  */

  imu.setGyroFilterBandwidth(BMI2_GYR_OSR2_MODE);

  // 인터럽트 핀 설정
  /*
  /     BMI2_FFULL_INT
  /     BMI2_FWM_INT
  /     BMI2_DRDY_INT
  /     BMI2_ERR_INT
  /     BMI2_SIG_MOTION_INT
  /     BMI2_WRIST_GESTURE_INT
  /     BMI2_ANY_MOTION_INT
  /     BMI2_NO_MOTION_INT
  /     BMI2_STEP_COUNTER_INT
  /     BMI2_STEP_DETECTOR_INT
  /     BMI2_STEP_ACTIVITY_INT
  /     BMI2_WRIST_WEAR_WAKE_UP_INT
  */
  imu.mapInterruptToPin(BMI2_DRDY_INT, BMI2_INT1); // DRDY: Data Ready

  bmi2_int_pin_config intPinConfig;
  intPinConfig.pin_type = BMI2_INT1;
  intPinConfig.int_latch = BMI2_INT_NON_LATCH;
  intPinConfig.pin_cfg[0].lvl = BMI2_INT_ACTIVE_HIGH;
  intPinConfig.pin_cfg[0].od = BMI2_INT_PUSH_PULL;
  intPinConfig.pin_cfg[0].output_en = BMI2_INT_OUTPUT_ENABLE;
  intPinConfig.pin_cfg[0].input_en = BMI2_INT_INPUT_DISABLE;
  imu.setInterruptPinConfig(intPinConfig);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, RISING);
  return true;
}

void BMP390_(){
  if (dataReady){
    dataReady = false;

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

void IMU_()
{
  if (dataReady)
  {
    // Reset flag for next interrupt
    dataReady = false;

    Serial.print("Interrupt occurred!");
    Serial.print("\t");

    // Get the interrupt status to know which condition triggered
    uint16_t interruptStatus = 0;
    imu.getInterruptStatus(&interruptStatus); // 인터럽트 정보 저장 (자이로가 준비 되었는지, 가속도가 준비 되었는지 확인용)

    if (interruptStatus & BMI2_GYR_DRDY_INT_MASK) // 비트 연산자 &를 이용하여 자이로 데이터 준비 인터럽트인지 확인
    {
      Serial.print("Gyro data ready!");
      Serial.print("\t");
      imu.getSensorData();
      Serial.print("X: ");
      Serial.print(imu.data.gyroX, 3);
      Serial.print("\t");
      Serial.print("Y: ");
      Serial.print(imu.data.gyroY, 3);
      Serial.print("\t");
      Serial.print("Z: ");
      Serial.print(imu.data.gyroZ, 3);
      Serial.print("\t");
    }
    if (interruptStatus & BMI2_ACC_DRDY_INT_MASK) // 비트 연산자 &를 이용하여 가속도 데이터 준비 인터럽트인지 확인
    {
      Serial.print("Accelerometer data ready!");
      Serial.print("\t");
      imu.getSensorData();
      Serial.print("X: ");
      Serial.print(imu.data.accelX, 3);
      Serial.print("\t");
      Serial.print("Y: ");
      Serial.print(imu.data.accelY, 3);
      Serial.print("\t");
      Serial.print("Z: ");
      Serial.print(imu.data.accelZ, 3);
    }
    if (!(interruptStatus & (BMI2_GYR_DRDY_INT_MASK | BMI2_ACC_DRDY_INT_MASK))) // 예외처리
    {
      Serial.print("Unknown interrupt condition!");
    }
    Serial.println();

    delay(2000);

  }
}

void setup() {
  Serial.begin(115200);

  if (!BMP_INI(&Wire, BMP_INT)) {
    Serial.println("BMP390 초기화 실패");
  }

  if (!IMU_INI(&Wire1, IMU_SCL, IMU_SDA)) {
    Serial.println("BMI270 초기화 실패");
  }
}

void loop() {
  BMP390_();
  IMU_();
}
