#include "BM.h"
// #define debug
BM::BM(TwoWire *wire, int clock, int interruptPin, int scl, int sda, float SEALEVELPRESSURE_HPA)
    : _Wire(*wire),
      _interruptPin(interruptPin),
      _clock(clock),
      _scl(scl),
      _sda(sda),
      _SEALEVELPRESSURE_HPA(SEALEVELPRESSURE_HPA)
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

    // Variable to track errors returned by API calls
    int8_t err = BMP3_OK;
    // ODR settings, see bmp3_defs.h (line 232-249)
    err = pressureSensor.setODRFrequency(BMP3_ODR_200_HZ);
    if (err != BMP3_OK)
    {
        return false;
    }
    //setOSRMultipliers,setFilterCoefficient,


    // Configure the BMP384 to trigger interrupts whenever a measurement is performed
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

void BM::read(double *p, double *a)
{
        *p = data.pressure / 100.0;
        *a = readAltitude(_SEALEVELPRESSURE_HPA);
}

double BM::readAltitude(double seaLevel)
{
    float atmospheric = data.pressure / 100.0F;
    return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
}