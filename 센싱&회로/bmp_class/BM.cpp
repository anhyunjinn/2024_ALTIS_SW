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
    _Wire.setSCL(_scl);
    _Wire.setSDA(_sda);

    if (pressureSensor.beginI2C(BMP384_I2C_ADDRESS_DEFAULT) != BMP3_OK)
    {
        return false;
    }
    // 데이터주기 설정
    //  Variable to track errors returned by API calls
    int8_t err = BMP3_OK;
    // ODR settings, see bmp3_defs.h (line 232-249)
    err = pressureSensor.setODRFrequency(BMP3_ODR_200_HZ);
    if (err != BMP3_OK)
    {
        return false;
    }
    // IIR필터 설정
    err = pressureSensor.setFilterCoefficient(BMP3_IIR_FILTER_COEFF_127);
    if (err)
    {
        return false;
    }
    // 오버샘플링 설정
    bmp3_odr_filter_settings osrMultipliers =
        {
            .press_os = BMP3_OVERSAMPLING_32X,
            .temp_os = BMP3_OVERSAMPLING_2X,
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
