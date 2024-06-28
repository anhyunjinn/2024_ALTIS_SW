#ifndef KALMANFILTER_H
#define KALMANFILTER_H

class KalmanFilter {
public:
    KalmanFilter(float q_angle, float q_gyro, float r_angle);
    float update(float new_angle, float new_rate, float dt);

private:
    float Q_angle; // Process noise variance for the accelerometer
    float Q_gyro; // Process noise variance for the gyro bias
    float R_angle; // Measurement noise variance

    float angle; // The angle calculated by the Kalman filter - part of the 2x1 state matrix
    float bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state matrix
    float rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

    float P[2][2]; // Error covariance matrix - This is a 2x2 matrix
};

#endif
