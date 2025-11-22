#ifndef STOP_H
#define STOP_H
#include "demand.hpp"

enum class StopType {
    PICKUP,
    DROPOFF
};

class Stop {
    private:
        // Atributos gerais
        StopType type;          // Tipo de parada: coleta ou desembarque
        int demand_id;          // ID da demanda associada à parada
        Point2D stop;           // Ponto da parada (mesmo da demanda)

        // Controle de memória
        int static_mem_usage;   // Quantidade de memória consumida pelo objeto
    
    public:
        // Construtor
        Stop(Demand& demand, StopType type);

        // Operações/Métodos
        Point2D& GetPoint();                // Retorna referência para o ponto da parada
        StopType GetType();                 // Retorna o tipo desta parada
        double Distance(Stop& other);       // Retorna a distância entre esta parada e outra

        // Controle de memória
        int GetMemoryUsage();               // Retorna a quantidade de memória usada pelo objeto
};

#endif