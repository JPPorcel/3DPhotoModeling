#include "matriz4f.h"

// Constructor sin argumentos (matriz vacia)
Matriz4f::Matriz4f(void)
    : filas(4)
{
    ReservaMemoria();
}


// Constructor
Matriz4f::Matriz4f(float * pose)
    : filas(4)
{
    Inicializar(pose);
}


/*********************************************************************/

// Constructor de matriz de 4 dimensiones según el eje y el ańgulo
Matriz4f::Matriz4f(char c, double alpha, Tupla3f vector)
    : filas(4)
{
    Inicializar(c, alpha, vector);
}


/*********************************************************************/

// Constructor de matriz de 4 dimensiones según transformación
Matriz4f::Matriz4f(char c, std::vector<double> param)
: filas(4)
{
    Inicializar(c, param);
}


/*********************************************************************/

// Constructor de matriz de 4 dimensiones dados los 16 parámetros
Matriz4f::Matriz4f(std::vector<double> param)
: filas(4)
{
    Inicializar(param);
}


/*********************************************************************/

// Constructor de copia
Matriz4f :: Matriz4f(const Matriz4f & otra)
{
    Copia(otra);
}


/*********************************************************************/

// Sobrecarga del operador = (Recibe una matriz)
Matriz4f & Matriz4f ::  operator = (const Matriz4f & otra)
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
Matriz4f :: ~Matriz4f(void)
{
    LiberaMemoria();
}


/*********************************************************************/

// Sobrecarga operador ()
double & Matriz4f :: operator () (const int fila, const int col)
{
    return datos[fila][col];
}


/*********************************************************************/

Matriz4f Matriz4f::operator * (const Matriz4f &m)
{
    Matriz4f resultado;
    for (int fila = 0; fila < filas; fila++)
    {
        for (int col = 0; col < filas; col++)
        {
            resultado.datos[col][fila] = 0.0;
            for (int k = 0; k < filas; k++)
            {
                resultado.datos[col][fila] += datos[k][fila]*m.datos[col][k];
            }
        }
    }
    return resultado;
}


/*********************************************************************/

Tupla3f Matriz4f::operator * (Tupla3f &t)
{
    Tupla4f resultado;
    Tupla4f t2 = Tupla4f(t.coo[0],t.coo[1],t.coo[2],1);
    for (int fila = 0; fila < filas; fila++)
    {
        resultado.coo[fila] = 0.0;
        for (int col = 0; col < filas; col++)
        {
            resultado.coo[fila] = resultado.coo[fila] + (datos[col][fila]*t2.coo[col]);
        }
    }
    return Tupla3f(resultado.coo[0],resultado.coo[1],resultado.coo[2]);
}


/*********************************************************************/
// Métodos privados
/*********************************************************************/

// Pide la memoria necesaria
void Matriz4f :: ReservaMemoria (void)
{
    datos = new double * [filas];

    for(int i = 0; i < filas; i++)
        datos[i] = new double[filas];

    for(int f=0; f<filas; f++)
        for(int c=0; c<filas; c++)
            datos[f][c] = 0;
}

/*********************************************************************/

// Libera la memoria ocupada
void Matriz4f :: LiberaMemoria (void)
{
    if (datos) {
        for(int i = 0; i < filas; i++)
            delete [] datos[i];
        delete [] datos;
    }
}

/*********************************************************************/

// Inicializa la matriz
void Matriz4f :: Inicializar(char c, double alpha, Tupla3f vector)
{
    // Reservar memoria
    ReservaMemoria();

    // inicializamos la matriz de rotación según para que eje sea,
    // para esta práctica será el eje y
    switch(c)
    {
        case 'x':
            datos[0][0] = 1; datos[1][1] = cos(alpha);
            datos[0][2] = sin(alpha); datos[2][1] = -sin(alpha);
            datos[2][2] = cos(alpha); datos[3][3] = 1;
            break;

        case 'y':
            datos[0][0] = cos(alpha); datos[0][2] = -sin(alpha);
            datos[1][1] = 1; datos[2][0] = sin(alpha);
            datos[2][2] = cos(alpha); datos[3][3] = 1;
            break;

        case 'z':
            datos[0][0] = cos(alpha); datos[0][1] = sin(alpha);
            datos[1][0] = -sin(alpha); datos[1][1] = cos(alpha);
            datos[2][2] = 1; datos[3][3] = 1;
            break;
        case 'a':
            datos[0][0] = cos(alpha)+vector.coo[0]*vector.coo[0]*(1-cos(alpha));
            datos[0][1] = -(vector.coo[0]*vector.coo[1]*(1-cos(alpha)-vector.coo[2]*sin(alpha)));
            datos[0][2] = vector.coo[0]*vector.coo[2]*(1-cos(alpha))+vector.coo[1]*sin(alpha);
            datos[1][0] = -(vector.coo[1]*vector.coo[0]*(1-cos(alpha))-vector.coo[1]*sin(alpha));
            datos[1][1] = cos(alpha)+vector.coo[1]*vector.coo[1]*(1-cos(alpha));
            datos[1][2] = -(vector.coo[1]*vector.coo[2]*(1-cos(alpha))-vector.coo[0]*sin(alpha));
            datos[2][0] = vector.coo[2]*vector.coo[0]*(1-cos(alpha))-vector.coo[1]*sin(alpha);
            datos[2][1] = -(vector.coo[2]*vector.coo[1]*(1-cos(alpha))+vector.coo[0]*sin(alpha));
            datos[2][2] = cos(alpha)+vector.coo[0]*vector.coo[0]*(1-cos(alpha));
            datos[3][3] = 1;
            break;
    }
}

/*********************************************************************/


// Inicializa la matriz
void Matriz4f :: Inicializar(char c, std::vector<double> param)
{
    // Reservar memoria
    ReservaMemoria();

    // inicializamos la matriz de rotación según el tipo de transformación
    // traslación o escalado
    switch(c)
    {
        case 't':
            datos[0][0] = 1; datos[0][3] = param[0];
            datos[1][1] = 1; datos[1][3] = param[1];
            datos[2][2] = 1; datos[2][3] = param[2];
            break;

        case 's':
            datos[0][0] = param[0]; datos[1][1] = param[1];
            datos[2][2] = param[2]; datos[3][3] = 1;
            break;
    }
}

/*********************************************************************/


// Inicializa la matriz
void Matriz4f :: Inicializar(std::vector<double> param)
{
    // Reservar memoria
    ReservaMemoria();

    for(int f=0; f<filas; f++)
        for(int c=0; c<filas; c++)
            datos[f][c] = param[(f*4)+c];

}


// Inicializa la matriz
void Matriz4f :: Inicializar(float * pose)
{
    // Reservar memoria
    ReservaMemoria();

    for(int f=0; f<filas; f++)
        for(int c=0; c<filas; c++)
            datos[f][c] = pose[(f*4)+c];

}

/*********************************************************************/


// Proceso de Copia
void Matriz4f :: Copia(const Matriz4f & otra)
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
