#include "evento.h"
#include "cilindro.h"
#include "controlador.h"

Evento::Evento()
{

}


Evento::Evento(int t, std::string des)
{
    tipo = t;
    descripcion = des;
}


void Evento::deshacer()
{
    switch (tipo)
    {
        case ADD_ELIPSE:
        {
            Controlador * controlador = Controlador::getInstance();
            int indice = controlador->indiceFigura;

            ((Cilindro*)controlador->figuras[indice])->deleteElipse(1);

            controlador->puntos.pop_back();

            std::cout << "Deshacer: " << descripcion << std::endl;
            break;
        }
        case ADD_PUNTO:
        {
            Controlador * controlador = Controlador::getInstance();
            controlador->puntos.pop_back();
            std::cout << "Deshacer: " << descripcion << std::endl;
            break;
        }
        case ADD_PUNTO_CUBOIDE:
        {
            Controlador * controlador = Controlador::getInstance();
            controlador->puntos.pop_back();
            ((Cuboide *)controlador->figuras[controlador->indiceFigura])->deletePunto();
            if(((Cuboide *)controlador->figuras[controlador->indiceFigura])->getPuntos().size() == 0)
                controlador->crearModelo = false;
            std::cout << "Deshacer: " << descripcion << std::endl;
            break;
        }
        default:
            break;
    }
}

void Evento::rehacer()
{
    switch (tipo)
    {
        case ADD_ELIPSE:
        {
            Controlador * controlador = Controlador::getInstance();
            int indice = controlador->indiceFigura;

            ((Cilindro*)controlador->figuras[indice])->addElipse(elipse);

            controlador->puntos.push_back(puntos[puntos.size()-1]);
            puntos.pop_back();

            std::cout << "Rehacer: " << descripcion << std::endl;
            break;
        }
        case ADD_PUNTO:
        {
            Controlador * controlador = Controlador::getInstance();
            controlador->puntos.push_back(puntos[puntos.size()-1]);
            ((Cuboide *)controlador->figuras[controlador->indiceFigura])->addPunto(puntos[puntos.size()-1]);
            puntos.pop_back();
            std::cout << "Rehacer: " << descripcion << std::endl;
            break;
        }
        default:
            break;
    }
}

