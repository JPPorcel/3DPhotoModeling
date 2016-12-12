#ifndef MATERIAL_H
#define MATERIAL_H


#include <iostream>
#include "tuplas.h"
#include "matrizr3d.h"
#include "file_ply_stl.h"
#include <vector>
#include <GL/glut.h>
#include <cmath>
#include "jpg_libs/jpg_imagen.hpp"

#include "textura.h"



class Material
{
private:
    Textura * text ;        // puntero a la textura (NULL si no hay)
    vector<Tupla3f>   colores ;    // colores: 0=emisión, 1=ambiental, 2=difuso, 3=especular
    float     exponente ;   // exponente para la componente pseudo-especular

public:

    Material();
    Material(const Material & otro);
    Material(string imagen, int modo = 0);
    Material(cv::Mat imagen, int modo = 0);
    Material(vector<Tupla3f> c, float e, string imagen = "", int modo = 0);
    void definir(vector<Tupla3f> c, float e, string imagen = "", int modo = 0);
    void activar();          // activa textura en el cauce fijo
    bool textura();
    int getModo();
    void pintarColores();
    Textura * getTextura() { return text; }

    Material & operator = (const Material & otro);
    Material operator * (float cte);
};

#endif // MATERIAL_H
