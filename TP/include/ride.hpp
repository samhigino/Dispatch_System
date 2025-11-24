#ifndef RIDE_H
#define RIDE_H

#include <stdexcept>
#include <iomanip>
#include <fstream>
#include "segment.hpp"
#include "demand_group.hpp"
#include "event_scaler.hpp"

const static int MAX_SEGMENTS = 40;

class Ride {
    private:
        // Atributos gerais
        Stop** stops;           // Vetor de paradas da corrida
        Segment* segments;      // Vetor de segmentos da corrida
        int stop_amount;        // Quantidade de paradas da corrida
        int segment_amount;     // Quantidade de segmentos da corrida
        
        // Atributos de simulação
        bool ongoing;           // Marca se a corrida está em andamento
        bool done;              // Marca se a corrida foi concluída
        double distance;        // Distância total da corrida
        double efficiency;      // Eficiência da corrida
        double start;           // Tempo do início da corrida
        double duration;        // Duração da corrida
        double end;             // Tempo do fim da corrida

        // Controle de memória
        int mem_usage;

    public:
        // Construtor e Destrutor
        Ride(DemandGroup& group, double min_efficiency);    // Construtor: inicializa as paradas, segmentos e outros atributos com base em um grupo de demandas. Lança low_efficiency caso a eficiência não seja atingida
        ~Ride();                                            // Destrutor: apaga o conteúdo dos vetores

        // Operações/Métodos
        void Start();                                   // Assinala início desta corrida
        void MarkDone();                                // Assinala conclusão desta corrida
        void CalculateDuration(double veh_speed);       // Calcula a duração desta corrida com base na velocidade dos veículos
        void PrintStops(std::ostream& out);             // Imprime a coordenada das paradas em ordem

        // Getters
        double GetEfficiency();
        double GetDistance();
        double GetStart();
        double GetDuration();
        double GetEnd();
        int GetStopAmount();

        // Controle de Memória
        int GetMemoryUsage();
};

#endif