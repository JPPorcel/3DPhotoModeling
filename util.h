#ifndef UTIL_H
#define UTIL_H

#include <opencv2/opencv.hpp>
#include <QGLWidget>
#include <QtWidgets>
#include <QtOpenGL>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include "fuenteluz.h"
#include "camara.h"
#include "model.h"
#include "cvblob.h"


class Util
{
public:
    Util(){}

    static double distanciaPuntos(cv::Point p1, cv::Point p2);
    static double anguloVectores(cv::Point p1, cv::Point p2);
    static double anguloPuntos(cv::Point a, cv::Point b);
    static cv::Point proyeccionPerpendicular(cv::Point a, cv::Point b, cv::Point p);
    static void torus(float R, float r, int N, int n);
    static cv::Point rotar(cv::Point origen, cv::Point p, float alfa);
    static cv::Point rotar(cv::Point origen, cv::Point p, double alfa, bool grados);
    static bool intersection(cv::Point o1, cv::Point p1, cv::Point o2, cv::Point p2, cv::Point &r);
    static double cross(cv::Point v1, cv::Point v2);
    static float * posit(std::vector<CvPoint3D32f> m, std::vector<CvPoint2D32f> s);
    static cv::Point ThreeDtoTwoD(Tupla3f p, float * pose, float * proj, int width, int height);
    static bool compararColor(float * c1, float * c2);
    static float * generarColor();
};

#endif // UTIL_H
