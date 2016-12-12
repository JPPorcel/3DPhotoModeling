#ifndef FIGURA_H
#define FIGURA_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include "model.h"

class Figura
{

protected:

    Model * modelo;
    float * color;
    float * pose;
    float focal;
    std::vector<CvPoint2D32f> srcImagePoints;
    std::vector<CvPoint3D32f> modelPoints;

public:
    virtual void pintar(cv::Mat &img)=0;
    virtual void generarModelo()=0;
    virtual Figura * getCopia()=0;
    virtual void extractTexture()=0;
    void setModelo(Model * m) { modelo = m; }
    Model * getModelo() { return modelo; }
    void setColor(float * c) { color = c; }
    float * getColor() { return color; }
    void setPose(float * p)
    {
        pose = new float[16];
        for(int i=0; i<16; i++) pose[i] = p[i];
    }
    float * getPose() { return pose; }
    virtual void setFocal(float f)=0;
    float getFocal(){ return focal; }
};

#endif // FIGURA_H
