#include "stop.hpp"

// CONSTRUTOR: inicializa a parada com base na demanda e tipo (coleta ou desembarque) passados
Stop::Stop(Demand& demand, StopType type) {
    this->demand_id = demand.GetID();

    switch(type) {
        case StopType::DROPOFF:
            this->type = StopType::DROPOFF;
            this->stop = demand.GetDestination();
            break;

        case StopType::PICKUP:
            this->type = StopType::PICKUP;
            this->stop = demand.GetOrigin();
            break;
    }

    this->static_mem_usage = 2*sizeof(int) + sizeof(StopType) + sizeof(Point2D*);
}

// GetPoint: Retorna referência para o ponto da parada
Point2D& Stop::GetPoint() {
    return this->stop;
}

// GetType: Retorna o tipo desta parada
StopType Stop::GetType() {
    return this->type;
}

// Distance: Retorna a distância entre esta parada e outra
double Stop::Distance(Stop& other) {
    return this->stop.Distance(other.GetPoint());
}

// GetMemoryUsage: Retorna a quantidade de memória usada pelo objeto
int Stop::GetMemoryUsage() {
    return this->static_mem_usage;
}