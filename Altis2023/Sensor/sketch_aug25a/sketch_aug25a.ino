#include "MPU9250.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

struct Sensor {
  float ax, ay, az;
  float gx, gy, gz;
  float alt;
};

Sensor sensor;

//---------------------êµ¬ì¡°ì²´----------------------//

MPU9250 mpu;

float Gavgz = 0;
float Gavgx = 0;
float Gavgy = 0;
float Aavgz = 0;
float Aavgx = 0;
float Aavgy = 0;
float altitude = 0;

//---------------------mpu9250-------------------------//

Adafruit_BMP280 bmp1;  // use I2C interface
Adafruit_Sensor *bmp_temp1 = bmp1.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure1 = bmp1.getPressureSensor();

unsigned long now = 0;
unsigned long t = 0;
unsigned long dt = 0;

//----------------------bmp280--------------------------//

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);
  settingMPU();
  calibrationMPU();
  settingBMP();
}

void loop() {
  if (mpu.update()) {
    getimu();
  }

  getAltitude();
  Serial.print(sensor.alt);
  Serial.println(" m");
  Serial.println(sensor.az);
}

void getimu() {
  sensor.az = mpu.getAccZ() - Aavgz + 1;
  sensor.ax = mpu.getAccX() - Aavgx;
  sensor.ay = mpu.getAccY() - Aavgy;
  sensor.gx = mpu.getGyroX() - Gavgx;
  sensor.gy = mpu.getGyroY() - Gavgy;
  sensor.gz = mpu.getGyroZ() - Gavgz;
}

void getAltitude() {
  sensors_event_t temp_event1, pressure_event1, altitude_event1;
  bmp_temp1->getEvent(&temp_event1);
  bmp_pressure1->getEvent(&pressure_event1);
  float temperature = temp_event1.temperature;
  float pressure = pressure_event1.pressure;
  altitude = bmp1.readAltitude(1013.25);
  sensor.alt = altitude;
}

void settingMPU() {
  MPU9250Setting setting;
  setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
  setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
  setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
  setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_200HZ;
  setting.gyro_fchoice = 0x03;
  setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_41HZ;
  setting.accel_fchoice = 0x01;
  setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;

  if (!mpu.setup(0x68, setting)) {  // change to your own address
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
}

void settingBMP() {
  while (!Serial) delay(100);  // wait for native usb
  Serial.println(F("BMP280 Sensor event test"));

  unsigned status;
  status = bmp1.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    Serial.print("SensorID was: 0x");
    Serial.println(bmp1.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);

    bmp1.setSampling(Adafruit_BMP280::MODE_NORMAL,   // Normal ëª¨ëë¡ ì¤ì 
                     Adafruit_BMP280::SAMPLING_X2,   // ì¨ë í´ìë 17-bit (x2)
                     Adafruit_BMP280::SAMPLING_X16,  // ìë ¥ í´ìë 20-bit (x16)
                     Adafruit_BMP280::FILTER_X16,    // IIR íí° ìíë§ ì 16
                     Adafruit_BMP280::STANDBY_MS_500);

    bmp_temp1->printSensorDetails();
  }
}

void calibrationMPU() {
  for (int i = 0; i < 1000; i++) {
    if (mpu.update()) {
      Gavgz += mpu.getGyroZ();
      Gavgx += mpu.getGyroX();
      Gavgy += mpu.getGyroY();
      Aavgz += mpu.getAccZ();
      Aavgx += mpu.getAccX();
      Aavgy += mpu.getAccY();
    }
  }
  Gavgz = Gavgz / 1000;
  Gavgx = Gavgx / 1000;
  Gavgy = Gavgy / 1000;
  Aavgz = Aavgz / 1000;
  Aavgx = Aavgx / 1000;
  Aavgy = Aavgy / 1000;
}
