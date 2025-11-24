#ifndef DEMAND_H
#define DEMAND_H
#include "2D_point.hpp"

class Demand {
    private:
        // Atributos gerais
        int id;                 // Identificador
        double time;            // Marcador de tempo de solicitação da demanda
        Point2D origin;         // Ponto de origem
        Point2D destination;    // Ponto de destino

        // Controle de memória
        int mem_usage;          // Quantidade de memória consumida pelo objeto

    public:
        // Construtores
        Demand() : Demand(-1, -1, 0.0, 0.0, 0.0, 0.0) { };                      // Demanda "nula" (por definição)
        Demand(int id, double t, double ox, double oy, double dx, double dy);   // Construtor completo
        Demand(const Demand& other);                                            // Construtor de cópia

        // Getters para acesso aos atributos
        int GetID();                                    // Retorna o id da demanda
        double GetTime();                               // Retorna o tempo de solicitação
        Point2D& GetOrigin();                           // Retorna referência para o ponto de origem
        Point2D& GetDestination();                      // Retorna referência para o ponto de destino
        double OriginDistance(Demand& other);           // Calcula distância entre origens desta demanda e outra demanda
        double DestinationDistance(Demand& other);      // Calcula distância entre destinos desta demanda e outra demanda
        double GetDistance();                           // Calcula a distância entre a origem e o destino desta demanda
        void operator=(const Demand& other);            // Sobrecarga de atribuição: copia os atributos desta demanda com base em outra

        // Controle de memória
        int GetMemoryUsage();           // Retorna a quantidade de memória usada pelo objeto
};

#endif