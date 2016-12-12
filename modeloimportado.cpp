#include "modeloimportado.h"
#include "util.h"
#include "controlador.h"

ModeloImportado::ModeloImportado(std::string ruta)
{
    setModelo(new Model(ruta.c_str()));

    Controlador * controlador = Controlador::getInstance();

    std::vector<CvPoint3D32f> m;
    m.push_back(cvPoint3D32f(0,0,0));
    m.push_back(cvPoint3D32f(1,0,0));
    m.push_back(cvPoint3D32f(0,0,1));
    m.push_back(cvPoint3D32f(0,1,0));

    std::vector<CvPoint2D32f> s;
    s.push_back(cvPoint2D32f(0, 0));
    s.push_back(cvPoint2D32f(100, 0));
    s.push_back(cvPoint2D32f(0, 10));
    s.push_back(cvPoint2D32f(0, 100));

    controlador->camara->setFocal(760.0);
    controlador->camara->calcularProyeccion(m, s);
    setPose(controlador->camara->posePOSIT);
    focal = 760.0;

    this->modelPoints = m;
    this->srcImagePoints = s;
}


ModeloImportado * ModeloImportado::getCopia()
{
    ModeloImportado * c = new ModeloImportado();

    c->modelo = new Model(*getModelo());
    c->pose = new float[16];
    for(int i=0; i<16; i++)
        c->pose[i] = pose[i];
    c->focal = getFocal();

    return c;
}

void ModeloImportado::pintar(cv::Mat &img)
{

}

void ModeloImportado::generarModelo()
{
    //extractTexture();
}

void ModeloImportado::extractTexture()
{
    Controlador * controlador = Controlador::getInstance();
    cv::Mat src = controlador->camara->getClean();
    cv::imwrite("tmp.png", src);
    cv::Mat a = cv::imread("tmp.png");
    getModelo()->setMaterial(Material(a, 0));

    std::vector<Tupla3f> vertices = getModelo()->getVertices();
    std::vector<tex> coordTex;
    for(int i=0; i<vertices.size(); i++)
    {
        cv::Point p = Util::ThreeDtoTwoD(vertices[i], getPose(), controlador->camara->projectionMatrix, controlador->width,
                                         controlador->height);
        tex t;
        t.first = p.x/(controlador->width*1.0);
        t.second = p.y/(controlador->height*1.0);
        coordTex.push_back(t);
    }

    getModelo()->setCoordTex(coordTex);
    //std::cout << "textura extraida" << std::endl;
}



void ModeloImportado::setFocal(float f)
{
    Controlador * controlador = Controlador::getInstance();
    focal = f;
    controlador->camara->calcularProyeccion(modelPoints, srcImagePoints);
    controlador->camara->setFocal(focal);
    setPose(controlador->camara->posePOSIT);
}
