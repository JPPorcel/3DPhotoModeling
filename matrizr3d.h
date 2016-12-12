#ifndef MATRIZR3D_H
#define MATRIZR3D_H


#include <iostream>
#include "tuplas.h"
#include "file_ply_stl.h"
#include <vector>
#include <GL/glut.h>
#include <cmath>
#include <opencv2/opencv.hpp>

class MatrizR3D
{

private:
    double ** datos;	// Los valores de la matriz
    int filas;			// Núm. de filas y columnas

public:

    // Constructor sin argumentos (matriz vacia)
    MatrizR3D(void);

    // Constructor de matriz de "d" dimensiones
    MatrizR3D(int eje, double alpha);

    // Constructor de copia
    MatrizR3D(const MatrizR3D & otra);

    // Destructor
    ~MatrizR3D(void);

    // Sobrecarga operador ()
    double & operator () (const int fila, const int col);

    // Sobrecarga operador *
    Tupla3f operator * (const Tupla3f & t);
    cv::Point operator * (const cv::Point & p);

    MatrizR3D & operator = (const MatrizR3D & otra);

    const static int EJE_X = 0;
    const static int EJE_Y = 1;
    const static int EJE_Z = 2;


private:

    // Pide la memoria necesaria
    void ReservaMemoria (void);

    // Libera la memoria ocupada
    void LiberaMemoria (void);

    // Inicializa la matriz
    void Inicializar(int eje, double alpha);

    // Proceso de Copia
    void Copia(const MatrizR3D & otra);
};

#endif // MATRIZR3D_H
