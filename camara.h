#ifndef CAMARA_H
#define CAMARA_H


#include <iostream>
#include <GL/glut.h>
#include <opencv2/opencv.hpp>
#include "tuplas.h"
#include <vector>
#include "matriz4f.h"


class Camara                // Imagen de textura
{
private:
    bool perspectiva;  // true -> camara en perspectiva, false -> camara en ortogonal
    double alpha, beta; // alpha -> rotacion entorno al eje X, beta -> rotacion entorno al eje Y

    float zoom;
    float rotacionX, rotacionY;

    int focal;

    static const float frustum_dis_del, frustum_dis_tra, frustum_ancho;

    float* rotation_matrix;
    float* translation_vector;
    CvPOSITObject *positObject;
    bool calculada;

    int height, width;

    std::string ruta;
    cv::Mat clean;
    std::vector<CvPoint3D32f> modelPoints;
    std::vector<CvPoint2D32f> srcImagePoints;

public:

    float projectionMatrix[16];
    int modo;
    cv::Mat img;
    float posePOSIT[16];

    Camara(std::string path, int w, int h);
    Camara(int focal, bool pers=true);

    cv::Mat getClean();

    void fijarProyeccion(int ventana_tam_x, int ventana_tam_y);
    void calcularProyeccion(std::vector<CvPoint3D32f> modelPoints, std::vector<CvPoint2D32f> srcImagePoints);


    void moveX(float inc);
    void moveY(float inc);
    void moveZ(float inc);
    void reset();

    void setFocal(int f);
    double getFocal();
    void setImage(std::string path, int width, int height);
    void clear();
};

#endif // CAMARA_H
