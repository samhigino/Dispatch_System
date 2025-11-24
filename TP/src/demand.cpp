#include "demand.hpp"

// CONSTRUTOR COMPLETO
Demand::Demand(int id, double time, double ox, double oy, double dx, double dy) {
    this->id = id;
    this->time = time;
    this->origin = Point2D(ox, oy);
    this->destination = Point2D(dx, dy);

    this->mem_usage = 3*sizeof(int) + 2*sizeof(Point2D);
}

// CONSTRUTOR DE CÓPIA
Demand::Demand(const Demand& other) {
    this->id = other.id;
    this->time = other.time;
    this->origin = other.origin;
    this->destination = other.destination;

    this->mem_usage = other.mem_usage; // Memória usada é a mesma
}

// GETTERS
int Demand::GetID() {
    return this->id;
}

double Demand::GetTime() {
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

// Calcula a distância entre a origem e o destino desta demanda
double Demand::GetDistance() {
    return this->origin.Distance(this->destination);
}

// Sobrecarga de atribuição
void Demand::operator=(const Demand& other) {
    this->id = other.id;
    this->time = other.time;
    this->origin = other.origin;
    this->destination = other.destination;
    
    this->mem_usage = other.mem_usage; // Memória usada é a mesma
}

int Demand::GetMemoryUsage() {
    return this->mem_usage;
}