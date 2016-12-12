#include "camara.h"

/*********************************************************************/

Camara::Camara(std::string path, int w, int h)
{
    perspectiva = true;
    focal = 760.0;
    rotacionX = 0;
    rotacionY = 0;
    zoom = 1.0;

    rotation_matrix = new float[9];
    translation_vector = new float[3];
    calculada = false;

    ruta = path;
    width = w;
    height = h;

    img = cv::imread(path);


    double f1 = height/(img.cols*1.0);
    double f2 = width/(img.rows*1.0);

    double f = f1;
    if(f > f2) f = f2;

    int w1 = (int)(img.cols*f);
    int h1 = (int)(img.rows*f);
    cv::Mat dst(w1, h1, img.type());
    resize(img, dst, dst.size(), 0, 0, cv::INTER_LANCZOS4);
    img = dst;

    img.copyTo(clean);
    modo = 1;
}


/*********************************************************************/

Camara::Camara(int foc, bool pers)
{
    perspectiva = pers;
    focal = foc;
    rotacionX = 0;
    rotacionY = 0;
    zoom = 1.0;

    rotation_matrix = new float[9];
    translation_vector = new float[3];
    calculada = false;
}


/*********************************************************************/

void Camara::fijarProyeccion(int w, int h)
{
    glClear(GL_DEPTH_BUFFER_BIT);

    width = w; height = h;

    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor( 255.0, 255.0, 255.0, 1.0 ) ;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf( projectionMatrix );

    double f1 = height/(img.cols*1.0);
    double f2 = width/(img.rows*1.0);

    double f = f1;
    if(f > f2) f = f2;

    int w1 = (int)(img.cols*f);
    int h1 = (int)(img.rows*f);

    if(w1 != img.cols || h1 != img.rows)
    {
        cv::Mat dst(w1, h1, img.type());
        resize(img, dst, dst.size(), 0, 0, cv::INTER_LANCZOS4);
        img = dst;
    }
}


/*********************************************************************/


/*********************************************************************/

void Camara::calcularProyeccion(std::vector<CvPoint3D32f> m, std::vector<CvPoint2D32f> s)
{

    srcImagePoints = s;
    modelPoints = m;
    cv::Mat intrinsics(3, 3, CV_32F);
    intrinsics.at<double>(0,0) = focal;
    intrinsics.at<double>(1,1) = focal;
    intrinsics.at<double>(0,2) = width * 0.5;
    intrinsics.at<double>(1,2) = height * 0.5;
    intrinsics.at<double>(2,2) = 1.0;

    double farPlane = 1000.0;
    double nearPlane = 1.0;

    projectionMatrix[0] = 2.0 * intrinsics.at<double>(0,0) / width;
    projectionMatrix[1] = 0.0;
    projectionMatrix[2] = 0.0;
    projectionMatrix[3] = 0.0;

    projectionMatrix[4] = 0.0;
    projectionMatrix[5] = 2.0 * intrinsics.at<double>(1,1) / height;
    projectionMatrix[6] = 0.0;
    projectionMatrix[7] = 0.0;

    projectionMatrix[8] = 2.0 * ( intrinsics.at<double>(0,2) / width) - 1.0;
    projectionMatrix[9] = 2.0 * ( intrinsics.at<double>(1,2) / height ) - 1.0;
    projectionMatrix[10] = -( farPlane+nearPlane ) / ( farPlane - nearPlane );
    projectionMatrix[11] = -1.0;

    projectionMatrix[12] = 0.0;
    projectionMatrix[13] = 0.0;
    projectionMatrix[14] = -2.0 * farPlane * nearPlane / ( farPlane - nearPlane );
    projectionMatrix[15] = 0.0;

    positObject = cvCreatePOSITObject(&modelPoints[0], static_cast<int>(modelPoints.size()));
    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 1000, 1.0e-100f);
    cvPOSIT( positObject, &srcImagePoints[0], focal, criteria, rotation_matrix, translation_vector );



    for (int f=0; f<3; f++)
        for (int c=0; c<3; c++)
            posePOSIT[c*4+f] = rotation_matrix[f*3+c];	//transposed

    posePOSIT[3] = 0.0;
    posePOSIT[7] = 0.0;
    posePOSIT[11] = 0.0;
    posePOSIT[12] = translation_vector[0];
    posePOSIT[13] = translation_vector[1];
    posePOSIT[14] = translation_vector[2];
    posePOSIT[15] = 1.0;
}


/*********************************************************************/

void Camara::setFocal(int f)
{
    focal = f;
    if(modelPoints.size() != 0 && srcImagePoints.size() != 0)
        calcularProyeccion(modelPoints, srcImagePoints);
}


double Camara::getFocal()
{
    return focal;
}

/*********************************************************************/

void Camara::setImage(std::string path, int w, int h)
{
    img = cv::imread(path);

    width = w;
    height = h;

    double f1 = height/(img.cols*1.0);
    double f2 = width/(img.rows*1.0);

    ruta = path;

    double f = f1;
    if(f > f2) f = f2;

    int w1 = (int)(img.cols*f);
    int h1 = (int)(img.rows*f);
    cv::Mat dst(w1, h1, img.type());
    resize(img, dst, dst.size(), 0, 0, cv::INTER_LANCZOS4);
    img = dst;

    img.copyTo(clean);
}

void Camara::clear()
{
    clean.copyTo(img);
}

cv::Mat Camara::getClean()
{
    return clean;
}


/*********************************************************************/

void Camara::moveX(float inc)
{
    translation_vector[0] += inc;
    //std::cout << translation_vector[0] << " | " << translation_vector[1] << " | " << translation_vector[2] << std::endl;
}


/*********************************************************************/

void Camara::moveY(float inc)
{
    translation_vector[1] += inc;
    //std::cout << translation_vector[0] << " | " << translation_vector[1] << " | " << translation_vector[2] << std::endl;
}


/*********************************************************************/

void Camara::moveZ(float inc)
{
    translation_vector[2] += inc;
    //std::cout << translation_vector[0] << " | " << translation_vector[1] << " | " << translation_vector[2] << std::endl;
}


/*********************************************************************/

void Camara::reset()
{
    calculada = false;
}


/*********************************************************************/


