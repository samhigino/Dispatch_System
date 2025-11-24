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
        void UpdateMemory();                        // O(1)
        DemandGroup* CreateDemandGroup();           // O(1)
        bool MakeRide(DemandGroup* group);          // O(n)
        bool CheckEfficiency(DemandGroup& group);   // O(n)

        // Controle de memória e depuração
        int static_mem_usage;           // Memória estática usada pelo objeto (imprescindível)
        int extra_mem_usage;            // Memória variável usada pelo objeto (auxiliar)
        int max_extra_mem_usage;        // Memória máxima já usada pelo objeto em algum momento da simulação

    public:
        // Construtor e destrutor
        Manager(int eta, double gamma, double delta, double alpha, double beta, float lambda, int demands);
        ~Manager();

        // Simulação (pré, durante e pós)
        int MakeDemand(int id, double t, double ox, double oy, double dx, double dy);  // Registra uma nova demanda e processa ela
        void StartSimulation(std::ostream& out);                                       // Inicia a simulação e imprime as estatísticas de cada corrida

        // Controle de memória
        int GetStaticMemUsage();    // Retorna a memória imprescindível usada pelo manager
        int GetExtraMemUsage();     // Retorna a memória extra usada pelo manager
        int GetMemoryUsage();       // Retorna o máximo de uso de memória já usado pelo manager
};

#endif