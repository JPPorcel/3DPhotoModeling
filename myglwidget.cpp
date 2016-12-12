// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>
#include <cmath>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "myglwidget.h"


/// definir los perfiles

/**
 * Constructor para MyGLWidget, se inicializan los parámetros por defecto
 *
 * @brief MyGLWidget::MyGLWidget
 * @param parent
 */
MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

/**
 * Destructor para MyGLWidget
 *
 * @brief MyGLWidget::~MyGLWidget
 */
MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initialize(int w, int h)
{
    controlador = Controlador::getInstance();
    controlador->setSize(w, h);
    controlador->main = main;
}

/*****************************************************************************/
// Public Slots

/**
 * Establece la distancia focal desde la UI
 *
 * @brief MyGLWidget::setFocal
 * @param f
 */
void MyGLWidget::setFocal(int f)
{
    controlador->setFocal(f);
    main->updateFocalLabel(f);
    updateGL();
}


/**
 * Cambio de modo (modo cuboide) desde la UI
 *
 * @brief MyGLWidget::cuboideButton
 */
void MyGLWidget::cuboideButton()
{
    main->cuboide();
    controlador->changeModo(Controlador::MODO_CUBOIDE);
}

void MyGLWidget::esferaButton()
{
    main->esfera();
    controlador->changeModo(Controlador::MODO_ESFERA);
    updateGL();
}

void MyGLWidget::seleccionButton()
{
    main->seleccionar();
    controlador->changeModo(Controlador::MODO_SELECCION);
    updateGL();
}

void MyGLWidget::manualButton()
{
    main->seleccionar();
    controlador->recalcularTextura();
    updateGL();
}

/**
 * Cambio de modo (modo cilindro) desde la UI
 *
 * @brief MyGLWidget::cilindroButton
 */
void MyGLWidget::cilindroButton()
{
    main->cilindro();
    controlador->changeModo(Controlador::MODO_REVOLUCION);
    updateGL();
}

/**
 * Limpia la pantalla, desde la UI
 *
 * @brief MyGLWidget::clearCV
 */
void MyGLWidget::clearCV()
{
    controlador->clear();

    updateGL();
}

void MyGLWidget::generarButton()
{
    main->seleccionar();
    controlador->generarModelo();
    updateGL();
}


/*****************************************************************************/
/*****************************************************************************/

/**
 * Inicializa los parámetros de OpenGL
 *
 * @brief MyGLWidget::initializeGL
 */
void MyGLWidget::initializeGL()
{
    this->controlador->initialize();
}


/**
 * Establece quien es el padre de la ventana
 *
 * @brief MyGLWidget::setMainWindow
 * @param m
 */
void MyGLWidget::setMainWindow(MainWindow * m)
{
    main = m;
}


/**
 * Función de dibujado de OpenGL (override)
 *
 * @brief MyGLWidget::paintGL
 */
void MyGLWidget::paintGL()
{
    controlador->pintar(true);
}

/**
 * Función de redimensionado de OpenGL
 *
 * @brief MyGLWidget::resizeGL
 * @param width
 * @param height
 */
void MyGLWidget::resizeGL(int width, int height)
{
    controlador->resize(width, height);
    updateGL();
}

void MyGLWidget::sizeAvailable(int width, int height)
{
    controlador->sizeAvailable(width, height);
}

/**
 * Función gestora de pulsación de botones de ratón
 *
 * @brief MyGLWidget::mousePressEvent
 * @param event
 */
void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    controlador->mousePressEvent(event);

    updateGL();
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    controlador->mouseReleaseEvent(event);

    updateGL();
}

/**
 * Función gestora del movimiento del ratón
 *
 * @brief MyGLWidget::mouseMoveEvent
 * @param event
 */
void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();
    main->updateXYLabels(x, y);

    if(controlador->mouseMoveEvent(event))
        updateGL();
}


void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
    int key = tolower(event->key());
    if(key == 'q')
        main->close();
    else
        controlador->keyPressEvent(event);

    updateGL();
}


void MyGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    controlador->keyReleaseEvent(event);

    updateGL();
}


void MyGLWidget::wheelEvent(QWheelEvent *event)
{
    controlador->wheelEvent(event);
    updateGL();
}


void MyGLWidget::openJpg()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Importar imagen jpg"), "", tr("JPG Files (*.jpg)"));

    controlador->setImage(fileName.toStdString());
    updateGL();
}

void MyGLWidget::openPLY()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Importar modelo PLY"), "", tr("PLY Files (*.ply)"));
    controlador->importPLY(fileName.toStdString());
    updateGL();
}

void MyGLWidget::setPositionCursor(int x, int y)
{
    QCursor c = cursor();
    c.setPos(mapToGlobal(QPoint(x, y)));
    setCursor(c);
}


void MyGLWidget::revButton()
{
    controlador->changeModo(Controlador::MODO_REVOLUCION);
}


void MyGLWidget::exportarImagen()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Exportar imagen PNG"), "", tr("PNG Files (*.png)"));
    controlador->exportImage(fileName.toStdString());
}

void MyGLWidget::exportarModelo()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Exportar modeo PLY"), "", tr("PLY Files (*.ply)"));
    controlador->exportPLY(fileName.toStdString());
}
