#ifndef MATRIZ4F_H
#define MATRIZ4F_H


#include <iostream>
#include "tuplas.h"
#include <vector>
#include <cmath>


class Matriz4f
{

private:
    double ** datos;	// Los valores de la matriz
    int filas;			// Núm. de filas y columnas

public:

    // Constructor sin argumentos (matriz vacia)
    Matriz4f(void);

    Matriz4f(float * pose);

    // Constructor de matriz de "d" dimensiones
    Matriz4f(char c, double alpha, Tupla3f vector = Tupla3f(0,0,0));

    Matriz4f(char c, std::vector<double> param);

    Matriz4f(std::vector<double> param);

    // Constructor de copia
    Matriz4f(const Matriz4f & otra);

    // Destructor
    ~Matriz4f(void);

    // Sobrecarga operador ()
    double & operator () (const int fila, const int col);

    Matriz4f operator * (const Matriz4f &m);
    Tupla3f operator * (Tupla3f &t);

    Matriz4f & operator = (const Matriz4f & otra);


private:

    // Pide la memoria necesaria
    void ReservaMemoria (void);

    // Libera la memoria ocupada
    void LiberaMemoria (void);

    // Inicializa la matriz
    void Inicializar(char c, double alpha, Tupla3f vector = Tupla3f(0,0,0));

    void Inicializar(char c, std::vector<double> param);

    void Inicializar(std::vector<double> param);

    void Inicializar(float * pose);

    // Proceso de Copia
    void Copia(const Matriz4f & otra);
};

#endif // MATRIZ4F_H
