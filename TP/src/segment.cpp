#include "segment.hpp"

Segment::Segment(Stop& beg, Stop& end, SegmentType type) {
    this->type = type;
    this->beg = &beg;
    this->end = &end;
    this->complete = false;
    this->total_distance = beg.GetPoint()->Distance(*end.GetPoint());
    this->static_mem_usage = sizeof(SegmentType) + beg.GetMemoryUsage() + end.GetMemoryUsage() + sizeof(double) + sizeof(bool) + sizeof(int);
}

void Segment::MarkComplete() {
    this->complete = true;
}