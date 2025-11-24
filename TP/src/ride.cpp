#include "ride.hpp"
#include "demand_group.hpp"
#include "eff_error.hpp"

//-------------------------------------------------------------------------------
// CONSTRUTOR E DESTRUTOR
//-------------------------------------------------------------------------------

// CONSTRUTOR: cria a corrida com base em um grupo de demandas passado; se a eficiência mínima não for atingida, lança uma exceção e cancela a criação da corrida
Ride::Ride(DemandGroup& group, double min_efficiency) {
    // Inicialização do tamanho
    int size = group.Size();
    if(size == 0) {
        throw std::logic_error("Can't create ride: group has no demands.");
    }
    
    // Alocação de memória para as paradas (armazenadas em um vetor de ponteiros)
    this->stop_amount = size*2;
    this->stops = new Stop*[stop_amount];

    for(int i = 0; i < stop_amount; i++) {
        this->stops[i] = nullptr;
    }

    // Criação das paradas de coleta e entrega para cada demanda no grupo
    for(int i = 0; i < size; i++) {
        Stop* new_stop_o = new Stop(*group.Get(i), StopType::PICKUP);
        Stop* new_stop_d = new Stop(*group.Get(i), StopType::DROPOFF);
        this->stops[i] = new_stop_o;
        this->stops[i + size] = new_stop_d;

        this->mem_usage += new_stop_o->GetMemoryUsage() + new_stop_d->GetMemoryUsage();
    }

    // Criação dos segmentos + cálculo prévio da distância total
    this->segment_amount = size*2-1;
    double dist = 0;
    this->segments = new Segment[segment_amount];

    for(int i = 0; i < segment_amount; i++) {
        Segment* new_segment = new Segment(*stops[i], *stops[i + 1]);
        this->segments[i] = *new_segment;
        dist += new_segment->GetDistance();
    }

    // Inicialização dos atributos de simulação
    this->ongoing = false;
    this->done = false;
    this->distance = dist;
    this->start = group.Get(0)->GetTime();
    this-> duration = 0;
    this->end = 0;
    
    // Cálculo da eficiência: lança uma exceção caso a eficiência mínima não tenha sido atingida e cancela a criação desta corrida
    if(this->segments[size-1].GetType() == SegmentType::TRAVEL) {
        // double travel_dist = this->segments[size-1].GetDistance();
        double individual_dist = 0;
        for(int i = 0; i < group.Size(); i++) {
            individual_dist += group.Get(i)->GetDistance();
        }
        this->efficiency = individual_dist/distance;

        if(this->efficiency < min_efficiency) {
            throw low_efficiency("Minimum efficiency not reached.");
        }
    }
    else {
        throw std::runtime_error("Wrong segment selected for efficiency evaluation.");
    }

    // Cálculo da memória usada
    this->mem_usage = sizeof(Segment)*segment_amount + sizeof(Segment*) + sizeof(Stop*) + sizeof(int)*3 + sizeof(double)*2 + sizeof(bool);
}

// DESTRUTOR: apaga os segmentos (que por sua vez vão apagar as paradas)
Ride::~Ride() {
    for(int i = 0; i < this->stop_amount; i++) {
        delete this->stops[i];
    }
    delete[] this->stops;
    delete[] this->segments;
}

//-------------------------------------------------------------------------------
// OPERAÇÕES/MÉTODOS
//-------------------------------------------------------------------------------

// Marca que esta corrida agora está em andamento
void Ride::Start() {
    this->ongoing = true;
}

// Marca tanto esta corrida como todos os segmentos como terminados
void Ride::MarkDone() {
    for(int i = 0; i < this->segment_amount; i++) {
        this->segments[i].MarkComplete();
    }

    this->ongoing = false;
    this->done = true;
}

// Calcula a duração total desta corrida
void Ride::CalculateDuration(double veh_speed) {
    this->duration = this->distance/veh_speed;
    this->end = this->start + this->duration;
}

// Imprime as coordenadas de cada parada, em ordem, na saída passada
void Ride::PrintStops(std::ostream& out) {
    for(int i = 0; i < stop_amount; i++) {
        out << " " << stops[i]->GetPoint().GetX() << " " << stops[i]->GetPoint().GetY();
    }
}

//-------------------------------------------------------------------------------
// GETTERS
//-------------------------------------------------------------------------------

double Ride::GetEfficiency() {
    return this->efficiency;
}

double Ride::GetDistance() {
    return this->distance;
}

double Ride::GetStart() {
    return this->start;
}

double Ride::GetDuration() {
    return this->duration;
}

double Ride::GetEnd() {
    return this->end;
}

int Ride::GetStopAmount() {
    return this->stop_amount;
}

//-------------------------------------------------------------------------------
// CONTROLE DE MEMÓRIA
//-------------------------------------------------------------------------------

int Ride::GetMemoryUsage() {
    return this->mem_usage;
}