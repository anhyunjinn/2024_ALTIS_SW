#ifndef MOVINGAVERAGEFILTER_H
#define MOVINGAVERAGEFILTER_H

class MovingAverageFilter {
public:
    MovingAverageFilter(int size);
    ~MovingAverageFilter();
    double filter(double value);

private:
    int size;
    int index;
    double sum;
    double* buffer;
};

#endif
