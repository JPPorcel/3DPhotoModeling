#ifndef MODEL_H
#define MODEL_H


#include <iostream>
#include "tuplas.h"
#include "matrizr3d.h"
#include "file_ply_stl.h"
#include <vector>
#include <GL/glut.h>
#include <math.h>
#include "material.h"
#include <fstream>

using namespace std;

typedef pair<GLfloat, GLfloat> tex;


class Model
{

private:

    vector<Tupla3f> vertices;
    vector<Tupla3i> caras;
    vector<Tupla3f> normalesVertices;
    vector<Tupla3f> normalesCaras;
    vector<Tupla3f> colores;
    vector<Tupla3f> baricentros;
    vector<tex> coordTex;
    Material material;
    Material materialSeleccionado;
    bool seleccionado;
    Tupla3f puntoCentro;
    double distanciaMaxima;

    void revolucion(int N, bool tapaSUP, bool tapaINF, bool textura);
    void creaPLY(const char * archivo);
    void barrido(const char * archivo, bool cerrado, double N);
    void calcularPuntoMedio();

public:

    Model(){}
    Model(const Model & otro);
    Model(const char * archivo, Material m = Material());
    Model(const char * archivo, int N, bool tapaSUP = true, bool tapaINF = true, bool textura = false,
          Material m = Material());
    Model(const char * archivo, bool cerrado, double N);
    Model(vector<Tupla3f> ver, vector<Tupla3i> car, vector<tex> coords);
    Model(vector<Tupla3f> contorno, int slides);
    void leePLY(const char * archivo, Material m = Material());
    void crearRevolucion(const char * archivo, int N, bool tapaSUP = true, bool tapaINF = true,
                         bool textura = false, Material m = Material());
    void crearBarrido(const char * archivo, bool cerrado, double N);
    void calcularNormalesVertices();
    void calcularNormalesCaras();
    void calcularNormales();
    void calcularBaricentros();
    vector<Tupla3f> getVertices();
    vector<Tupla3i> getCaras();
    Material getMaterial();
    void setMaterial(Material m);
    void setCoordTex(vector<tex> c);
    void draw(int modo = 2, bool = false, bool = false, bool s = false);
    void drawNormalesCaras();
    void drawNormalesVertices();
    void seleccionar();
    void deseleccionar();
    Tupla3f getPuntoMedio();
    double getDistanciaMaxima();
    bool isSelecc();

    void exportarPLY(string path);

    void rotar(int eje, double angle, bool sobreSi=true);
    void escalar(double factor);
    void escalar(int eje, double factor);
    void mover(Tupla3f m);
};

#endif // MODEL_H
