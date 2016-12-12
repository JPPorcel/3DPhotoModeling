// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** Juan Pablo Porcel Porcel (C)
// **
// ** Matriz de rotación
// **
// *********************************************************************

#include "matrizr3d.h"

#include <iostream>
using namespace std;

/*********************************************************************/

// Constructor sin argumentos (matriz vacia)
MatrizR3D::MatrizR3D(void)
{
    filas = 0;
    datos = 0;
}


/*********************************************************************/

// Constructor de matriz de 3 dimensiones según el eje y el ańgulo
MatrizR3D::MatrizR3D(int eje, double alpha)
    : filas(3)
{
    Inicializar(eje, alpha);
}


/*********************************************************************/

// Constructor de copia
MatrizR3D :: MatrizR3D(const MatrizR3D & otra)
{
    Copia(otra);
}


/*********************************************************************/

// Sobrecarga del operador = (Recibe una matriz)
MatrizR3D & MatrizR3D ::  operator = (const MatrizR3D & otra)
{
    if (this != &otra) // Evitar autoasignacion
    {
        LiberaMemoria(); // Liberar memoria reservada

        Copia(otra);
    }

        return (*this);
}


/*********************************************************************/

// Destructor
MatrizR3D :: ~MatrizR3D(void)
{
    LiberaMemoria();
}


/*********************************************************************/

// Sobrecarga operador ()
double & MatrizR3D :: operator () (const int fila, const int col)
{
    return datos[fila][col];
}


/*********************************************************************/

// Sobrecarga operador *
Tupla3f MatrizR3D :: operator * (const Tupla3f & t)
{

    Tupla3f tmp;

    for(int f=0; f<filas; f++)
    {
        double a = 0;
        for(int c=0; c<filas; c++)
            a += datos[f][c]*t.coo[c];
        tmp.coo[f] = a;
    }

    return (tmp);
}


// Sobrecarga operador *
cv::Point MatrizR3D :: operator * (const cv::Point & p)
{
    Tupla3f t(p.x, p.y, 0);
    Tupla3f tmp;

    for(int f=0; f<filas; f++)
    {
        double a = 0;
        for(int c=0; c<filas; c++)
            a += datos[f][c]*t.coo[c];
        tmp.coo[f] = a;
    }

    return cv::Point(tmp.coo[0], tmp.coo[1]);
}



/*********************************************************************/
// Métodos privados
/*********************************************************************/

// Pide la memoria necesaria
void MatrizR3D :: ReservaMemoria (void)
{
    datos = new double * [filas];

    for(int i = 0; i < filas; i++)
        datos[i] = new double[filas];
}

/*********************************************************************/

// Libera la memoria ocupada
void MatrizR3D :: LiberaMemoria (void)
{
    if (datos) {
        for(int i = 0; i < filas; i++)
            delete [] datos[i];
        delete [] datos;
    }
}

/*********************************************************************/

// Inicializa la matriz
void MatrizR3D :: Inicializar(int eje, double alpha)
{
    // Reservar memoria
    ReservaMemoria();

    switch(eje)
    {
        case EJE_X:
            datos[0][0] = 1; datos[0][1] = 0;
            datos[0][2] = 0; datos[1][0] = 0;
            datos[1][1] = cos(alpha); datos[1][2] = -sin(alpha);
            datos[2][0] = 0; datos[2][1] = sin(alpha);
            datos[2][2] = cos(alpha);
            break;

        case EJE_Y:
            datos[0][0] = cos(alpha); datos[0][1] = 0;
            datos[0][2] = sin(alpha); datos[1][0] = 0;
            datos[1][1] = 1; datos[1][2] = 0;
            datos[2][0] = -sin(alpha); datos[2][1] = 0;
            datos[2][2] = cos(alpha);
            break;

        case EJE_Z:
            datos[0][0] = cos(alpha); datos[0][1] = -sin(alpha);
            datos[0][2] = 0; datos[1][0] = sin(alpha);
            datos[1][1] = cos(alpha); datos[1][2] = 0;
            datos[2][0] = 0; datos[2][1] = 0;
            datos[2][2] = 1;
            break;
    }
}

/*********************************************************************/

// Proceso de Copia
void MatrizR3D :: Copia(const MatrizR3D & otra)
{
    filas = otra.filas;

    // Reserva de memoria
    ReservaMemoria();

    // Se copian los datos de "otra"
    for(int f=0; f<filas; f++)
        for(int c=0; c<filas; c++)
            datos[f][c] = otra.datos[f][c];
}


/*********************************************************************/
