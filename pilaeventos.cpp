#include "pilaeventos.h"

PilaEventos::PilaEventos()
{
}


void PilaEventos::pushEvento(Evento e)
{
    pila.push(e);
    while(!pila2.empty()) pila2.pop();
}

Evento PilaEventos::popEvento()
{
    if(pila.size() > 0)
    {
        Evento e = pila.top();
        e.deshacer();
        pila.pop();
        pila2.push(e);
        return e;
    }

    return Evento();
}


Evento PilaEventos::redoEvento()
{
    if(pila2.size() > 0)
    {
        Evento e = pila2.top();
        e.rehacer();
        pila2.pop();
        pila.push(e);
        return e;
    }

    return Evento();
}
