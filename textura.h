#ifndef TEXTURA_H
#define TEXTURA_H


#include <iostream>
#include "file_ply_stl.h"
#include <GL/glut.h>
#include "jpg_libs/jpg_imagen.hpp"

using namespace std;

class Textura                // Imagen de textura
{
private:
    GLuint        id_text;    // identificador OpenGL de la textura
    unsigned      modo;       // modo de gen. cc.tt. (0 = desactivado, 1 = objeto, 2 = camara)
    cv::Mat imagen;

public:

    Textura(const Textura & otra);
    Textura(string imagen, int m);
    Textura(cv::Mat imagen, int m);
    void activar();          // activa textura en el cauce fijo
    int getModo();
    cv::Mat getImagen() { return imagen; }
};

#endif // TEXTURA_H
