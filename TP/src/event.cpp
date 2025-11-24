#include "event.hpp"

// Construtor padrão: inicializa os atributos com os valores fornecidos
Event::Event(int id, double time, EventType type) : id(id), time(time), type(type) { 
    this->mem_usage = sizeof(int)*2 + sizeof(double) + sizeof(EventType);
}

// Construtor de cópia: inicializa os atributos com os valores do outro objeto
Event::Event(Event& other) 
    : id(other.id), time(other.time), type(other.type), mem_usage(other.mem_usage) { }

// Getters
int Event::GetID() {
    return this->id;
}

double Event::GetTime() {
    return this->time;
}

EventType Event::GetType() {
    return this->type;
}

// Sobrecarga de atribuição para cópia
void Event::operator=(const Event& other) {
    this->id = other.id;
    this->time = other.time;
    this->type = other.type;
}

// Retorna a memória usada pelo objeto
int Event::GetMemoryUsage() {
    return this->mem_usage;
}