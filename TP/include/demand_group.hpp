#ifndef VECTOR_H
#define VECTOR_H
#include <stdexcept>
#include "demand.hpp"

class DemandGroup {
    private:
        // Atributos gerais
        Demand* group;                  // Grupo de demandas em vetor no heap
        int max_size;                   // Tamanho máximo do grupo
        int item_counter;               // Controle de tamanho do vetor
        
        // Controle de memória
        int mem_usage;                  // Total de memória usada pelo objeto

    public:
        // Construtor e Destrutor
        DemandGroup(int max_size);
        ~DemandGroup();

        // Operações/Métodos
        int Insert(Demand& item);       // Retorna o índice ou -1 se estiver cheio
        Demand* Get(int index);         // Retorna ponteiro para o item no índice passado
        int Size();                     // Retorna o tamanho do vetor
        void Clear();                   // Limpa o vetor

        // Controle de memória: detalhamento em demand_group.cpp
        int GetMemoryUsage();
};

#endif