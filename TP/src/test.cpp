#include <chrono>
#include <iomanip>
#include "simulation_manager.hpp"

using namespace std;

int main() {
    // Booting
    cout << "Program started. Opening out files.\n";
    ifstream in("tests/input_1.txt", fstream::in);
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
    int demand_count;   // Número de demandas da simulação

    in >> eta >> gamma >> delta >> alpha >> beta >> lambda >> demand_count;
    debug << "Parameters collected:\n\teta: " << eta << "\n\tgamma: " << gamma << "\n\tdelta: " << delta << "\n\talpha: " << alpha << "\n\tbeta: " << beta << "\n\tlambda: " << lambda << "\n\tdemand_count: " << demand_count << "\n\n";
    debug.flush();

    // Inicialização do gerente
    debug << "Initializing manager...\n";
    Manager manager(eta, gamma, delta, alpha, beta, lambda);
    debug << "Manager initialized.\n\n";

    // Coleta de dados para criação de demandas (demand_count vezes)
    debug << "Collecting demands...\n";
    for(int i = 0; i < demand_count; i++) {
        debug << "\t-> Demand " << i << endl;
        int id;
        int time;
        double ox, oy, dx, dy;
        in >> id >> time >> ox >> oy >> dx >> dy;
        debug << "\t\tMaking demand with id: " << id << "; time: " << time << "; ox: " << ox << "; oy: " << oy << "; dx: " << dx << "; dy: " << dy << endl;

        manager.MakeDemand(id, time, ox, oy, dx, dy);
        debug << "\t\tDemand created successfully.\n";
    }
    debug << "\nAll demands collected. Starting simulation...\n";

    // Simulação
    manager.StartSimulation(out);
    debug << "End of simulation.\n";

    debug.close();
    in.close();
    out.close();
    return 0;
}