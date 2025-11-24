#include <chrono>
#include <iomanip>
#include "simulation_manager.hpp"

using namespace std;

int main() {
    // Booting
    cout << "Program started. Opening out files.\n";
    ifstream in("tests/input_3.txt", fstream::in);
    ofstream out("output/output.txt", fstream::out);
    ofstream debug("output/debug.txt", fstream::out);

    if(!debug.is_open()) {
        cout << "Debug file did not open." << std::endl;
        return 1;
    }

    if(!in.is_open() || !out.is_open()) {
        cout << "Error opening input or output." << std::endl;
        return 2;
    }

    cout << "Out files opened. Initiating execution.\n\n";

    out << fixed << setprecision(2);
    debug << fixed << setprecision(2);

    // Coleta dos parâmetros de simulação
    debug << "Collecting simulation parameters...\n";
    int eta;            // Capacidade dos veículos
    double gamma;       // Velocidade dos veículos
    double delta;       // Intervalo máximo de tempo entre origens de corridas compartilhadas
    double alpha;       // Distância máxima entre origem de corridas compartilhadas
    double beta;        // Distância máxima entre destino de corridas compartilhadas
    float lambda;       // Eficiência mínima da corrida compartilhada
    int demand_amount;   // Número de demandas da simulação

    in >> eta >> gamma >> delta >> alpha >> beta >> lambda >> demand_amount;
    debug << "Parameters collected:\n\teta: " << eta << "\n\tgamma: " << gamma << "\n\tdelta: " << delta << "\n\talpha: " << alpha << "\n\tbeta: " << beta << "\n\tlambda: " << lambda << "\n\tdemand_amount: " << demand_amount << "\n\n";
    debug.flush();

    // Inicialização do gerente
    debug << "Initializing manager...\n";
    Manager manager(eta, gamma, delta, alpha, beta, lambda, demand_amount);
    debug << "Manager initialized.\n";
    debug.flush();

    // Coleta de dados para criação de demandas (demand_amount vezes)
    debug << "----------------------------------------------------------------\nCOLLECTING DEMANDS (PRE-SIMULATION)...\n----------------------------------------------------------------\n";
    for(int i = 0; i < demand_amount; i++) {
        debug << "\n\t-> Demand " << i << endl;
        int id;
        double time;
        double ox, oy, dx, dy;
        in >> id >> time >> ox >> oy >> dx >> dy;

        manager.MakeDemand(id, time, ox, oy, dx, dy, debug);
        debug << "\tDemand created successfully.\n";
    }
    debug << "\nAll demands collected.\n\n";

    // Simulação
    debug << "----------------------------------------------------------------\nSTARTING SIMULATION...\n----------------------------------------------------------------\n\n";
    manager.StartSimulation(out, debug);
    debug << "\nEnd of simulation.\n";

    debug.close();
    in.close();
    out.close();
    return 0;
}