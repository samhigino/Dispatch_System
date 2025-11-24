#include <iostream>
#include "demand_group.hpp"

// CONSTRUTOR: inicializa o contador como 0 e cria o grupo com o tamanho máximo passado
DemandGroup::DemandGroup(int max_size) : item_counter(0) {
    this->max_size = max_size;
    this->group = new Demand[max_size];

    // Controle de memória
    this->mem_usage = 4*sizeof(int) + sizeof(Demand*) + sizeof(Demand)*max_size;
}

// DESTRUTOR: apaga todas as demandas alocadas dinamicamente
DemandGroup::~DemandGroup() {
    delete[] this->group;
}

// Insert: insere um item caso o grupo já não esteja cheio e retorna o índice onde foi inserido
int DemandGroup::Insert(Demand& item) {
    if(this->item_counter >= this->max_size) {
        throw std::out_of_range("DemandGroup: trying to insert in a full group.");
    }
    else {
        this->group[this->item_counter] = item;
        this->item_counter++;
        return this->item_counter - 1;
    }
}

// Remove: remove o item mais recente do grupo e retorna o índice onde foi removido
int DemandGroup::Remove() {
    if(this->item_counter == 0) {
        return -1; // Grupo vazio
    }
    else {
        this->item_counter--;
        return this->item_counter;
    }
}

// Get: retorna um ponteiro para a Demanda alocada no índice passado
Demand* DemandGroup::Get(int index) {
    // Lança uma exceção caso a posição seja inacessível
    if(index < 0 || index >= this->item_counter) {
        throw std::out_of_range("DemandGroup: inaccessible position");
    }

    // Retorna caso seja acessível
    return &this->group[index];
}

// Size: retorna quantas Demandas há no grupo
int DemandGroup::Size() {
    return this->item_counter;
}

// IsFull: retorna se o grupo está cheio
bool DemandGroup::IsFull() {
    return this->item_counter == this->max_size;
}

// Clear: apaga todas as demandas e reinicia o contador
void DemandGroup::Clear() {
    this->item_counter = 0;
}

// GetMemUsage: retorna quanto de memória este objeto consome, incluindo as demandas alocadas dinamicamente
int DemandGroup::GetMemoryUsage() {
    return this->mem_usage;
}