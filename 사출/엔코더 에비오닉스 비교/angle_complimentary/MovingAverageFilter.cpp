#include "MovingAverageFilter.h"

MovingAverageFilter::MovingAverageFilter(int size) {
    this->size = size;
    buffer = new double[size];
    for (int i = 0; i < size; i++) {
        buffer[i] = 0.0;
    }
    index = 0;
    sum = 0.0;
}

MovingAverageFilter::~MovingAverageFilter() {
    delete[] buffer;
}

double MovingAverageFilter::filter(double value) {
    sum -= buffer[index];
    buffer[index] = value;
    sum += value;
    index = (index + 1) % size;
    return sum / size;
}
