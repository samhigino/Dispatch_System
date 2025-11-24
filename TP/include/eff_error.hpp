#ifndef EFFICIENCYERROR_H
#define EFFICIENCYERROR_H

#include <stdexcept>
#include <string>

// Exceção para caso de baixa eficiência na criação de uma corrida
class low_efficiency : std::runtime_error {
    public:
        low_efficiency(const std::string& message) throw() : std::runtime_error(message) { };
        virtual const char* what() const throw() {
            return exception::what();
        }
};

#endif