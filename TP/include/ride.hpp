#ifndef RIDE_H
#define RIDE_H

#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "segment.hpp"
#include "demand_group.hpp"
#include "event_scaler.hpp"

const static int MAX_SEGMENTS = 40;

class Ride {
    private:
        // Atributos gerais
        Stop** stops;
        Segment* segments;
        int stop_amount;
        int segment_amount;
        
        // Atributos de simulação
        bool ongoing;
        bool done;
        double distance;
        double efficiency;
        int start;
        double duration;

        // Controle de memória
        int mem_usage;

    public:
        // Construtor e Destrutor
        Ride(DemandGroup* group, double min_efficiency);
        ~Ride();

        // Operações/Métodos
        void Start();
        void MarkDone();
        void CalculateDuration(double veh_speed);
        double GetEfficiency();
        double GetDistance();
        int GetStart();
        double GetDuration();
        int GetStopAmount();
        void PrintStops(std::ofstream& out);

        // Controle de Memória
        int GetMemoryUsage();
};

#endif