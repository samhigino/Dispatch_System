#include "segment.hpp"

//-------------------------------------------------------------------------------
// CONSTRUTORES E DESTRUTOR
//-------------------------------------------------------------------------------

// CONSTRUTOR PADRÃO: inicializa um "segmento nulo" (tipo padroniza para PICKUP)
Segment::Segment() {
    this->beg = nullptr;
    this->end = nullptr;
    this->complete = false;
    this->total_distance = 0;
    this->type = SegmentType::PICKUP;

    this->static_mem_usage = sizeof(SegmentType) + sizeof(double) + sizeof(bool) + sizeof(int);
}

// CONSTRUTOR PRINCIPAL: inicializa os ponteiros referenciando as paradas passadas como parâmetro e formaliza o tipo de segmento com base nas paradas
Segment::Segment(Stop& begs, Stop& ends) {
    this->beg = &begs;
    this->end = &ends;
    this->complete = false;
    this->total_distance = beg->Distance(*end);
    
    switch(beg->GetType()) {
        case StopType::PICKUP:
            if(end->GetType() == StopType::PICKUP) {
                this->type = SegmentType::PICKUP;
            }
            else {
                this->type = SegmentType::TRAVEL;
            }
            break;

        case StopType::DROPOFF:
            if(end->GetType() == StopType::DROPOFF) {
                this->type = SegmentType::DROPOFF;
            }
            else {
                throw "Invalid stops when creating segment.";   // Caso as paradas passadas sejam (entrega, coleta), o que não faz sentido
            }
            break;
    }

    this->static_mem_usage = sizeof(SegmentType) + begs.GetMemoryUsage() + ends.GetMemoryUsage() + sizeof(double) + sizeof(bool) + sizeof(int);
}

// DESTRUTOR: destrói as paradas associadas
Segment::~Segment() {
    try {
        delete this->beg;
        delete this->end;
    }
    catch(...) {
        return;
    }
}

//-------------------------------------------------------------------------------
// OPERAÇÕES/MÉTODOS
//-------------------------------------------------------------------------------

// Assinala compleção do segmento
void Segment::MarkComplete() {
    this->complete = true;
}

// Sobrecarga de operação: atualiza as paradas e os atributos de acordo
void Segment::operator=(Segment& other) {
    if(this->beg = nullptr) {
        this->beg = other.beg;
    }
    else {
        delete this->beg;
        this->beg = other.beg;
    }

    if(this->end = nullptr) {
        this->end = other.end;
    }
    else {
        delete this->end;
        this->end = other.end;
    }
    
    this->total_distance = other.total_distance;
    this->type = other.type;

    this->static_mem_usage = other.static_mem_usage;
}

// Retorna o tipo do segmento
SegmentType Segment::GetType() {
    return this->type;
}

// Retorna a distância total do segmento
double Segment::GetDistance() {
    return this->total_distance;
}

// Retorna o uso da memória do objeto
int Segment::GetMemoryUsage() {
    return this->static_mem_usage;
}