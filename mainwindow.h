// window.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QMainWindow *parent = 0);
    void updateXYLabels(int x, int y);
    void updateFocalLabel(int f);
    ~MainWindow();
    void resizeEvent(QResizeEvent* event);
    void resizeGLWidget(int width, int height);
    void setPositionCursor(int x, int y);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void seleccionar();
    void generar();
    void cuboide();
    void esfera();
    void textura();
    void cilindro();

private slots:
    void on_actionOpen_jpg_triggered();

    void on_actionImportar_modelo_triggered();

    void on_actionExportar_imagen_triggered();

    void on_actionExportar_modelo_triggered();

private:
    Ui::MainWindow *ui;
    void limpiar();
};

#endif // WINDOW_H
