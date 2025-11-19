#include "state_manager.hpp"

//-------------------------------------------------------------------------------
// CONSTRUTOR E DESTRUTOR
//-------------------------------------------------------------------------------

// CONSTRUTOR: incializa todas os parâmetros de simulação com os parâmetros, tempo global como 0, memória estática e extra como apropriado
Manager::Manager(int eta, double gamma, double delta, double alpha, double beta, float lambda) {
    // Parametrização da simulação
    this->veh_capacity = eta;
    this->veh_speed = gamma;
    this->delta = delta;
    this->origin_max_distance = alpha;
    this->destin_max_distance = beta;
    this->min_efficiency = lambda;

    // Objetos de simulação e variáveis de controle
    this->global_time = 0;
    this->demand_groups = new DemandGroup*[MAX_GROUPS];
    this->rides = new Ride*[MAX_GROUPS];

    // Controle de memória
    this->static_mem_usage = 4*sizeof(int) + 4*sizeof(double) + sizeof(float) + sizeof(DemandGroup**)+ sizeof(Ride**) + sizeof(DemandGroup*)*MAX_GROUPS + sizeof(Ride*)*MAX_GROUPS;
    this->extra_mem_usage = 0;
}

// DESTRUTOR: libera memória alocada para os grupos de demandas e corridas
Manager::~Manager() {
    for(int i = 0; i < MAX_GROUPS; i++) {
        delete this->demand_groups[i];
        delete this->rides[i];
    }
    delete[] this->demand_groups;
    delete[] this->rides;
}

//-------------------------------------------------------------------------------
// SIMULAÇÃO (PRÉ, DURANTE E PÓS)
//-------------------------------------------------------------------------------

// MakeDemand (pré-simulação): Cria uma nova demanda com os parâmetros passados e a insere no grupo de demandas seguindo as restrições de compartilhamento. Retorna o grupo em que a demanda foi inserida ou -1 se não foi possível inserir em nenhum grupo.
int Manager::MakeDemand(int id, int t, double ox, double oy, double dx, double dy) {
    // Controle de memória: armazenamento do uso de memória anterior para comparação posterior
    int prev_mem_usage = this->max_extra_mem_usage;
    int max_usage = 0;

    // Criação da demanda e recuperação da primeira demanda no grupo mais recente ainda não fechado
    Demand* new_demand = new Demand(id, t, ox, oy, dx, dy);                 // nova demanda
    DemandGroup* current_group = demand_groups[this->group_count - 1];      // grupo mais recente
    Demand* dem_in_place = current_group->Get(1);                           // demanda de comparação
    int time_diff = new_demand->GetTime() - dem_in_place->GetTime();        // diferença de tempo entre ambas

    // Update da memória
    this->extra_mem_usage += new_demand->GetMemoryUsage() + sizeof(new_demand) + sizeof(current_group) + sizeof(dem_in_place) + sizeof(time_diff);

    // Se o critério de tempo compatível, inicia a checagem dos outros critérios
    bool compatible = true;    // sairá do if como false se não for compatível com o grupo atual (por qualquer critério)
    if(abs(time_diff) <= this->delta) {
        // Checagem de distância entre origens e destinos
        for(int i = 0; i < current_group->Size(); i++) {
            double orig_dist = current_group->Get(i)->GetOriginDistance(*new_demand);
            double dest_dist = current_group->Get(i)->GetDestinationDistance(*new_demand);

            if(orig_dist > this->origin_max_distance || dest_dist > this->destin_max_distance) {
                compatible = false;
                break;
            }
        }

        // Checagem de eficiência mínima
        if(compatible) {

        }
    }
}

//-------------------------------------------------------------------------------
// CONTROLE DE MEMÓRIA
//-------------------------------------------------------------------------------

// GetStaticMemUsage: Retorna a memória estática usada pelo objeto
int Manager::GetStaticMemUsage() {
    return this->static_mem_usage;
}

// GetExtraMemUsage: Retorna a memória auxiliar (variável) máxima já usada pelo objeto
// Iteradores e variáveis para o próprio controle de memória são ignorados (desprezíveis)
int Manager::GetExtraMemUsage() {
    return this->max_extra_mem_usage;
}

// GetMemoryUsage: Retorna a memória total usada pelo objeto
int Manager::GetMemoryUsage() {
    return this->static_mem_usage + this->extra_mem_usage;
}