#ifndef SEGMENT_H
#define SEGMENT_H
#include "stop.hpp"

enum class SegmentType {
    PICKUP,
    TRAVEL,
    DROPOFF
};

class Segment {
    private:
        // Atributos gerais
        SegmentType type;               // Tipo de segmento: coleta (entre duas origens), deslocamento (entre uma origem e um destino) ou entrega (entre dois destinos)
        Stop* beg;                      // Início do segmento: ponteiro para uma parada
        Stop* end;                      // Fim do segmento: ponteiro para uma parada

        // Atributos de simulação
        double total_distance;          // Distância total do segmento
        bool complete;                  // Marca se o trecho foi completo durante a simulação ou não

        // Controle de memória
        int static_mem_usage;           // Quantidade de memória consumida pelo objeto

    public:
        // Construtores e Destrutor
        Segment();                      // Construtor padrão pra um segmento "nulo" (por definição)
        Segment(Stop& beg, Stop& end);  // Construtor da classe: inicializa os ponteiros referenciando as paradas passadas como parâmetro e formaliza o tipo de segmento com base nas paradas
        ~Segment();                     // Destrutor: apaga as paradas associadas

        // Operações/Métodos
        void operator=(Segment& other);         // Sobrecarga de atribuição: copia as paradas e atualiza os outros atributos
        void MarkComplete();                    // Marca o segmento como completo
        double GetDistance();                   // Retorna o comprimento do segmento
        SegmentType GetType();                  // Retorna o tipo do segmento

        // Controle de memória
        int GetMemoryUsage();           // Retorna a quantidade de memória usada pelo objeto
};

#endif