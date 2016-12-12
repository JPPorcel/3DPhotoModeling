#ifndef CONTROLADOR_H
#define CONTROLADOR_H

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
#include "util.h"
#include "figura.h"
#include "cilindro.h"
#include "cuboide.h"
#include "modeloimportado.h"
#include "mainwindow.h"
#include "evento.h"
#include "pilaeventos.h"

typedef int MODO_CONTROLADOR;

class Controlador
{

private:

    static Controlador * instance;
    Controlador();

    PilaEventos pilaEventos;

    /// ruta donde se encuentra la imagen seleccionada
    std::string src;

    /// capa sobre la que se pinta la ayuda
    cv::Mat capaAyuda;


public:

    /// getInstance para implementar el patrón singleton
    static Controlador * getInstance()
    {
        if(instance == NULL)
            instance = new Controlador();

        return instance;
    }

    const static MODO_CONTROLADOR MODO_CUBOIDE = 0;
    const static MODO_CONTROLADOR MODO_REVOLUCION = 1;
    const static MODO_CONTROLADOR MODO_SELECCION = 2;
    const static MODO_CONTROLADOR MODO_CONTORNO = 3;
    const static MODO_CONTROLADOR MODO_ROTACION = 4;
    const static MODO_CONTROLADOR MODO_TRASLACION = 5;
    const static MODO_CONTROLADOR MODO_ESFERA = 6;
    const static MODO_CONTROLADOR MODO_ESCALADO = 7;
    static Tupla3f colorX;
    static Tupla3f colorY;
    static Tupla3f colorZ;

    /// modo actual de la aplicación
    MODO_CONTROLADOR modo;
    void changeModo(MODO_CONTROLADOR modo);

    /// modo de pintado
    int modoPintado;

    /// referencia a la interfaz principal
    MainWindow * main;

    /// Figuras
    std::vector<Figura*> figuras;
    int indiceFigura;

    /// puntos
    std::vector<cv::Point> puntos;

    /// colores de los modelos
    std::vector<float *> colores;
    void asignarColor();

    /// variables para el modo cilindro
    double ejeMayor;
    cv::Size axes;
    double angle;
    cv::Point pmedio;
    bool elipseCalculada;
    Cilindro::Elipse ultimaElipse;
    double distancia;

    /// ancho y alto de la imagen base
    int width, height;

    /// true si el botón izq del ratón está pulsado
    bool leftButtonPressed;

    Camara * camara;
    FuenteLuz * luz;


    /// última posición del cursor
    cv::Point * lastPosition;

    /// método de pick (selección de objetos)
    GLfloat* pick(int x, int y);


    void setSize(int w, int h);
    void initialize();
    void resize(int width, int height);
    void sizeAvailable(int width, int height);
    void pintarEjes();
    void pintar(bool luzActiva = true);
    void pintarCV();
    void mousePressEvent(QMouseEvent *event);
    bool mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    cv::Point keyControlFunction(cv::Point p);
    void wheelEvent(QWheelEvent *event);
    void setFocal(int f);
    void clear();
    void setImage(std::string src);
    void importPLY(std::string src);
    void exportImage(std::string src);
    void exportPLY(std::string src);



    /// transformaciones sobre el modelo
    cv::Point posicionInicial;
    void rotarModelo(cv::Point p);
    void escalarModelo(cv::Point p);
    void trasladarModelo(cv::Point p);

    /// ayudas para las transformaciones
    // rotación
    int rotacionAnterior;
    int ejeRotacion;
    void pintarAyudaRotacion();
    // escalar
    double escaladoAnterior;
    int ejeEscalado;
    void pintarAyudaEscalado();
    // trasladar
    Tupla3f traslacionAnterior;
    int ejeTraslacion;
    Tupla3f puntoInicioTraslacion;
    void pintarAyudaTraslacion();
    // selección
    void pintarAyudaModelo();
    void cross(cv::Point p);

    /// figura seleccionada para copiar
    Figura * figuraCopia;

    void recalcularTextura();

    int modeloSeleccionado;
    float * colorSeleccionado;


    cv::Point ThreeDtoTwoD(Tupla3f p);

    void modoRevolucion(cv::Point p);
    bool crearModelo;
    void modoCuboide(cv::Point p);
    void modoEsfera(cv::Point p);
    void calcularPosicionCilindro();
    void calcularPosicionCuboide();

    bool pintarModelo;
    void generarModelo();


    bool shift;
    bool control;
    double anguloEje;
};

#endif // CONTROLADOR_H
