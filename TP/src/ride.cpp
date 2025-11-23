#include "ride.hpp"
#include "demand_group.hpp"
#include "eff_error.hpp"

//-------------------------------------------------------------------------------
// CONSTRUTOR E DESTRUTOR
//-------------------------------------------------------------------------------

// CONSTRUTOR: cria a corrida com base em um grupo de demandas passado; se a eficiência mínima não for atingida, lança uma exceção e cancela a criação da corrida
Ride::Ride(DemandGroup& group, double min_efficiency, std::ostream& debug) {
    debug << "\n\t\t\t\t\t\tTrying to create Ride...\n";
    debug.flush();

    // Alocação de memória para as paradas (armazenadas em um vetor de ponteiros)
    int size = group.Size();
    if(size == 0) {
        debug << "\t\t\t\t\t\tCOULDN'T CREATE RIDE: group has no demands.\n\n";
        debug.flush();
        throw std::logic_error("Can't create ride: group has no demands.");
    }
    this->stop_amount = size*2;
    debug << "\t\t\t\t\t\tThis ride attends " << size << " demands and has " << stop_amount << " stops.\n";
    this->stops = new Stop*[stop_amount];
    for(int i = 0; i < stop_amount; i++) {
        this->stops[i] = nullptr;
    }
    debug << "\t\t\t\t\t\tJust created stops array. Now creating the stops themselves.\n";

    // Criação das paradas de coleta e entrega para cada demanda no grupo
    for(int i = 0; i < size; i++) {
        debug << "\t\t\t\t\t\t\tCreating stops for demand " << i << " (id " << group.Get(i)->GetID() << ").\n";
        debug.flush();
        Stop* new_stop_o = new Stop(*group.Get(i), StopType::PICKUP);
        Stop* new_stop_d = new Stop(*group.Get(i), StopType::DROPOFF);
        this->stops[i] = new_stop_o;
        this->stops[i + size] = new_stop_d;

        this->mem_usage += new_stop_o->GetMemoryUsage() + new_stop_d->GetMemoryUsage();
    }
    debug << "\t\t\t\t\t\tAll stops created successfully.\n";
    debug.flush();

    // Criação dos segmentos + cálculo prévio da distância total
    debug << "\t\t\t\t\t\tNow creating segments and calculating total distance.\n";
    this->segment_amount = size*2-1;
    double dist = 0;
    this->segments = new Segment[segment_amount];
    debug << "\t\t\t\t\t\tJust created segments array. Now creating the segments themselves.\n";

    for(int i = 0; i < segment_amount; i++) {
        debug << "\t\t\t\t\t\t\tCreating segment " << i << " (from stop " << i << " to stop " << i + 1 << ").\n";
        Segment* new_segment = new Segment(*stops[i], *stops[i + 1]);
        this->segments[i] = *new_segment;
        dist += new_segment->GetDistance();
        debug << "\t\t\t\t\t\t\tSegment created successfully.\n\t\t\t\t\t\t\t\tDistance: " << new_segment->GetDistance() << ";\n\t\t\t\t\t\t\t\tType: ";
        switch(new_segment->GetType()) {
            case SegmentType::PICKUP:
                debug << "PICKUP.\n";
                break;
            case SegmentType::TRAVEL:
                debug << "TRAVEL.\n";
                break;
            case SegmentType::DROPOFF:
                debug << "DROPOFF.\n";
                break;
        }
        debug.flush();
    }
    debug << "\t\t\t\t\t\tAll segments created successfully.\n";

    // Inicialização dos atributos de simulação
    debug << "\t\t\t\t\t\tFinishing Ride creation by initializing simulation attributes. This should be trivial.\n";
    debug.flush();
    this->ongoing = false;
    this->done = false;
    this->distance = dist;
    this->start = group.Get(0)->GetTime();
    this-> duration = 0;
    this->end = 0;
    debug << "\t\t\t\t\t\tYeah, it was trivial.\n";
    debug.flush();
    
    // Cálculo da eficiência: lança uma exceção caso a eficiência mínima não tenha sido atingida e cancela a criação desta corrida
    debug << "\t\t\t\t\t\tNow let's calculate efficiency and check minimum efficiency criterion.\n";
    if(this->segments[size-1].GetType() == SegmentType::TRAVEL) {
        // double travel_dist = this->segments[size-1].GetDistance();
        double individual_dist = 0;
        for(int i = 0; i < group.Size(); i++) {
            individual_dist += group.Get(i)->GetDistance();
        }
        this->efficiency = individual_dist/distance;
        debug << "\t\t\t\t\t\t\tIndividual distances combined: " << individual_dist << ";\n\t\t\t\t\t\t\tTotal distance: " << distance << ";\n";
        debug << "\t\t\t\t\t\t\tEfficiency calculated: " << this->efficiency << ". Minimum required: " << min_efficiency << ".\n";
        debug.flush();

        if(this->efficiency < min_efficiency) {
            debug << "\t\t\t\t\t\tCOULDN'T CREATE RIDE: minimum efficiency not reached. Deleting everything.\n\n";
            debug.flush();
            throw low_efficiency("Minimum efficiency not reached.");
        }
    }
    else {
        debug << "\t\t\t\t\t\tCOULDN'T CREATE RIDE: segment type didn't match.\n\n";
        debug.flush();
        throw std::runtime_error("Wrong segment selected for efficiency evaluation.");
    }

    // Cálculo da memória usada
    debug << "\t\t\t\t\t\tEfficiency ok.\n";
    debug << "\t\t\t\t\t\tFinishing creation by calculating used memory.\n\n";
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

double Ride::GetEfficiency() {
    return this->efficiency;
}

double Ride::GetDistance() {
    return this->distance;
}

int Ride::GetStart() {
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

// Imprime as coordenadas de cada parada, em ordem, na saída passada
void Ride::PrintStops(std::ofstream& out) {
    for(int i = 0; i < stop_amount; i++) {
        out << " " << stops[i]->GetPoint().GetX() << " " << stops[i]->GetPoint().GetY();
    }
}

//-------------------------------------------------------------------------------
// CONTROLE DE MEMÓRIA
//-------------------------------------------------------------------------------

int Ride::GetMemoryUsage() {
    return this->mem_usage;
}