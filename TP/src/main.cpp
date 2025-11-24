#include "simulation_manager.hpp"

int main() {
    // Booting
    std::cout << std::fixed << std::setprecision(2);

    // Coleta dos parâmetros de simulação
    int eta;            // Capacidade dos veículos
    double gamma;       // Velocidade dos veículos
    double delta;       // Intervalo máximo de tempo entre origens de corridas compartilhadas
    double alpha;       // Distância máxima entre origem de corridas compartilhadas
    double beta;        // Distância máxima entre destino de corridas compartilhadas
    float lambda;       // Eficiência mínima da corrida compartilhada
    int demand_amount;  // Número de demandas da simulação

    std::cin >> eta >> gamma >> delta >> alpha >> beta >> lambda >> demand_amount;

    // Inicialização do gerente
    Manager manager(eta, gamma, delta, alpha, beta, lambda, demand_amount);

    // Coleta de dados para criação de demandas (demand_amount vezes)
    for(int i = 0; i < demand_amount; i++) {
        int id;
        double time;
        double ox, oy, dx, dy;
        std::cin >> id >> time >> ox >> oy >> dx >> dy;

        manager.MakeDemand(id, time, ox, oy, dx, dy);
    }

    // Simulação
    manager.StartSimulation(std::cout);

    return 0;
}