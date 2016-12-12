// window.cpp

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "myglwidget.h"

MainWindow::MainWindow(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setMinimumSize(QDesktopWidget().availableGeometry(this).size() * 0.95);

    ui->setupUi(this);

    std::cout << "Size: " << QDesktopWidget().availableGeometry(this).size().width() <<
                 "x" << QDesktopWidget().availableGeometry(this).size().height() << std::endl;

    //connect(ui->myGLWidget, SIGNAL(fovChanged(int)), ui->fovSlider, SLOT(setValue(int)));
    //connect(ui->myGLWidget, SIGNAL(focalChanged(int)), ui->focalSlider, SLOT(setValue(int)));

    ui->myGLWidget->setMainWindow(this);
    ui->myGLWidget->initialize(ui->myGLWidget->size().width(), ui->myGLWidget->size().width());

    ui->xLabel->setText(QString(""));
    ui->yLabel->setText(QString(""));
    ui->label->setStyleSheet("border: 1px solid grey");
    QPalette pal = ui->cilindroButton->palette();
    pal.setColor(QPalette::Button, QColor(100,149,237));
    ui->cilindroButton->setAutoFillBackground(true);
    ui->cilindroButton->setPalette(pal);
    ui->cilindroButton->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateXYLabels(int x, int y)
{
    std::string xs = static_cast<std::ostringstream*>( &(std::ostringstream() << x) )->str();
    ui->xLabel->setText(QString(xs.c_str()));
    std::string ys = static_cast<std::ostringstream*>( &(std::ostringstream() << y) )->str();
    ui->yLabel->setText(QString(ys.c_str()));
}


void MainWindow::updateFocalLabel(int f)
{
    std::string x = static_cast<std::ostringstream*>( &(std::ostringstream() << f) )->str();
    ui->focalLabel->setText(QString(x.c_str()));
    ui->focalSlider->setValue(f);
}


void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   int height = event->size().height();
   int width = event->size().width();


   int w1 = ui->leftPanel->size().width();
   int h1 = ui->bottomPanel->size().height();
   ui->myGLWidget->sizeAvailable(width-w1, height-h1);
   ui->bottomPanel->move(0, height-h1);
   // Your code here.
}

void MainWindow::setPositionCursor(int x, int y)
{
    ui->myGLWidget->setPositionCursor(x, y);
}


void MainWindow::resizeGLWidget(int width, int height)
{
    std::cout << "Fix size: " << width << "x" << height << std::endl;
    ui->myGLWidget->resize(width, height);
}

void MainWindow::on_actionOpen_jpg_triggered()
{
    ui->myGLWidget->openJpg();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    ui->myGLWidget->keyPressEvent(event);
}


void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    ui->myGLWidget->keyReleaseEvent(event);
}

void MainWindow::on_actionImportar_modelo_triggered()
{
    ui->myGLWidget->openPLY();
}

void MainWindow::seleccionar()
{
    limpiar();
    QPalette pal = ui->seleccionar->palette();
    pal.setColor(QPalette::Button, QColor(100,149,237));
    ui->seleccionar->setAutoFillBackground(true);
    ui->seleccionar->setPalette(pal);
    ui->seleccionar->update();
}

void MainWindow::cilindro()
{
    limpiar();
    QPalette pal = ui->cilindroButton->palette();
    pal.setColor(QPalette::Button, QColor(100,149,237));
    ui->cilindroButton->setAutoFillBackground(true);
    ui->cilindroButton->setPalette(pal);
    ui->cilindroButton->update();
}

void MainWindow::cuboide()
{
    limpiar();
    QPalette pal = ui->cuboideButton->palette();
    pal.setColor(QPalette::Button, QColor(100,149,237));
    ui->cuboideButton->setAutoFillBackground(true);
    ui->cuboideButton->setPalette(pal);
    ui->cuboideButton->update();
}
void MainWindow::generar()
{
    limpiar();
    QPalette pal = ui->generarModelo->palette();
    pal.setColor(QPalette::Button, QColor(100,149,237));
    ui->generarModelo->setAutoFillBackground(true);
    ui->generarModelo->setPalette(pal);
    ui->generarModelo->update();
}
void MainWindow::textura()
{
    limpiar();
    QPalette pal = ui->extraerTextura->palette();
    pal.setColor(QPalette::Button, QColor(100,149,237));
    ui->extraerTextura->setAutoFillBackground(true);
    ui->extraerTextura->setPalette(pal);
    ui->extraerTextura->update();
}
void MainWindow::esfera()
{
    limpiar();
    QPalette pal = ui->esfera->palette();
    pal.setColor(QPalette::Button, QColor(100,149,237));
    ui->esfera->setAutoFillBackground(true);
    ui->esfera->setPalette(pal);
    ui->esfera->update();
}

void MainWindow::limpiar()
{
    QPalette pal = ui->cilindroButton->palette();
    pal.setColor(QPalette::Button, QColor(100,149,237,0));

    ui->cilindroButton->setPalette(pal);
    ui->cilindroButton->update();

    ui->cuboideButton->setPalette(pal);
    ui->cuboideButton->update();

    ui->extraerTextura->setPalette(pal);
    ui->extraerTextura->update();

    ui->seleccionar->setPalette(pal);
    ui->seleccionar->update();

    ui->generarModelo->setPalette(pal);
    ui->generarModelo->update();

    ui->esfera->setPalette(pal);
    ui->esfera->update();
}

void MainWindow::on_actionExportar_imagen_triggered()
{
    ui->myGLWidget->exportarImagen();
}

void MainWindow::on_actionExportar_modelo_triggered()
{
    ui->myGLWidget->exportarModelo();
}
