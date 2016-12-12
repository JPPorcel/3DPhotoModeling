#ifndef MODELOIMPORTADO_H
#define MODELOIMPORTADO_H

#include "figura.h"
#include <vector>
#include <opencv2/opencv.hpp>

class ModeloImportado: public Figura
{
public:
    ModeloImportado(std::string ruta);
    ModeloImportado() {}
    virtual void pintar(cv::Mat &img);
    virtual void generarModelo();
    virtual ModeloImportado * getCopia();
    virtual void extractTexture();
    virtual void setFocal(float f);
};

#endif // MODELOIMPORTADO_H
