#ifndef EVENTO_H
#define EVENTO_H

#include <iostream>
#include "cilindro.h"

typedef int TIPO_EVENTO;

class Evento
{
private:

    std::string descripcion;

    int tipo;

public:

    const static TIPO_EVENTO ADD_ELIPSE = 0;
    const static TIPO_EVENTO ADD_PUNTO = 1;
    const static TIPO_EVENTO ADD_PUNTO_CUBOIDE = 2;


    Cilindro::Elipse elipse;
    std::vector<cv::Point> puntos;


    Evento();

    Evento(int t, std::string des);

    void deshacer();
    void rehacer();
    TIPO_EVENTO getTipo() { return tipo; }
};

#endif // EVENTO_H
