#include "2D_point.hpp"
#include <cmath>

// CONSTRUTOR COMPLETO
Point2D::Point2D(double x, double y) {
    this->x = x;
    this->y = y;
}

// CONSTRUTOR DE CÓPIA
Point2D::Point2D(const Point2D& other) {
    this->x = other.x;
    this->y = other.y;
}

// Sobrecarga de atribuição - copia para este objeto as coordenadas do outro
void Point2D::operator=(const Point2D& other) {
    this->x = other.x;
    this->y = other.y;
}

// GetX: retorna a coordenada deste ponto no eixo X
double Point2D::GetX() {
    return this->x;
}

// GetY: retorna a coordenada deste ponto no eixo Y
double Point2D::GetY() {
    return this->y;
}

// Distance: calcula a distância (double) entre esse e outro ponto passado por referência
double Point2D::Distance(Point2D& other) {
    double dx = this->x - other.GetX();
    double dy = this->y - other.GetY();
    return sqrt((dx*dx) + (dy*dy));
}