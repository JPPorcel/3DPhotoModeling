// myglwidget.h

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <opencv2/opencv.hpp>
#include "mainwindow.h"
#include "fuenteluz.h"
#include "controlador.h"

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    void setMainWindow(MainWindow * m);
    ~MyGLWidget();
    void initialize(int w, int h);
    void sizeAvailable(int width, int height);
    void setPositionCursor(int x, int y);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void exportarImagen();
    void exportarModelo();
signals:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);


public slots:
    void setFocal(int f);
    void cuboideButton();
    void cilindroButton();
    void esferaButton();
    void manualButton();
    void clearCV();
    void openJpg();
    void openPLY();
    void revButton();
    void generarButton();
    void seleccionButton();

signals:
    void focalChanged(int f);

private:

    Controlador * controlador;
    MainWindow * main;
};

#endif // MYGLWIDGET_H
