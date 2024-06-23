#include "BM.h"
// #define debug
BM::BM(TwoWire *wire, int clock, int interruptPin, int scl, int sda)
  : _Wire(*wire),
    _interruptPin(interruptPin),
    _clock(clock),
    _scl(scl),
    _sda(sda)
{
}

bool BM::set()
{
  _Wire.begin();
  _Wire.setClock(_clock);
  // _Wire.setSCL(_scl);
  // _Wire.setSDA(_sda);

  if (pressureSensor.beginI2C(BMP384_I2C_ADDRESS_DEFAULT) != BMP3_OK)
  {
    return false;
  }
  // 데이터주기 설정
  /*
    #define BMP3_ODR_200_HZ                         UINT8_C(0x00)
    #define BMP3_ODR_100_HZ                         UINT8_C(0x01)
    #define BMP3_ODR_50_HZ                          UINT8_C(0x02)
    #define BMP3_ODR_25_HZ                          UINT8_C(0x03)
    #define BMP3_ODR_12_5_HZ                        UINT8_C(0x04)
    #define BMP3_ODR_6_25_HZ                        UINT8_C(0x05)
    #define BMP3_ODR_3_1_HZ                         UINT8_C(0x06)
    #define BMP3_ODR_1_5_HZ                         UINT8_C(0x07)
    #define BMP3_ODR_0_78_HZ                        UINT8_C(0x08)
    #define BMP3_ODR_0_39_HZ                        UINT8_C(0x09)
    #define BMP3_ODR_0_2_HZ                         UINT8_C(0x0A)
    #define BMP3_ODR_0_1_HZ                         UINT8_C(0x0B)
    #define BMP3_ODR_0_05_HZ                        UINT8_C(0x0C)
    #define BMP3_ODR_0_02_HZ                        UINT8_C(0x0D)
    #define BMP3_ODR_0_01_HZ                        UINT8_C(0x0E)
    #define BMP3_ODR_0_006_HZ                       UINT8_C(0x0F)
    #define BMP3_ODR_0_003_HZ                       UINT8_C(0x10)
    #define BMP3_ODR_0_001_HZ                       UINT8_C(0x11)

  */
  //  Variable to track errors returned by API calls
  int8_t err = BMP3_OK;
  // ODR settings, see bmp3_defs.h (line 232-249)
  err = pressureSensor.setODRFrequency(BMP3_ODR_200_HZ);
  if (err != BMP3_OK)
  {
    return false;
  }
  // IIR필터 설정
  /*
    #define BMP3_IIR_FILTER_DISABLE                 UINT8_C(0x00)
    #define BMP3_IIR_FILTER_COEFF_1                 UINT8_C(0x01)
    #define BMP3_IIR_FILTER_COEFF_3                 UINT8_C(0x02)
    #define BMP3_IIR_FILTER_COEFF_7                 UINT8_C(0x03)
    #define BMP3_IIR_FILTER_COEFF_15                UINT8_C(0x04)
    #define BMP3_IIR_FILTER_COEFF_31                UINT8_C(0x05)
    #define BMP3_IIR_FILTER_COEFF_63                UINT8_C(0x06)
    #define BMP3_IIR_FILTER_COEFF_127               UINT8_C(0x07)
  */
  err = pressureSensor.setFilterCoefficient(BMP3_IIR_FILTER_DISABLE);
  if (err)
  {
    return false;
  }
  // 오버샘플링 설정
  /*
    #define BMP3_NO_OVERSAMPLING                    UINT8_C(0x00)
    #define BMP3_OVERSAMPLING_2X                    UINT8_C(0x01)
    #define BMP3_OVERSAMPLING_4X                    UINT8_C(0x02)
    #define BMP3_OVERSAMPLING_8X                    UINT8_C(0x03)
    #define BMP3_OVERSAMPLING_16X                   UINT8_C(0x04)
    #define BMP3_OVERSAMPLING_32X                   UINT8_C(0x05)
  */
  bmp3_odr_filter_settings osrMultipliers =
  {
    .press_os = BMP3_NO_OVERSAMPLING,
    .temp_os = BMP3_NO_OVERSAMPLING,
    0,
    0 // Unused values, included to avoid compiler warnings-as-error
  };

  err = pressureSensor.setOSRMultipliers(osrMultipliers);
  if (err)
  {
    return false;
  }

  // 인터럽트 설정
  //  Configure the BMP384 to trigger interrupts whenever a measurement is performed
  bmp3_int_ctrl_settings interruptSettings =
  {
    .output_mode = BMP3_INT_PIN_PUSH_PULL, // Push-pull or open-drain
    .level = BMP3_INT_PIN_ACTIVE_HIGH,     // Active low or high
    .latch = BMP3_INT_PIN_NON_LATCH,       // Latch or non-latch
    .drdy_en = BMP3_ENABLE                 // Trigger interrupts when data is ready
  };
  err = pressureSensor.setInterruptSettings(interruptSettings);
  if (err != BMP3_OK)
  {
    return false;
  }
  _interruptOccurred = false;
  return true;
}

void BM::handler()
{
  _interruptOccurred = true;
}

bool BM::ready()
{
  if (_interruptOccurred)
  {
    // Reset flag for next interrupt
    _interruptOccurred = false;
    // Variable to track errors returned by API calls
    int8_t err = BMP3_OK;
    // Get the interrupt status to know which condition triggered
    bmp3_int_status interruptStatus;
    err = pressureSensor.getInterruptStatus(&interruptStatus);
    if (err == BMP3_OK)
    {
      if (interruptStatus.drdy)
      {
        int8_t err = pressureSensor.getSensorData(&data);
        if (err == BMP3_OK)
        {
          return true;
        }
      }
    }
  }
  return false;
}

void BM::read(double *p)
{
  *p = data.pressure / 100.0;
}
