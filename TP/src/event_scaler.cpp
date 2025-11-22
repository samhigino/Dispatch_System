#include <stdexcept>
#include "event_scaler.hpp"

//-------------------------------------------------------------------------------
// FUNÇÕES AUXILIARES
//-------------------------------------------------------------------------------

// Retorna o ancestral do nó i
int EventScaler::GetAncestral(int i) {
    return (i - 1) / 2;
}

// Retorna o sucessor à esquerda do nó i
int EventScaler::GetLeftSuccessor(int i) {
    return 2 * i + 1;
}

// Retorna o sucessor à direita do nó i
int EventScaler::GetRightSuccessor(int i) {
    return 2 * i + 2;
}

// HeapifyDown: restaura a propriedade de min-heap a partir de um nó i para baixo
void EventScaler::HeapifyDown(int i) {
    int small = i;          // Assume-se inicialmente que o Evento de menor tempo é i
    int right = GetRightSuccessor(i);
    int left = GetLeftSuccessor(i);

    if(left < this->size && minheap[left].GetTime() < minheap[small].GetTime())
        small = left;

    if(right < this->size && minheap[right].GetTime() < minheap[small].GetTime())
        small = right;

    // Caso um dos filhos seja menor, troca com i e chama recursivamente para o filho
    if(small != i) {
        Event aux(minheap[i]);
        minheap[i] = minheap[small];
        minheap[small] = aux;
        HeapifyDown(small);
    }
    else return;
}

// HeapifyUp: restaura a propriedade de min-heap a partir de um nó i para cima
void EventScaler::HeapifyUp(int i) {
    int anc = GetAncestral(i);

    // Caso o nó seja maior que seu ancestral, troca e chama recursivamente para o ancestral
    if(minheap[i].GetTime() > minheap[anc].GetTime()) {
        Event aux(minheap[i]);
        minheap[i] = minheap[anc];
        minheap[anc] = aux;
        HeapifyUp(anc);
    }
    else return;
}

//-------------------------------------------------------------------------------
// CONSTRUTOR
//-------------------------------------------------------------------------------

// Construtor: inicializa automaticamente o vetor do min-heap e os outros atributos de acordo
EventScaler::EventScaler() {
    this->size = 0;

    // Controle de memória: todo Evento ocupa a mesma quantidade de memória
    this->mem_usage = sizeof(int) + minheap[0].GetMemoryUsage()*MAX_HEAP_SIZE;
}

//-------------------------------------------------------------------------------
// OPERAÇÕES/MÉTODOS
//-------------------------------------------------------------------------------

// ScheduleEvent: agenda um evento - insere-o no min-heap e organiza o min-heap
void EventScaler::ScheduleEvent(int id, double time, EventType type) {
    if(this->size == MAX_HEAP_SIZE) {
        throw std::runtime_error("Can't schedule event: max size reached.");
    }
    else {
        minheap[size] = Event(id, time, type);
        this->size++;
        HeapifyUp(size-1);
    }
}

// GetNextEvent: recupera o próximo evento na fila de prioridade
Event& EventScaler::GetNextEvent() {
    // Caso de min-heap vazio
    if(this->size == 0) {
        throw std::runtime_error("Can't recover event: min-heap empty.");
    }
    // Caso trivial: somente 1 evento
    if(this->size == 1) {
        this->size--;
        return minheap[0];
    }

    // Armazena o evento que irá ser retornado, heapify antes de retornar
    this->nextevent = minheap[0];
    minheap[0] = minheap[this->size-1];
    this->size--;
    HeapifyDown(0);
    return nextevent;
}

//-------------------------------------------------------------------------------
// CONTROLE DE MEMÓRIA
//-------------------------------------------------------------------------------

int EventScaler::GetMemoryUsage() {
    return this->mem_usage;
}