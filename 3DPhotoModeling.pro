#-------------------------------------------------
#
# Project created by QtCreator 2016-05-05T18:29:50
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DPhotoModeling
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        myglwidget.cpp \
    fuenteluz.cpp \
    model.cpp \
    file_ply_stl.cpp \
    matrizr3d.cpp \
    material.cpp \
    jpg_libs/jpg_imagen.cpp \
    textura.cpp \
    controlador.cpp \
    camara.cpp \
    util.cpp \
    cuboide.cpp \
    cilindro.cpp \
    evento.cpp \
    pilaeventos.cpp \
    modeloimportado.cpp

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_features2d -lglut -lGLU -O3

HEADERS  += mainwindow.h \
            myglwidget.h \
    fuenteluz.h \
    tuplas.h \
    model.h \
    file_ply_stl.h \
    matrizr3d.h \
    material.h \
    jpg_libs/jpg_imagen.hpp \
    textura.h \
    controlador.h \
    camara.h \
    util.h \
    cuboide.h \
    cilindro.h \
    evento.h \
    pilaeventos.h \
    modeloimportado.h \
    figura.h

FORMS    += mainwindow.ui

DISTFILES += \
    images/c6.jpg \
    images/c8.jpg \
    images/cilindro.jpg \
    images/cubo.jpg \
    images/cubo2.jpg \
    images/cubo3.jpg \
    icons/cube.jpg

RESOURCES += \
    resources.qrc
