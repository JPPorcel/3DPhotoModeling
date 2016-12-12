#ifndef CILINDRO_H
#define CILINDRO_H

#include "figura.h"
#include <vector>
#include <opencv2/opencv.hpp>

class Cilindro: public Figura
{
private:
    std::vector<cv::Point> puntos;
    int tamcontorno;

public:


    struct Elipse
    {
        cv::Point punto;
        cv::Size axes;
        double angle;
        cv::Point ancla1;
        cv::Point ancla2;
        cv::Point ancla3;
        cv::Point ancla4;
    };

    Cilindro();
    virtual void pintar(cv::Mat &img);
    virtual void generarModelo();
    void addElipse(Elipse e);
    void deleteElipse(int n);
    virtual void extractTexture();
    virtual void setFocal(float f);
    virtual Cilindro * getCopia();
    std::vector<Elipse> getElipses();

private:

    std::vector<Elipse> elipses;
};

#endif // CILINDRO_H
