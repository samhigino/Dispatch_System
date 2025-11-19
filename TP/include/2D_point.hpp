#ifndef POINT_H
#define POINT_H

class Point2D {
    private:
        // Atributos: coordenadas
        double x;
        double y;

    public:
        // Construtores
        Point2D() : Point2D(0.0, 0.0) { };      // Construtor padrão: incializa as coordenadas zeradas
        Point2D(double x, double y);            // Construtor completo
        Point2D(const Point2D& other);          // Construtor de cópia: usa as coordenadas do ponto passado

        // Operações/Métodos
        void operator=(const Point2D& other);   // Sobrecarga de atribuição: copia as coordenadas desse ponto com base em um passado como referência
        double Distance(Point2D& other);        // Retorna a distância entre esse ponto e outro
        double GetX();                          // Retorna X
        double GetY();                          // Retorna Y
};

#endif