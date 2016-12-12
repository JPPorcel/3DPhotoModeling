#ifndef COLAEVENTOS_H
#define COLAEVENTOS_H

#include <iostream>
#include <stack>
#include "evento.h"

class PilaEventos
{

private:

    std::stack<Evento> pila;
    std::stack<Evento> pila2;

public:



    PilaEventos();

    void pushEvento(Evento e);
    Evento popEvento();
    Evento redoEvento();
    Evento topEvento();
};

#endif // COLAEVENTOS_H
