#ifndef SEGMENT_H
#define SEGMENT_H
#include "stop.hpp"

enum class SegmentType {
    PICKUP,
    TRAVEL,
    DROPOFF
};

class Segment {
    private:
        SegmentType type;
        Stop* beg;
        Stop* end;
        double total_distance;
        bool complete;
        int static_mem_usage;

    public:
        Segment(Stop& beg, Stop& end, SegmentType type);
        void MarkComplete();
};

#endif