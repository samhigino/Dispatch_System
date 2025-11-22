#include "simulation_manager.hpp"
#include "eff_error.hpp"

//-------------------------------------------------------------------------------
// FUNÇÕES AUXILIARES
//-------------------------------------------------------------------------------

// UpdateMemory: atualiza a memória máxima usada se o uso de memória extra atual for maior que o máximo anterior
void Manager::UpdateMemory() {
    if(this->extra_mem_usage > this->max_extra_mem_usage) {
        this->max_extra_mem_usage = this->extra_mem_usage;
    }
}

// CreateDemandGroup: cria um novo grupo de demandas, insere-a no vetor de grupos e retorna o ponteiro para o novo grupo
DemandGroup* Manager::CreateDemandGroup() {
    // Criação do grupo
    if(group_count < MAX_GROUPS) {
        this->demand_groups[group_count] = new DemandGroup(this->veh_capacity);
        this->group_count++;
    }
    else {
        throw std::out_of_range("Max demand groups reached. Can't create a new group.");
    }

    // Update de memória
    this->extra_mem_usage += demand_groups[group_count]->GetMemoryUsage();
    UpdateMemory();

    return demand_groups[this->group_count - 1];
}

// MakeRide: cria uma nova corrida baseada no grupo passado como parâmetro e a insere no vetor de corridas
void Manager::MakeRide(DemandGroup* group) {
    if(ride_count == MAX_GROUPS) {
        throw std::out_of_range("Can't create a new ride: max number of rides reached.");
    }

    try {
        // Criação da corrida
        this->rides[ride_count] = new Ride(group, this->min_efficiency);
        this->rides[ride_count]->CalculateDuration(this->veh_speed);
        double ride_start = this->rides[ride_count]->GetStart();
        double ride_end = ride_start + this->rides[ride_count]->GetDuration();

        // Agendamento dos eventos
        this->scaler.ScheduleEvent(ride_count, ride_start, EventType::RIDESTART);
        this->scaler.ScheduleEvent(ride_count, ride_end, EventType::RIDEEND);
        ride_count++;

        // Update de memória
        this->extra_mem_usage += rides[ride_count-1]->GetMemoryUsage();
        UpdateMemory();
    }
    catch(const low_efficiency& e) {
        std::cout << "Couldn't create a ride (low efficiency): " << e.what() << std::endl;
        return;
    }
}

// CheckEfficiency: confere se a criação de uma corrida com o grupo passado como parâmetro satisfaria o critério de eficiência mínima. Retorna true se sim, false caso não
bool Manager::CheckEfficiency(DemandGroup* group) {
    try {
        Ride* aux_ride = new Ride(group, this->min_efficiency);

        // Update de memória máxima auxiliar usada
        this->extra_mem_usage += aux_ride->GetMemoryUsage();
        if(this->extra_mem_usage > this->max_extra_mem_usage) {
            this->max_extra_mem_usage = this->extra_mem_usage;
        }
        this->extra_mem_usage -= aux_ride->GetMemoryUsage();

        delete aux_ride;
        return true;
    }
    catch(const low_efficiency& e) {
        return false;
    }
}

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
    this->group_count = 0;
    CreateDemandGroup();
    this->rides = new Ride*[MAX_GROUPS];
    this->ride_count = 0;

    // Controle de memória
    this->static_mem_usage = 4*sizeof(int) + 4*sizeof(double) + sizeof(float) + this->scaler.GetMemoryUsage() + sizeof(DemandGroup**)+ sizeof(Ride**) + sizeof(DemandGroup*)*MAX_GROUPS + sizeof(Ride*)*MAX_GROUPS;
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
    // Criação da demanda e recuperação da primeira demanda no grupo mais recente ainda não fechado
    Demand* new_demand = new Demand(id, t, ox, oy, dx, dy);                 // nova demanda
    DemandGroup* current_group = demand_groups[this->group_count - 1];      // grupo mais recente
    Demand* dem_in_place = current_group->Get(0);                           // demanda de comparação
    int time_diff = new_demand->GetTime() - dem_in_place->GetTime();        // diferença de tempo entre ambas

    // Update da memória
    this->extra_mem_usage += new_demand->GetMemoryUsage();
    UpdateMemory();

    // Se o critério de tempo compatível, inicia a checagem dos outros critérios
    bool compatible = true;    // sairá do if como false se não for compatível com o grupo atual (por qualquer critério)
    if(abs(time_diff) <= this->delta) {
        // Checagem de distância entre origens e destinos
        for(int i = 0; i < current_group->Size(); i++) {
            double orig_dist = current_group->Get(i)->OriginDistance(*new_demand);
            double dest_dist = current_group->Get(i)->DestinationDistance(*new_demand);

            if(orig_dist > this->origin_max_distance || dest_dist > this->destin_max_distance) {
                compatible = false;
                break;
            }
        }

        // Checagem de eficiência mínima
        if(compatible) {
            current_group->Insert(*new_demand);

            // Se a adição da nova demanda fez o critério de eficiência ficar abaixo do mínimo, ela é removida do grupo atual, é dada como incompatível e é feita a definição da corrida com o grupo atual
            if(!CheckEfficiency(current_group)) {
                current_group->Remove();
                compatible = false;
                MakeRide(current_group);
            }
        }
    }

    // Caso a corrida seja incompatível com o grupo por qualquer critério, cria um novo grupo para inseri-la e retorna
    if(!compatible) {
        try {
            DemandGroup* new_group = CreateDemandGroup();
            new_group->Insert(*new_demand);
        }
        catch(const std::out_of_range& e) {
            return -1;
        }
    }

    return this->group_count - 1;
}

// StartSimulation (durante simulação): começa a executar a simulação, recupera todos os eventos agendados e conclui as corridas. Imprime as informações de cada corrida à medida que são concluídas
void Manager::StartSimulation(std::ofstream& out) {
    out << std::fixed << std::setprecision(2);

    while(1) {
        try {
            // Recuperação do evento
            Event ev = this->scaler.GetNextEvent();
            this->global_time = ev.GetTime();

            // Atualização da memória
            this->extra_mem_usage += ev.GetMemoryUsage();
            UpdateMemory();

            // Processamento do evento
            switch(ev.GetType()) {
                case EventType::RIDESTART:
                    // Recuperação da corrida e início
                    int index_ride = ev.GetID();
                    Ride* ride = this->rides[index_ride];
                    ride->Start();

                    break;

                case EventType::RIDEEND:
                    // Recuperação da corrida associada ao evento
                    int index_ride = ev.GetID();
                    Ride* ride = this->rides[index_ride];
                    ride->MarkDone();

                    // Imprimindo status da corrida
                    out << ride->GetDuration() 
                        << " "
                        << ride->GetDistance()
                        << " "
                        << ride->GetStopAmount();
                    ride->PrintStops(out);
                    out << std::endl;

                    break;
            }
        }
        catch(const std::runtime_error& e) {
            out << "End of events: " << e.what() << std::endl;
            return;
        }
    }
}

// GetStatistics (pós-simulação): calcula e imprime as estatísticas da simulação.
void Manager::GetStatistics(std::ofstream& out) {
    out << std::fixed << std::setprecision(2);
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