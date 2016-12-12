#include "cilindro.h"
#include "controlador.h"

Cilindro::Cilindro()
{
    setModelo(NULL);
}

Cilindro * Cilindro::getCopia()
{
    Cilindro * c = new Cilindro();

    c->modelo = new Model(*getModelo());
    c->pose = new float[16];
    for(int i=0; i<16; i++)
        c->pose[i] = pose[i];
    c->focal = getFocal();

    return c;
}

void Cilindro::pintar(cv::Mat &img)
{
    if(elipses.size() > 0)
    {
        for(int i=0; i<elipses.size()-1; i++)
        {
            cv::ellipse(img, elipses[0].punto, elipses[0].axes,
                    -elipses[0].angle, 0, 360, cv::Scalar(255, 0, 0), 1, CV_AA);
            cv::line(img, elipses[i].ancla1, elipses[i+1].ancla1, cv::Scalar(237,149,100), 2, CV_AA);
            cv::line(img, elipses[i].ancla2, elipses[i+1].ancla2, cv::Scalar(237,149,100), 2, CV_AA);
            /*
            cv::line(camara->img, elipses[i].ancla3, elipses[i+1].ancla3, cv::Scalar(237,149,100), 2, CV_AA);
            cv::line(camara->img, elipses[i].ancla4, elipses[i+1].ancla4, cv::Scalar(237,149,100), 2, CV_AA);
            */

            cv::ellipse(img, elipses[i].punto, elipses[i].axes,
                        -elipses[i].angle, 0, 360, cv::Scalar(255, 0, 0), 1, CV_AA);/**/
        }
        cv::ellipse(img, elipses[elipses.size()-1].punto,
                elipses[elipses.size()-1].axes, -elipses[elipses.size()-1].angle, 0, 360,
                cv::Scalar(255, 0, 0), 1, CV_AA);
    }
}

void Cilindro::generarModelo()
{
    std::vector<CvPoint2D32f> imagePoints;
    std::vector<CvPoint3D32f> modelPoints;

    Controlador * controlador = Controlador::getInstance();
    int width = controlador->width;
    int height = controlador->height;

    Elipse e = elipses[elipses.size()-1];
    Elipse e2 = elipses[0];

    double d1 = Util::distanciaPuntos(e.ancla1, e.ancla2);
    double d = Util::distanciaPuntos(e.punto, e2.punto);


    d = (d/d1);

    double d2 = Util::distanciaPuntos(e2.ancla2, e2.punto);
    double d3 = Util::distanciaPuntos(e2.ancla1, e2.ancla2);

    d2 = (d2/d1);
    if(abs(0.5-d2) < (d3*0.0002)) d2 = 0.5;


    imagePoints.push_back(cvPoint2D32f(e.punto.x - (width/2), (height/2) - e.punto.y));
    modelPoints.push_back(cvPoint3D32f(0, 0, 0));

    imagePoints.push_back(cvPoint2D32f(e.ancla2.x - (width/2), (height/2) - e.ancla2.y));
    modelPoints.push_back(cvPoint3D32f(0.5, 0, 0));

    imagePoints.push_back(cvPoint2D32f(e.ancla3.x - (width/2), (height/2) - e.ancla3.y));
    modelPoints.push_back(cvPoint3D32f(0, 0, -0.5));

    //imagePoints.push_back(cvPoint2D32f(e2.punto.x - (width/2), (height/2) - e2.punto.y));
    //modelPoints.push_back(cvPoint3D32f(0, d, 0));

    imagePoints.push_back(cvPoint2D32f(e2.ancla4.x - (width/2), (height/2) - e2.ancla4.y));
    modelPoints.push_back(cvPoint3D32f(0, d, d2));

    imagePoints.push_back(cvPoint2D32f(e2.ancla3.x - (width/2), (height/2) - e2.ancla3.y));
    modelPoints.push_back(cvPoint3D32f(0, d, -d2));

    controlador->camara->calcularProyeccion(modelPoints, imagePoints);

    this->modelPoints = modelPoints;
    this->srcImagePoints = imagePoints;

    int focal;
    int focal1;

    double distanciaMinima = 10000.0;
    double distanciaMinima1 = 10000.0;
    for(int f=300; f<2400; f+=4)
    {
        controlador->camara->setFocal(f);

        cv::Point p = Util::ThreeDtoTwoD(Tupla3f(0, d, -d2), controlador->camara->posePOSIT,
                                         controlador->camara->projectionMatrix, width, height);

        double distancia = Util::distanciaPuntos(p, e2.ancla3);
        if(distancia < distanciaMinima)
        {
            distanciaMinima = distancia;
            focal = f;
        }

        p = Util::ThreeDtoTwoD(Tupla3f(0, 0, -0.5), controlador->camara->posePOSIT,
                                         controlador->camara->projectionMatrix, width, height);

        distancia = Util::distanciaPuntos(p, e.ancla3);
        if(distancia < distanciaMinima1)
        {
            distanciaMinima1 = distancia;
            focal1 = f;
        }
    }

    if(focal1 > focal) focal = focal1;
    controlador->camara->setFocal(focal);

    std::cout << "Focal length calculado a: " << focal << std::endl;

    std::vector<Tupla3f> v;
    v.push_back(Tupla3f(0.5, 0, 0));
    for(int i=elipses.size()-1; i>0; i--)
    {
        double d3 = Util::distanciaPuntos(e.punto, elipses[i].punto);
        d3 = (d3/d1);
        double d4 = Util::distanciaPuntos(elipses[i].ancla1, elipses[i].ancla2);
        d4 = (d4/d1)*0.5;
        v.push_back(Tupla3f(d4, d3, 0));
    }
    v.push_back(Tupla3f(d2, d, 0));
    v.push_back(Tupla3f(d2, d, 0));

    tamcontorno = v.size();

    setPose(controlador->camara->posePOSIT);
    setFocal(focal);


    int ancho = Util::distanciaPuntos(e2.ancla1, e2.ancla2);
    setModelo(new Model(v, ancho*2));
    cv::Mat src = controlador->camara->getClean();
    cv::imwrite("tmp.png", src);
    cv::Mat a = cv::imread("tmp.png");
    getModelo()->setMaterial(Material(a, 0));
    getModelo()->escalar(MatrizR3D::EJE_X,0.95);
    extractTexture();
    getModelo()->escalar(MatrizR3D::EJE_X,1/0.95);
}

void Cilindro::extractTexture()
{
    Controlador * controlador = Controlador::getInstance();

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


    int n = (vertices.size()-2);
    for(int i=tamcontorno; i<(n/2)-tamcontorno; i+=tamcontorno)
    {
        for(int k=1; k<tamcontorno; k++)
        {
            coordTex[n-i+k] = coordTex[i+k];
        }
    }

    getModelo()->setCoordTex(coordTex);

    //cv::imshow("textura", textura);
    //cv::imwrite("tmp.png", textura);
    //getModelo()->setMaterial(Material(textura));
}



void Cilindro::addElipse(Elipse e)
{
    elipses.push_back(e);
}

void Cilindro::deleteElipse(int n)
{
    if(n > elipses.size())
        elipses.resize(0);
    else
        elipses.resize(elipses.size()-n);
}

std::vector<Cilindro::Elipse> Cilindro::getElipses()
{
    return elipses;
}

void Cilindro::setFocal(float f)
{
    Controlador * controlador = Controlador::getInstance();
    focal = f;
    controlador->camara->calcularProyeccion(modelPoints, srcImagePoints);
    controlador->camara->setFocal(focal);
    setPose(controlador->camara->posePOSIT);
}
