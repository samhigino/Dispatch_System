#ifndef EVENT_H
#define EVENT_H

enum class EventType {
    RIDESTART,
    RIDEEND
};

class Event {
    private:
        // Atributos
        int id;             // Identificador do evento - referência à corrida associada (eventos da mesma corrida tem mesmo id)
        double time;        // Marcador de tempo do evento
        EventType type;     // Tipo de evento (início ou fim de uma corrida)

        // Controle de memória
        int mem_usage;

    public:
        // Construtores
        Event() : Event(-1, -1, EventType::RIDESTART) { };  // Constrturor padrão
        Event(int id, double time, EventType type);         // Construtor regular
        Event(Event& other);                                // Construtor de cópia

        // Métodos
        int GetID();
        double GetTime();
        EventType GetType();
        void operator=(const Event& other);     // Sobrecarga de atribuição para cópias

        // Controle de memória
        int GetMemoryUsage();
};

#endif