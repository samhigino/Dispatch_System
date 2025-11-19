#include <iostream>
#include "state_manager.hpp"

int main() {
    // Coleta dos parâmetros de simulação
    int eta;            // Capacidade dos veículos
    double gamma;       // VVelocidade dos veículos
    double delta;       // Intervalo máximo de tempo entre origens de corridas compartilhadas
    double alpha;       // Distância máxima entre origem de corridas compartilhadas
    double beta;        // Distância máxima entre destino de corridas compartilhadas
    float lambda;       // Eficiência mínima da corrida compartilhada
    int demand_count;   // Número de demandas da simulação

    std::cin >> eta >> gamma >> delta >> alpha >> beta >> lambda >> demand_count;

    // Inicialização do gerente de corridas

    // Coleta de dadods para criação de demandas (demand_count vezes)
    for(int i = 0; i < demand_count; i++) {
        int id;
        int time;
        double ox, oy, dx, dy;
        std::cin >> id >> time >> ox >> oy >> dx >> dy;
    }
}