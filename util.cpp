#include "util.h"
#include <random>


/*********************************************************************/
// Funciones auxiliares
double Util::distanciaPuntos(cv::Point p1, cv::Point p2)
{
    double a = (p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y);
    double b = sqrt(a);

    return b;
}

/*********************************************************************/

double Util::anguloVectores(cv::Point p1, cv::Point p2)
{
    double a = p1.x*p2.x + p1.y*p2.y;
    double b = sqrt(p1.x*p1.x + p1.y*p1.y) * sqrt(p2.x*p2.x + p2.y*p2.y);

    return (acos(a/b) * 180/M_PI);
}


double Util::anguloPuntos(cv::Point a1, cv::Point b1)
{
    cv::Point p1(1,0);
    cv::Point p2 = b1-a1;
    double a = p1.x*p2.x + p1.y*p2.y;
    double b = sqrt(p1.x*p1.x + p1.y*p1.y) * sqrt(p2.x*p2.x + p2.y*p2.y);

    double angle = ((acos(a/b) * 180/M_PI));

    if(a1.y < b1.y) angle = 180 + (180-angle);

    return angle;
}

/*********************************************************************/

cv::Point Util::proyeccionPerpendicular(cv::Point v1, cv::Point v2, cv::Point p)
{
    cv::Point e1(v2.x-v1.x, v2.y-v1.y);
    cv::Point e2(p.x-v1.x, p.y-v1.y);
    double valDp = e1.dot(e2);

    double lenLineE1 = sqrt(e1.x*e1.x + e1.y*e1.y);
    double lenLineE2 = sqrt(e2.x*e2.x + e2.y*e2.y);
    double cos = valDp/(lenLineE1*lenLineE2);

    double projLenOfLine = cos * lenLineE2;
    cv::Point point((int)(v1.x + (projLenOfLine * e1.x) / lenLineE1), (int) (v1.y + (projLenOfLine * e1.y) / lenLineE1));

    return point;
}
/*****************************************************************************/


void Util::torus(float R, float r, int N, int n){
    int maxn= 1000; // max precision
    n=min(n,maxn-1);
    N=min(N,maxn-1);
    float rr=1.5f*r;
    double dv=2*M_PI/n;
    double dw=2*M_PI/N;
    double v=0.0f;
    double w=0.0f;
    // outer loop
    while(w<2*M_PI+dw)
    {
        v=0.0f;
        glBegin(GL_TRIANGLE_STRIP);
        // inner loop
        while(v<2*M_PI+dv)
        {
            glNormal3d(
                    (R+rr*cos(v))*cos(w)-(R+r*cos(v))*cos(w),
                    (R+rr*cos(v))*sin(w)-(R+r*cos(v))*sin(w),
                    (rr*sin(v)-r*sin(v)));
            glVertex3d((R+r*cos(v))*cos(w),
                       (R+r*cos(v))*sin(w),
                        r*sin(v));
            glNormal3d(
                    (R+rr*cos(v+dv))*cos(w+dw)-(R+r*cos(v+dv))*cos(w+dw),
                    (R+rr*cos(v+dv))*sin(w+dw)-(R+r*cos(v+dv))*sin(w+dw),
                    rr*sin(v+dv)-r*sin(v+dv));
            glVertex3d((R+r*cos(v+dv))*cos(w+dw),
                       (R+r*cos(v+dv))*sin(w+dw),
                        r*sin(v+dv));
            v+=dv;
        } // inner loop
        glEnd();
        w+=dw;
    } //outer loop

}


cv::Point Util::rotar(cv::Point origen, cv::Point p, float alfa)
{
    float new_x = ((p.x - origen.x) * cos(alfa)) - ((p.y - origen.y) * sin(alfa)) + origen.x;
    float new_y = ((p.y - origen.y) * cos(alfa)) - ((p.x - origen.x) * sin(alfa)) + origen.y;

    return cv::Point(new_x, new_y);
}

cv::Point Util::rotar(cv::Point origen, cv::Point p, double alfa, bool grados)
{
    alfa = -alfa*M_PI/180.0;
    MatrizR3D m(MatrizR3D::EJE_Z, alfa);

    cv::Point p1 = m*cv::Point(p.x-origen.x, p.y-origen.y);


    return cv::Point(p1.x+origen.x, p1.y+origen.y);
}


bool Util::intersection(cv::Point p1, cv::Point p2, cv::Point p3, cv::Point p4, cv::Point &r)
{
   float xD1,yD1,xD2,yD2,xD3,yD3;
   float dot,deg,len1,len2;
   float segmentLen1,segmentLen2;
   float ua,ub,div;

   // calculate differences
   xD1=p2.x-p1.x;
   xD2=p4.x-p3.x;
   yD1=p2.y-p1.y;
   yD2=p4.y-p3.y;
   xD3=p1.x-p3.x;
   yD3=p1.y-p3.y;

   // calculate the lengths of the two lines
   len1=sqrt(xD1*xD1+yD1*yD1);
   len2=sqrt(xD2*xD2+yD2*yD2);

   // calculate angle between the two lines.
   dot=(xD1*xD2+yD1*yD2); // dot product
   deg=dot/(len1*len2);

   // if abs(angle)==1 then the lines are parallell,
   // so no intersection is possible
   if(abs(deg)==1) return false;

   // find intersection Pt between two lines
   cv::Point pt = cv::Point(0,0);
   div=yD2*xD1-xD2*yD1;
   ua=(xD2*yD3-yD2*xD3)/div;
   ub=(xD1*yD3-yD1*xD3)/div;
   pt.x=p1.x+ua*xD1;
   pt.y=p1.y+ua*yD1;

   // calculate the combined length of the two segments
   // between Pt-p1 and Pt-p2
   xD1=pt.x-p1.x;
   xD2=pt.x-p2.x;
   yD1=pt.y-p1.y;
   yD2=pt.y-p2.y;
   segmentLen1=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);

   // calculate the combined length of the two segments
   // between Pt-p3 and Pt-p4
   xD1=pt.x-p3.x;
   xD2=pt.x-p4.x;
   yD1=pt.y-p3.y;
   yD2=pt.y-p4.y;
   segmentLen2=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);

   // if the lengths of both sets of segments are the same as
   // the lenghts of the two lines the point is actually
   // on the line segment.

   // if the point isn’t on the line, return null
   if(abs(len1-segmentLen1)>0.01 || abs(len2-segmentLen2)>0.01)
     return false;

   r = pt;
   // return the valid intersection
   return true;
}

float * Util::posit(std::vector<CvPoint3D32f> m, std::vector<CvPoint2D32f> s)
{
    float* rotation_matrix;
    float* translation_vector;
    rotation_matrix = new float[9];
    translation_vector = new float[3];
    CvPOSITObject *positObject;

    int focal = 700;

    float * pose = new float[16];

    positObject = cvCreatePOSITObject(&m[0], static_cast<int>(m.size()));
    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 100, 1.0e-4f);
    cvPOSIT( positObject, &s[0], focal, criteria, rotation_matrix, translation_vector );


    for (int f=0; f<3; f++)
        for (int c=0; c<3; c++)
            pose[c*4+f] = rotation_matrix[f*3+c];	//transposed

    pose[3] = 0.0;
    pose[7] = 0.0;
    pose[11] = 0.0;
    pose[12] = translation_vector[0];
    pose[13] = translation_vector[1];
    pose[14] = translation_vector[2];
    pose[15] = 1.0;

    return pose;
}


cv::Point Util::ThreeDtoTwoD(Tupla3f p, float * pose, float * proj, int width, int height)
{
    glPushMatrix();
    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor( 255.0, 255.0, 255.0, 1.0 ) ;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf( proj );

    glTranslatef(0,0,0);
    // flip Z, regla mano izquierda
    glScalef( 1, 1, -1 );

    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(pose);

    GLdouble modelview[16], projection[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble x, y, z;
    gluProject(p.coo[0], p.coo[1], p.coo[2],
        modelview, projection, viewport,
        &x, &y, &z);

    glPopMatrix();

    return cv::Point(x, height-y);
}


bool Util::compararColor(float *c1, float *c2)
{
    if(c1[0] == c2[0] && c1[1] == c2[1] && c1[2] == c2[2])
        return true;

    return false;
}

float * Util::generarColor()
{
    int min = 0;
    int max = 100;

    float * c = new float[3];

    for(int i=0; i<3; i++)
    {
        int output = min + (rand() % (int)(max - min + 1));
        c[i] = output/100.0;
    }

    return c;
}
