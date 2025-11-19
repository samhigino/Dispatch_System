#ifndef RIDE_H
#define RIDE_H
#include "segment.hpp"

const static int MAX_SEGMENTS = 50;

class Ride {
    private:
        Segment segments[MAX_SEGMENTS];

    public:
        Ride();
};

#endif