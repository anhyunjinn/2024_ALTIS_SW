#include <Arduino.h>
#include "IMU.h"
#include "EJ.h"
#include "KalmanFilter.h"

// IMU
IMU imu(&Wire1, 1000000, 3, 16, 17); // I2C 객체, 클럭 속도, 인터럽트 핀 SCL, SDA

// Kalman Filters for roll and pitch
KalmanFilter kalmanRoll(0.001, 0.003, 0.015);
KalmanFilter kalmanPitch(0.001, 0.003, 0.015);

void imu_handler() {
    imu.handler();
}

// EJ
EJ ej(21, 5 * 1000, 1003.6); // PWM 핀, 타이머, 해면기압

void setup() {
    Serial.begin(115200);

    if (!imu.set()) {
        Serial.print("bmi270 에러");
        while (1);
    }
    attachInterrupt(digitalPinToInterrupt(imu._interruptPin), imu_handler, RISING);
    ej.set();
}

void loop() {
    static unsigned long lastTime = 0;
    unsigned long currentTime = millis();
    static unsigned long prevTime = 0;
    float dt = (currentTime - prevTime) / 1000.0;
    prevTime = currentTime;

    if (currentTime - lastTime >= 20) { // 20ms 주기로 데이터 전송
        lastTime = currentTime;

        float ax, ay, az, gx, gy, gz;
        int type = imu.read(&ax, &ay, &az, &gx, &gy, &gz);

        if (type == 2 || type == 3) {
            // 각속도를 각도로 변환
            float rollRate = gx;
            float pitchRate = gy;

            // 가속도를 이용한 각도 계산
            float roll = atan2(ay, az) * 57.2957795131; // 라디안을 도로 변환
            float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 57.2957795131;

            // 칼만 필터 적용
            roll = kalmanRoll.update(roll, rollRate, dt);
            pitch = kalmanPitch.update(pitch, pitchRate, dt);

            // 각도값 출력 (시리얼 플로터용)
            Serial.print(roll);
            Serial.print(",");
            Serial.println(pitch);
        }
    }
}
