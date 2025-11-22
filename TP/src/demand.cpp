#include "demand.hpp"

// CONSTRUTOR COMPLETO
Demand::Demand(int id, int time, double ox, double oy, double dx, double dy) {
    this->id = id;
    this->time = time;
    this->origin = Point2D(ox, oy);
    this->destination = Point2D(dx, dy);
    this->state = State::DEMANDED;

    this->mem_usage = 3*sizeof(int) + 2*sizeof(Point2D) + sizeof(State);
}

// GETTERS
int Demand::GetID() {
    return this->id;
}

int Demand::GetTime() {
    return this->time;
}

Point2D& Demand::GetOrigin() {
    return this->origin;
}

Point2D& Demand::GetDestination() {
    return this->destination;
}

// Calcula a distância entre as origens desta e de outra demanda
double Demand::OriginDistance(Demand& other) {
    return this->origin.Distance(other.GetOrigin());
}

// Calcula a distância entre os destinos desta e de outra demanda
double Demand::DestinationDistance(Demand& other) {
    return this->destination.Distance(other.GetDestination());
}

double Demand::GetDistance() {
    return this->origin.Distance(this->destination);
}

int Demand::GetMemoryUsage() {
    return this->mem_usage;
}