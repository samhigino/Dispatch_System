#ifndef EVENTSCALER_H
#define EVENTSCALER_H
#include "event.hpp"

const static int MAX_HEAP_SIZE = 511;   // 8 níveis

class EventScaler {
    private:
        // Atributos
        Event minheap[511];
        Event nextevent;
        int size;

        // Funções auxiliares
        int GetAncestral(int i);        // Retorna o ancestral de um nó 
        int GetLeftSuccessor(int i);    // Retorna o sucessor à esquerda de um nó
        int GetRightSuccessor(int i);   // Retorna o sucessor à direita de um nó
        void HeapifyDown(int i);        // Restaura a propriedade de min-heap a partir de um nó i para baixo
        void HeapifyUp(int i);          // Restaura a propriedade de min-heap a partir de um nó i para cima

        // Controle de memória
        int mem_usage;

    public:
        // Construtor e Destrutor
        EventScaler();

        // Operações/Métodos
        void ScheduleEvent(int id, double time, EventType type);
        Event& GetNextEvent();
        int GetSize();
        void Print(std::ostream& debug);

        // Controle de memória
        int GetMemoryUsage();
};

#endif