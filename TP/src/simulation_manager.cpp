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
DemandGroup* Manager::CreateDemandGroup(std::ostream& debug) {
    // Criação do grupo
    debug << "\t\t\t\tCreating demand group...\n";
    if(group_count < MAX_GROUPS) {
        debug << "\t\t\t\t\tWe can create a new group. This is fine.\n";
        debug << "\t\t\t\t\tCreating new group in demand_groups[" << group_count << "] passing veh_capacity = " << veh_capacity << " to it. Now evaluating the creation of the group.\n" ;
        this->demand_groups[group_count] = new DemandGroup(this->veh_capacity);
        this->group_count++;
        debug << "\t\t\t\t\tGroup created. group_count was also updated to " << group_count << ".\n";
    }
    else {
        debug << "\t\t\t\t\tIMPOSSIBLE TO CREATE NEW DEMAND GROUP. MAX GROUPS REACHED.\n";
        throw std::out_of_range("Max demand groups reached. Can't create a new group.");
    }

    // Update de memória
    debug << "\t\t\t\t\tUpdating memory...\n";
    this->extra_mem_usage += demand_groups[group_count-1]->GetMemoryUsage();
    UpdateMemory();
    debug << "\t\t\t\t\tMemory updated.\n";

    debug << "\t\t\t\t\tGroup created.\n";
    return demand_groups[this->group_count - 1];
}

// MakeRide: cria uma nova corrida baseada no grupo passado como parâmetro e a insere no vetor de corridas
void Manager::MakeRide(DemandGroup* group, std::ostream& debug) {
    debug << "\n\t\t\t\tMaking a new Ride now...\n";
    if(ride_count == MAX_GROUPS) {
        debug << "\t\t\t\tAlready have max groups. Can't create a new one.\n\n";
        debug.flush();
        throw std::out_of_range("Can't create a new ride: max number of rides reached.");
    }

    try {
        // Criação da corrida
        debug << "\t\t\t\t\tCreating the ride and calculating its duration. This will be ride number " << ride_count << ".\n";
        debug.flush();
        this->rides[ride_count] = new Ride(*group, this->min_efficiency, debug);
        this->rides[ride_count]->CalculateDuration(this->veh_speed);
        debug << "\t\t\t\t\tRide made successfully.\n";
        double ride_start = this->rides[ride_count]->GetStart();
        double ride_end = ride_start + this->rides[ride_count]->GetDuration();
        debug << "\t\t\t\t\tIt starts in " << ride_start << " and ends in " << ride_end << std::endl;

        // Agendamento dos eventos
        debug << "\t\t\t\t\tScheduling events now.\n";
        this->scaler.ScheduleEvent(ride_count, ride_start, EventType::RIDESTART);
        this->scaler.ScheduleEvent(ride_count, ride_end, EventType::RIDEEND);
        ride_count++;
        debug << "\t\t\t\t\tEvents scheduled and ride_count was updated. There are now " << scaler.GetSize() << " events scheduled.\n";
        this->scaler.Print(debug);

        // Update de memória
        debug << "\t\t\t\t\tUpdating memory...\n";
        this->extra_mem_usage += rides[ride_count-1]->GetMemoryUsage();
        UpdateMemory();
        debug << "\t\t\t\t\tMemory updated. Everything was fine.\n\n";
        debug.flush();
    }
    catch(const low_efficiency& e) {
        debug << "\t\t\t\t\tCouldn't create a ride (low efficiency): " << e.what() << std::endl << std::endl;
        debug.flush();
        return;
    }
}

// CheckEfficiency: confere se a criação de uma corrida com o grupo passado como parâmetro satisfaria o critério de eficiência mínima. Retorna true se sim, false caso não
bool Manager::CheckEfficiency(DemandGroup& group, std::ostream& debug) {
    try {
        debug << "\n\t\t\t\t\tChecking for efficiency of group...\n\t\t\t\t\tCreating aux_ride.\n";
        debug.flush();
        Ride* aux_ride = new Ride(group, this->min_efficiency, debug);

        // Update de memória máxima auxiliar usada
        debug << "\t\t\t\t\tUpdating memory usage.\n";
        this->extra_mem_usage += aux_ride->GetMemoryUsage();
        UpdateMemory();
        this->extra_mem_usage -= aux_ride->GetMemoryUsage();

        debug << "\t\t\t\t\tWas efficient enough. Now deleting aux_ride.\n\n";
        debug.flush();
        delete aux_ride;
        return true;
    }
    catch(const low_efficiency& e) {
        debug << "\t\t\t\t\tWas not efficient enough.\n\n";
        debug.flush();
        return false;
    }
}

//-------------------------------------------------------------------------------
// CONSTRUTOR E DESTRUTOR
//-------------------------------------------------------------------------------

// CONSTRUTOR: incializa todas os parâmetros de simulação com os parâmetros, tempo global como 0, memória estática e extra como apropriado
Manager::Manager(int eta, double gamma, double delta, double alpha, double beta, float lambda, int demands) {
    // Parametrização da simulação
    this->veh_capacity = eta;
    this->veh_speed = gamma;
    this->delta = delta;
    this->origin_max_distance = alpha;
    this->destin_max_distance = beta;
    this->min_efficiency = lambda;
    this->demand_amount = demands;

    // Objetos de simulação e variáveis de controle
    this->global_time = 0;
    this->demand_groups = new DemandGroup*[MAX_GROUPS];
    this->group_count = 0;
    this->rides = new Ride*[MAX_GROUPS];
    this->ride_count = 0;
    for(int i = 0; i < MAX_GROUPS; i++) {
        this->demand_groups[i] = nullptr;
        this->rides[i] = nullptr;
    }
    CreateDemandGroup(std::cout);
    this->demand_count = 0;

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
int Manager::MakeDemand(int id, double t, double ox, double oy, double dx, double dy, std::ostream& debug) {
    this->demand_count++;
    debug << "\t\tCreating demand with id: " << id << "; time: " << t << "; ox: " << ox << "; oy: " << oy << "; dx: " << dx << "; dy: " << dy << std::endl;

    // Caso trivial: primeira demanda da simulação
    debug << "\t\t\tTrivial case?\n";
    debug.flush();
    if(this->group_count == 1 && this->demand_groups[0]->Size() == 0) {
        debug << "\t\t\tTrivial case. Just insert it in group 0.\n";
        Demand* first_demand = new Demand(id, t, ox, oy, dx, dy);
        this->demand_groups[0]->Insert(*first_demand);
        return 0;
    }
    debug << "\t\t\tNope.\n";
    debug.flush();

    // Criação da demanda e recuperação da primeira demanda no grupo mais recente ainda não fechado
    debug << "\t\t\tCurrent group is: " << group_count - 1 << ". Let's try to insert it in there.\n";
    Demand* new_demand = new Demand(id, t, ox, oy, dx, dy);                     // nova demanda
    DemandGroup* current_group = this->demand_groups[this->group_count - 1];    // grupo mais recente
    Demand* dem_in_place = current_group->Get(0);                               // demanda de comparação
    int time_diff = new_demand->GetTime() - dem_in_place->GetTime();            // diferença de tempo entre ambas
    debug << "\t\t\tThe time difference here is " << time_diff << ".\n";
    debug.flush();

    // Update da memória
    debug << "\t\t\tUpdating memory.\n";
    this->extra_mem_usage += new_demand->GetMemoryUsage();
    UpdateMemory();
    debug << "\t\t\tMemory updated.\n";
    debug.flush();

    // Início da checagem de critérios de compatibilidade
    bool compatible = true;    // sairá do if como false se não for compatível com o grupo atual (por qualquer critério)
    debug << "\t\t\tChecking compatibility now...\n";
    debug.flush();
    
    // Checagem de tamanho do grupo atual
    if(current_group->IsFull()) {
        debug << "\t\t\t\tIncompatible by group size (group is full). Let's make the ride.\n";
        compatible = false;
    }

    // Checagem de tempo
    if(compatible && abs(time_diff) > this->delta) {
        debug << "\t\t\t\tIncompatible by time difference (delta = " << this->delta << ").\n";
        compatible = false;
    }

    // Checagem de distância entre origens e destinos
    if(compatible) {
        for(int i = 0; i < current_group->Size(); i++) {
            double orig_dist = current_group->Get(i)->OriginDistance(*new_demand);
            double dest_dist = current_group->Get(i)->DestinationDistance(*new_demand);

            if(orig_dist > this->origin_max_distance || dest_dist > this->destin_max_distance) {
                debug << "\t\t\t\tIncompatible by distance with demand id " << current_group->Get(i)->GetID() << " (Demand number " << i << " in this group).\n\t\t\t\t  Origin distance: " << orig_dist << ";\n\t\t\t\t  Destination distance: " << dest_dist << ".\n";
                debug.flush();
                compatible = false;
                break;
            }
            debug << "\t\t\t\tCompatible by distance with demand id " << current_group->Get(i)->GetID() << " (Demand number " << i << " in this group).\n\t\t\t\t  Origin distance: " << orig_dist << ";\n\t\t\t\t  Destination distance: " << dest_dist << ".\n";
            debug.flush();
        }
    }

    // Checagem de eficiência mínima
    if(compatible) {
        debug << "\t\t\t\tTrying efficiency check now...\n";
        debug.flush();
        current_group->Insert(*new_demand);
        debug << "\t\t\t\tDemand inserted in the group for efficiency check.\n";
        debug.flush();

        // Se a adição da nova demanda fez o critério de eficiência ficar abaixo do mínimo, ela é removida do grupo atual, é dada como incompatível e é feita a definição da corrida com o grupo atual
        if(!CheckEfficiency(*current_group, debug)) {
            debug << "\t\t\t\tIncompatible by efficiency. Let's remove it from the group and make the ride.\n";
            debug.flush();
            current_group->Remove();
            compatible = false;
        }
    }

    // Caso a demanda seja incompatível com o grupo por qualquer critério, finaliza a definição da corrida do grupo atual e cria um novo grupo para inseri-la
    if(!compatible) {
        try {
            MakeRide(current_group, debug);
            debug << "\t\t\tThe demand was incompatible. Creating a new group for the demand now...\n";
            debug.flush();
            DemandGroup* new_group = CreateDemandGroup(debug);
            new_group->Insert(*new_demand);
        }
        catch(const std::out_of_range& e) {
            debug << "\t\t\tCouldn't create a new group for the demand: " << e.what() << std::endl;
            debug.flush();
            return -1;
        }
    }
    debug << "\t\t\tDemand inserted successfully in group " << group_count - 1 << ".\n";
    debug.flush();

    // Se esta é a úlima demanda, concluir a definição da corrida de seu grupo (não haverão outras inseridas)
    if(this->demand_count == this->demand_amount) {
        debug << "\t\t\tThis was the last demand. Making ride for its group (" << (this-group_count)-1 << ") now...\n";
        DemandGroup* last_group = this->demand_groups[this->group_count - 1];
        MakeRide(last_group, debug);
    }

    return this->group_count - 1;
}

// StartSimulation (durante simulação): começa a executar a simulação, recupera todos os eventos agendados e conclui as corridas. Imprime as informações de cada corrida à medida que são concluídas
void Manager::StartSimulation(std::ostream& out, std::ostream& debug) {
    debug << "Starting simulation now...\n";
    if(this->ride_count == this->group_count) {
        debug << "\tNumber of groups: " << this->group_count << std::endl;
        debug << "\tNumber of rides: " << this->ride_count << std::endl;
        debug << "Here's how the demands were (should've been) merged together: \n";
        for(int i = 0; i < this->group_count; i++) {
            debug << "\tRide " << i << ": demands ";
            int current_size = this->demand_groups[i]->Size();
            for(int j = 0; j <= current_size - 1; j++) {
                if(j != current_size - 1) {
                    debug << this->demand_groups[i]->Get(j)->GetID() << " and ";
                }
                else {
                    debug << this->demand_groups[i]->Get(j)->GetID() << "\n";
                }
            }
            debug.flush();
        }
    }
    else {
        debug << "We don't have as many rides as we have groups. Something went wrong.\n";
        debug << "\tNumber of groups: " << this->group_count << std::endl;
        debug << "\tNumber of rides: " << this->ride_count << std::endl;
        debug.flush();
        throw std::logic_error("Unequal amount of rides and groups.");
    }
    debug << "The scaler looks like this before starting the event processing:\n";
    this->scaler.Print(debug);
    debug << "Beginning event processing loop now...\n\n";
    debug.flush();

    while(1) {
        try {
            // Recuperação do evento
            debug << "\tRecovering event...\n";
            Event ev = this->scaler.GetNextEvent();
            debug << "\tEvent recovered, scaler now:\n";
            // this->scaler.Print(debug);
            this->global_time = ev.GetTime();

            // Atualização da memória
            this->extra_mem_usage += ev.GetMemoryUsage();
            UpdateMemory();

            // Processamento do evento
            switch(ev.GetType()) {
                case EventType::RIDESTART: {
                    // Recuperação da corrida e início
                    debug << "\tThis is a RIDESTART for ride " << ev.GetID() << ", so there's nothing to be done here.\n";
                    int index_ride = ev.GetID();
                    Ride* ride = this->rides[index_ride];
                    ride->Start();

                    break;
                }

                case EventType::RIDEEND: {
                    // Recuperação da corrida associada ao evento
                    debug << "\tThis is a RIDEEND for ride " << ev.GetID() << ". Marking it as done and printing its status now.\n";
                    int index_ride = ev.GetID();
                    Ride* ride = this->rides[index_ride];
                    ride->MarkDone();

                    // Imprimindo status da corrida
                    out << ride->GetEnd()
                        << " "
                        << ride->GetDistance()
                        << " "
                        << ride->GetStopAmount();
                    ride->PrintStops(out);
                    out << std::endl;

                    debug << "\t\tConclusion time: " << ride->GetEnd()
                        << ";\n\t\tDistance: "
                        << ride->GetDistance()
                        << ";\n\t\tNumber of stops: "
                        << ride->GetStopAmount()
                        << "\n\t\tStops: ";
                    ride->PrintStops(debug);
                    debug << std::endl;

                    break;
                }
            }
        }
        catch(const std::runtime_error& e) {
            debug << "End of events: " << e.what() << std::endl;
            return;
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