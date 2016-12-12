#ifndef CUBOIDE_H
#define CUBOIDE_H

#include "figura.h"
#include <vector>
#include <opencv2/opencv.hpp>

class Cuboide: public Figura
{
private:
    std::vector<cv::Point> puntos;

    double factor;
    std::vector<Tupla3f> vertices;
    int tamcontorno;
public:
    Cuboide();
    void pintar(cv::Mat &img);
    virtual void generarModelo();
    virtual Cuboide * getCopia();
    void addPunto(cv::Point p);
    void deletePunto();
    void calcularForma();
    virtual void extractTexture();
    virtual void setFocal(float f);
    std::vector<cv::Point> getPuntos() { return puntos; }
    void setFactor(double f);
    double getFactor() { return factor; }
};

#endif // CUBOIDE_H
