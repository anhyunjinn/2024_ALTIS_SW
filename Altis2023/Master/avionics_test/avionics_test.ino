#include "MPU9250.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include "FS.h"
#include "SD.h"

/*íµì */
#define lora Serial2
unsigned long frequency = 500;
unsigned long lora_time;
struct Sensor {
  //imu
  float ax, ay, az;
  float gx, gy, gz;
  //alt
  float alt;
  //gps
  float lat;
  float lng;
  //angle
  float yaw;
  float pitch;
  //time
  unsigned long t;
  bool ej;
};
Sensor sensor;

/*mpu9250*/
MPU9250 mpu;
float Gavgz = 0;
float Gavgx = 0;
float Gavgy = 0;
float Aavgz = 0;
float Aavgx = 0;
float Aavgy = 0;
/*bmp280*/
float altitude = 0;

/*bmp280*/
Adafruit_BMP280 bmp;  // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();
unsigned long now = 0;
unsigned long t = 0;
unsigned long dt = 0;
double alt_avg = 0;
float air_p = 1019.3;

/*gps*/
#define GPS Serial1
TinyGPSPlus gps;

/*sdì¹´ë*/
#define SD_CS_PIN 5
File dataFile;

/*Ejection*/
int relay = 13;

void setup() {
  Serial.begin(115200);
  lora.begin(9600, SERIAL_8N1, 16, 17);
  GPS.begin(9600, SERIAL_8N1, 14, 27);
  Wire.begin();
  delay(100);
  settingMPU();
  settingBMP();
  initializeSDCard();
  writeSDCardHeader("/sensor_data.txt");
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
}

void loop() {
  if (mpu.update()) {
    getimu();
  }
  if (GPS.available() > 0) {
    getGps();
  }
  getAltitude();
  if (millis() - lora_time >= frequency) {  // ìê° ê°ê²©
    sensor.t = millis();
    lora.write((byte *)&sensor, sizeof(sensor));
    lora_time = millis();
  }
  CF();
  ej();
  saveDataToSDCard(sensor);
  display();
}

/*imu*/
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
  if (!mpu.setup(0x68, setting)) {
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(1000);
    }
  }
  //calibration
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
//imu get
void getimu() {
  sensor.az = (mpu.getAccZ() - Aavgz + 1) * 0.981;
  sensor.ax = (mpu.getAccX() - Aavgx) * 0.981;
  sensor.ay = (mpu.getAccY() - Aavgy) * 0.981;
  sensor.gx = (mpu.getGyroX() - Gavgx) * 180.0 / PI;
  sensor.gy = (mpu.getGyroY() - Gavgy) * 180.0 / PI;
  sensor.gz = (mpu.getGyroZ() - Gavgz) * 180.0 / PI;
}
/*bmp280*/
//setting bmp
void settingBMP() {
  while (!Serial) delay(100);  // wait for native usb
  Serial.println(F("BMP280 Sensor event test"));

  unsigned status;
  status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    while (1) delay(100);

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,   // Normal ëª¨ëë¡ ì¤ì 
                    Adafruit_BMP280::SAMPLING_X2,   // ì¨ë í´ìë 17-bit (x2)
                    Adafruit_BMP280::SAMPLING_X16,  // ìë ¥ í´ìë 20-bit (x16)
                    Adafruit_BMP280::FILTER_X16,    // IIR íí° ìíë§ ì 16
                    Adafruit_BMP280::STANDBY_MS_500);

    bmp_temp->printSensorDetails();
  }
  for (int i = 0; i < 1000; i++) {
    sensors_event_t temp_event, pressure_event, altitude_event;
    bmp_temp->getEvent(&temp_event);
    bmp_pressure->getEvent(&pressure_event);
    float temperature = temp_event.temperature;
    float pressure = pressure_event.pressure;
    altitude = bmp.readAltitude(air_p);
    alt_avg += altitude / 1000;
  }
}
//get altitude
void getAltitude() {
  sensors_event_t temp_event, pressure_event, altitude_event;
  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);
  float temperature = temp_event.temperature;
  float pressure = pressure_event.pressure;
  altitude = bmp.readAltitude(air_p);
  sensor.alt = altitude - alt_avg;
}

/*gps*/
void getGps() {
  if (gps.encode(GPS.read())) {
    if (gps.location.isValid()) {
      sensor.lat = gps.location.lat();
      sensor.lng = gps.location.lng();
    } else {
      sensor.lat = 0;
      sensor.lng = 0;
    }
  }
}
bool initializeSDCard() {
  if (!SD.begin()) {
    return false;
    while (1) { Serial.println("sdì¹´ë ì¤ë¥"); }
  }
  return true;
}

void saveDataToSDCard(Sensor sensor) {
  File dataFile = SD.open("/sensor_data.txt", FILE_APPEND);
  if (!dataFile) {
    while (1) {
      Serial.println("Failed to open file for appending");
    }
  }

  // Format the data as a comma-separated line
  String dataMessage = String(sensor.ax, 6) + "," + String(sensor.ay, 6) + "," + String(sensor.az, 6) + ","
                       + String(sensor.gz, 6) + "," + String(sensor.alt, 2) + "," + String(sensor.lat, 6) + "," + String(sensor.lng, 6) + ","
                       + String(sensor.yaw, 6) + "," + String(sensor.pitch, 6) + "," + String(sensor.t) + "," + String(sensor.ej) + "\r\n";

  dataFile.print(dataMessage);
  dataFile.close();
}
void writeSDCardHeader(const char *path) {
  File dataFile = SD.open(path, FILE_WRITE);
  if (!dataFile) {
    while (1) {
      Serial.println("Failed to open file for appending");
    }
  }

  dataFile.print("Ax, Ay, Az, Gz, Alt, Lat, Lng, Yaw, Pitch, Time, EJ \r\n");
  dataFile.close();
}

void ej() {
  if (lora.available() > 0) {
    char command = lora.read();
    if (command == 'e') {
      sensor.ej = true;
    }
  } 
  else if (sensor.alt > 120 && abs(sensor.yaw) * abs(sensor.yaw) + abs(sensor.pitch) * abs(sensor.pitch) > 6400) {
    sensor.ej = true;
  } 
  else {
    sensor.ej = false;
  }

  if (sensor.ej == true) {
    digitalWrite(relay, HIGH);
  } 
  else {
    digitalWrite(relay, LOW);
  }
}

void CF() {
  static double angleAccX = atan2(sensor.ay, sqrt(sensor.az * sensor.az + sensor.ax * sensor.ax)) * 360 / 2.0 / PI;
  static double angleAccY = atan2(sensor.ax, sqrt(sensor.az * sensor.az + sensor.ay * sensor.ay)) * 360 / -2.0 / PI;

  static double interval = 0;
  interval = (millis() - interval) * 0.001;

  sensor.yaw = (0.98 * (sensor.yaw + sensor.gx * interval)) + (0.02 * angleAccX);
  sensor.pitch = (0.98 * (sensor.pitch + sensor.gy * interval)) + (0.02 * angleAccY);

  interval = millis();
}

void display() {
  Serial.print("ax: ");
  Serial.print(sensor.ax);
  Serial.print("\tay: ");
  Serial.print(sensor.ay);
  Serial.print("\taz: ");
  Serial.print(sensor.az);
  Serial.print("\tgx: ");
  Serial.print(sensor.gx);
  Serial.print("\tgy: ");
  Serial.print(sensor.gy);
  Serial.print("\tgz: ");
  Serial.print(sensor.gz);
  Serial.print("\talt: ");
  Serial.print(sensor.alt);
  Serial.print("\tlat: ");
  Serial.print(sensor.lat, 6);  // ììì  6ìë¦¬ê¹ì§ ì¶ë ¥
  Serial.print("\tlng: ");
  Serial.print(sensor.lng, 6);  // ììì  6ìë¦¬ê¹ì§ ì¶ë ¥
  Serial.print("\tyaw: ");
  Serial.print(sensor.yaw);
  Serial.print("\tpitch: ");
  Serial.print(sensor.pitch);
  Serial.print("\tt: ");
  Serial.print(sensor.t);
  Serial.print("\tej: ");
  Serial.println(sensor.ej);
}