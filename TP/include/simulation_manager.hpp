#ifndef MANAGER_H
#define MANAGER_H
#include "ride.hpp"
#include "demand_group.hpp"
#include "event_scaler.hpp"

const static int MAX_GROUPS = 200;

class Manager {
    private:
        // Parâmetros de simulação
        int veh_capacity;               // eta - Capacidade dos veículos
        double veh_speed;               // gamma - Velocidade dos veículos
        double delta;                   // delta - Intervalo máximo de tempo entre origens de corridas compartilhadas
        double origin_max_distance;     // alpha - Distância máxima entre origem de corridas compartilhadas
        double destin_max_distance;     // beta - Distância máxima entre destino de corridas compartilhadas
        float min_efficiency;           // lambda - Eficiência mínima da corrida compartilhada
        int demand_amount;              // quantas demandas serão feitas

        // Objetos de simulação e variáveis de controle
        EventScaler scaler;                         // Escalonador
        double global_time;                         // Tempo global da simulação
        DemandGroup** demand_groups;                // Grupos de demandas (grupo contém demandas elegíveis para compartilhamento)
        int group_count;                            // Quantidade de grupos de demandas atualmente
        Ride** rides;                               // Corridas geradas com base nos grupos de demandas
        int ride_count;                             // Quantidade de corridas já geradas atualmente
        int demand_count;                           // Quantidade de demandas já recebidas

        // Funções auxiliares (não acessíveis externamente - ver uso em state_manager.cpp)
        void UpdateMemory();
        DemandGroup* CreateDemandGroup(std::ostream& debug);
        void MakeRide(DemandGroup* group, std::ostream& debug);
        bool CheckEfficiency(DemandGroup& group, std::ostream& debug);

        // Controle de memória e depuração
        int static_mem_usage;           // Memória estática usada pelo objeto (imprescindível)
        int extra_mem_usage;            // Memória variável usada pelo objeto (auxiliar)
        int max_extra_mem_usage;        // Memória máxima já usada pelo objeto em algum momento da simulação

    public:
        // Construtor e destrutor
        Manager(int eta, double gamma, double delta, double alpha, double beta, float lambda, int demands);
        ~Manager();

        // Simulação (pré, durante e pós)
        int MakeDemand(int id, double t, double ox, double oy, double dx, double dy, std::ostream& debug);
        void StartSimulation(std::ostream& out, std::ostream& debug);
        void GetStatistics(std::ostream& out);

        // Controle de memória
        int GetStaticMemUsage();
        int GetExtraMemUsage();
        int GetMemoryUsage();
};

#endif