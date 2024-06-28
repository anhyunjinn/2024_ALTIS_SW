#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(float q_angle, float q_gyro, float r_angle) {
    Q_angle = q_angle;
    Q_gyro = q_gyro;
    R_angle = r_angle;

    angle = 0.0f;
    bias = 0.0f;

    P[0][0] = 0.0f;
    P[0][1] = 0.0f;
    P[1][0] = 0.0f;
    P[1][1] = 0.0f;
}

float KalmanFilter::update(float new_angle, float new_rate, float dt) {
    rate = new_rate - bias;
    angle += dt * rate;

    // Update estimation error covariance - Project the error covariance ahead
    P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_gyro * dt;

    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    float S = P[0][0] + R_angle; // Estimate error
    float K[2]; // Kalman gain - This is a 2x1 vector
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;

    // Calculate the angle and bias - Update estimate with measurement zk (new_angle)
    float y = new_angle - angle; // Angle difference
    angle += K[0] * y;
    bias += K[1] * y;

    // Update the error covariance - Update the error covariance
    float P00_temp = P[0][0];
    float P01_temp = P[0][1];

    P[0][0] -= K[0] * P00_temp;
    P[0][1] -= K[0] * P01_temp;
    P[1][0] -= K[1] * P00_temp;
    P[1][1] -= K[1] * P01_temp;

    return angle;
}
