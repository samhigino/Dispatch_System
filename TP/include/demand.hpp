#ifndef DEMAND_H
#define DEMAND_H
#include "2D_point.hpp"

enum class State { 
    DEMANDED,
    INDIVIDUAL,
    SHARED,
    COMPLETE
};

class Demand {
    private:
        // Atributos gerais
        int id;                 // Identificador
        int time;               // Marcador de tempo de solicitação da demanda
        Point2D origin;         // Ponto de origem
        Point2D destination;    // Ponto de destino
        State state;            // Estado da demanda: solicitada, individual, compartilhada ou completa

        // Controle de memória
        int mem_usage;          // Quantidade de memória consumida pelo objeto

    public:
        // Construtores
        Demand() : Demand(-1, -1, 0.0, 0.0, 0.0, 0.0) { };                  // Demanda "nula" (por definição)
        Demand(int id, int t, double ox, double oy, double dx, double dy);  // Construtor completo

        // Getters para acesso aos atributos
        int GetID();                                    // Retorna o id da demanda
        int GetTime();                                  // Retorna o tempo de solicitação
        Point2D& GetOrigin();                           // Retorna referência para o ponto de origem
        Point2D& GetDestination();                      // Retorna referência para o ponto de destino
        double GetOriginDistance(Demand& other);        // Calcula distância entre origens desta demanda e outra demanda
        double GetDestinationDistance(Demand& other);   // Calcula distância entre destinos desta demanda e outra demanda

        // Controle de memória
        int GetMemoryUsage();           // Retorna a quantidade de memória usada pelo objeto
};

#endif