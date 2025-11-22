#include "simulation_manager.hpp"

int main() {
    // Booting
    std::cout << "Program started.\n";
    std::ifstream in("tests/input_1.txt", std::fstream::in);
    std::ofstream out("output/output.txt", std::fstream::out);
    std::ofstream debug("output/debug.txt", std::fstream::out);

    if(!debug.is_open()) {
        std::cout << "Debug file did not open." << std::endl;
        return 1;
    }

    if(!in.is_open() || !out.is_open()) {
        debug << "Error opening input or output." << std::endl;
        return 2;
    }

    out << std::fixed << std::setprecision(2);
    debug << std::fixed << std::setprecision(2);

    // Coleta dos parâmetros de simulação
    int eta;            // Capacidade dos veículos
    double gamma;       // Velocidade dos veículos
    double delta;       // Intervalo máximo de tempo entre origens de corridas compartilhadas
    double alpha;       // Distância máxima entre origem de corridas compartilhadas
    double beta;        // Distância máxima entre destino de corridas compartilhadas
    float lambda;       // Eficiência mínima da corrida compartilhada
    int demand_count;   // Número de demandas da simulação

    in >> eta >> gamma >> delta >> alpha >> beta >> lambda >> demand_count;

    // Inicialização do gerente
    Manager manager(eta, gamma, delta, alpha, beta, lambda);

    // Coleta de dados para criação de demandas (demand_count vezes)
    for(int i = 0; i < demand_count; i++) {
        int id;
        int time;
        double ox, oy, dx, dy;
        in >> id >> time >> ox >> oy >> dx >> dy;

        manager.MakeDemand(id, time, ox, oy, dx, dy);
    }

    // Simulação
    manager.StartSimulation(out);

    return 0;
}