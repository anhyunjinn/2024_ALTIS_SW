#include "IMU.h"

IMU::IMU(TwoWire* wire, int clock, int interruptPin, int scl, int sda)
    : _Wire(*wire),
      _interruptPin(interruptPin),
      _clock(clock),
      _scl(scl),
      _sda(sda) {}

bool IMU::set()
{
  _Wire.begin();
  _Wire.setClock(_clock);
  _Wire.setSCL(_scl);
  _Wire.setSDA(_sda);
  if (_imu.beginI2C(i2cAddress, _Wire) != BMI2_OK)
  {
    return false;
  }

  /*가속도 전원모드
  ///     BMI2_POWER_OPT_MODE 0.78Hz to 400Hz
  ///     BMI2_PERF_OPT_MODE 12.5Hz to 1600Hz
  */
  _imu.setAccelPowerMode(BMI2_PERF_OPT_MODE);
  /*가속도 주기
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
  _imu.setAccelODR(BMI2_ACC_ODR_1600HZ);
  /*가속도 필터 설정
  /     BMI2_ACC_OSR4_AVG1// Oversampling Ratio
  /     BMI2_ACC_OSR2_AVG2
  /     BMI2_ACC_NORMAL_AVG4 (default)
  /     BMI2_ACC_CIC_AVG8//Cascaded Integrator-Comb
  /     BMI2_ACC_RES_AVG16//Resolution Enhancement
  /     BMI2_ACC_RES_AVG32
  /     BMI2_ACC_RES_AVG64
  /     BMI2_ACC_RES_AVG128
  */
  _imu.setAccelFilterBandwidth(BMI2_ACC_NORMAL_AVG4);

  /*자이로 전원모드
  /     BMI2_POWER_OPT_MODE
  /     BMI2_PERF_OPT_MODE (default)

  /     BMI2_POWER_OPT_MODE
  /     BMI2_PERF_OPT_MODE (default)
  */
  _imu.setGyroPowerMode(BMI2_PERF_OPT_MODE, BMI2_PERF_OPT_MODE);
  /*자이로 주기 설정
  /     BMI2_GYR_ODR_25HZ
  /     BMI2_GYR_ODR_50HZ
  /     BMI2_GYR_ODR_100HZ
  /     BMI2_GYR_ODR_200HZ (default)
  /     BMI2_GYR_ODR_400HZ
  /     BMI2_GYR_ODR_800HZ
  /     BMI2_GYR_ODR_1600HZ
  /     BMI2_GYR_ODR_3200HZ
  */
  _imu.setGyroODR(BMI2_GYR_ODR_3200HZ);
  /*자이로 필터 설정
  /     BMI2_GYR_OSR4_MODE // Oversampling Ratio 4
  /     BMI2_GYR_OSR2_MODE // Oversampling Ratio 2 3200/2 Hz

  /     BMI2_GYR_NORMAL_MODE (default) //오버샘플링 없이 센서 데이터를 그대로 출력합니다.
  /     BMI2_GYR_CIC_MODE // Cascaded Integrator-Comb
  */
  _imu.setGyroFilterBandwidth(BMI2_GYR_OSR2_MODE);

  /*인터럽트 핀 설정
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
  _imu.mapInterruptToPin(BMI2_DRDY_INT, BMI2_INT1); // DRDY: Data Ready

  bmi2_int_pin_config intPinConfig;
  intPinConfig.pin_type = BMI2_INT1;
  intPinConfig.int_latch = BMI2_INT_NON_LATCH;
  intPinConfig.pin_cfg[0].lvl = BMI2_INT_ACTIVE_HIGH;
  intPinConfig.pin_cfg[0].od = BMI2_INT_PUSH_PULL;
  intPinConfig.pin_cfg[0].output_en = BMI2_INT_OUTPUT_ENABLE;
  intPinConfig.pin_cfg[0].input_en = BMI2_INT_INPUT_DISABLE;
  _imu.setInterruptPinConfig(intPinConfig);
  return true;
}

bool IMU::ready()
{
  if(_interruptOccurred == false){
    return false;
  }
  _interruptOccurred = false; // 플래그 초기화
  return true;
}

void IMU::read(float* ax,float* ay,float* az,float* gx,float* gy,float* gz)
{
  uint16_t interruptStatus = 0;
    _imu.getInterruptStatus(&interruptStatus); // 인터럽트 정보 저장 (자이로가 준비 되었는지, 가속도가 준비 되었는지 확인용)
    if (interruptStatus & BMI2_GYR_DRDY_INT_MASK) // 비트 연산자 &를 이용하여 자이로 데이터 준비 인터럽트인지 확인
    {
      _imu.getSensorData();
      *gx = _imu.data.gyroX;
      *gy = _imu.data.gyroY;
      *gz = _imu.data.gyroZ;
    }
    if (interruptStatus & BMI2_ACC_DRDY_INT_MASK) // 비트 연산자 &를 이용하여 가속도 데이터 준비 인터럽트인지 확인
    {
      _imu.getSensorData();
      *ax =_imu.data.accelX;
      *ay =_imu.data.accelY;
      *az =_imu.data.accelZ;
    }
}


void IMU::handler()
{
  _interruptOccurred = true;
}