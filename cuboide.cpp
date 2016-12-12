#include "cuboide.h"
#include "util.h"
#include "controlador.h"

Cuboide::Cuboide()
{
    factor = 1;
    setModelo(NULL);
}

Cuboide * Cuboide::getCopia()
{
    Cuboide * c = new Cuboide();

    c->modelo = new Model(*getModelo());
    c->pose = new float[16];
    for(int i=0; i<16; i++)
        c->pose[i] = pose[i];
    c->focal = getFocal();

    return c;
}

void Cuboide::pintar(cv::Mat &img)
{
    if(puntos.size() > 3)
    {
        Controlador * controlador = Controlador::getInstance();
        std::vector<Tupla3f> v = modelo->getVertices();
        std::vector<cv::Point> points;

        for(int i=0; i<v.size()-2; i++)
        {
            cv::Point p = Util::ThreeDtoTwoD(v[i], getPose(), controlador->camara->projectionMatrix,
                                             controlador->width, controlador->height);
            points.push_back(p);
        }


        int n = (points.size())/5;
        for(int i=0; i<points.size()-n; i++)
        {
            cv::line(img, points[i], points[i+n], cv::Scalar(237,149,100), 2, CV_AA);
        }
    }
    else
    {
        for(int i=0; i<puntos.size()-1; i++)
        {
            cv::circle(img, puntos[i], 1, cv::Scalar(255, 0, 0), 1, CV_AA);
            cv::line(img, puntos[i], puntos[i+1], cv::Scalar(237,149,100), 2, CV_AA);
        }
    }

    /*
    for(int i=0; i<modelPoints.size()/4; i++)
    {
        for(int k=0; k<4; k++)
        {
            cv::line(img, modelPoints[(i*4+k)%4 + (i*4)], modelPoints[(i*4+k+1)%4 + (i*4)],
                    cv::Scalar(237,149,100), 2, CV_AA);
            if(i != 0)
            {
                cv::line(img, modelPoints[(i*4+k)%4 + (i*4)], modelPoints[((i-1)*4+k)%4 + ((i-1)*4)],
                        cv::Scalar(237,149,100), 2, CV_AA);
            }
        }
    }
    */
}

void Cuboide::generarModelo()
{
    calcularForma();
    extractTexture();
}

void Cuboide::extractTexture()
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

    for(int k=1; k<tamcontorno; k++)
    {
        coordTex[k+tamcontorno*3] = coordTex[k+tamcontorno];
    }

    getModelo()->setCoordTex(coordTex);
}

void Cuboide::addPunto(cv::Point p)
{
    puntos.push_back(p);
    if(puntos.size() > 3)
        calcularForma();
}

void Cuboide::deletePunto()
{
    puntos.pop_back();
    if(vertices.size() > 2)
    {
        vertices.pop_back();
    }
}


void Cuboide::calcularForma()
{
    if(puntos.size() > 3)
    {
        cv::Point p0 = puntos[0];
        cv::Point px = puntos[1];
        cv::Point pxz = puntos[2];
        cv::Point pxyz = puntos[3];

        cv::Point pMedio = (p0+pxz)*0.5;
        double dr = Util::distanciaPuntos(pMedio, p0);
        double r = sqrt(2)*0.5;
        double d = Util::distanciaPuntos(pxyz, pxz);
        double r1 = (d/dr) * r;
        if(abs(1-r1) < 0.25) r1 = 1;

        std::vector<Tupla3f> v;
        if(puntos.size() == 4)
        {
            v.push_back(Tupla3f(r,0,0));

            double rx = r*factor;
            v.push_back(Tupla3f(rx,r1,0));
        }
        else
        {
            v = vertices;
            v.pop_back();
            d = Util::distanciaPuntos(puntos[puntos.size()-1], pxz);
            double r2 = (d/dr) * r;
            double rx = r*factor;
            v.push_back(Tupla3f(rx,r2,0));
        }
        v.push_back(v[v.size()-1]);
        tamcontorno = v.size();
        setModelo(new Model(v, 4));
        //modelo->rotar(MatrizR3D::EJE_Y, 45*M_PI/180.0);
        //modelo->mover(tupla3f(0.5,0,0.5));

        //setModelo(new Model("/home/juanpi/QtProjects/prueba/plys/cube.ply"));
        //modelo->mover(tupla3f(0.5,0.5,0));


        Controlador * controlador = Controlador::getInstance();
        int width = controlador->width;
        int height = controlador->height;


        std::vector<CvPoint3D32f> m;
        m.push_back(cvPoint3D32f(0,0,0));
        m.push_back(cvPoint3D32f(r,0,0));
        m.push_back(cvPoint3D32f(0,0,-r));
        m.push_back(cvPoint3D32f(r,r1,0));

        std::vector<CvPoint2D32f> s;
        s.push_back(cvPoint2D32f(pMedio.x-(width/2), (height/2)-pMedio.y));
        s.push_back(cvPoint2D32f(pxz.x-(width/2), (height/2)-pxz.y));
        s.push_back(cvPoint2D32f(px.x-(width/2), (height/2)-px.y));
        s.push_back(cvPoint2D32f(pxyz.x-(width/2), (height/2)-pxyz.y));

        controlador->camara->setFocal(760.0);
        controlador->camara->calcularProyeccion(m, s);
        setPose(controlador->camara->posePOSIT);
        focal = 760.0;

        this->modelPoints = m;
        this->srcImagePoints = s;
        vertices = v;
    }
}


void Cuboide::setFocal(float f)
{
    Controlador * controlador = Controlador::getInstance();
    focal = f;
    controlador->camara->calcularProyeccion(modelPoints, srcImagePoints);
    controlador->camara->setFocal(focal);
    setPose(controlador->camara->posePOSIT);
}

void Cuboide::setFactor(double f)
{
    factor = f;
    if(factor <= 0) factor = 0.01;
}
