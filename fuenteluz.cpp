#include "fuenteluz.h"


FuenteLuz::FuenteLuz(Tupla4f p, std::vector<Tupla3f> c, float lo, float la)
{
    posvec = p;
    colores = c;
    alpha = lo;
    beta = la;
}

void FuenteLuz::definir(Tupla4f p, std::vector<Tupla3f> c, float lo, float la)
{
    posvec = p;
    colores = c;
    alpha = lo;
    beta = la;
}


void FuenteLuz::activar(int i)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0 + i);
    glLightfv( GL_LIGHT0+i, GL_AMBIENT, &colores[0].coo[0] ) ;
    glLightfv( GL_LIGHT0+i, GL_DIFFUSE, &colores[1].coo[0] ) ;
    glLightfv( GL_LIGHT0+i, GL_SPECULAR, &colores[2].coo[0] ) ;

    Tupla3f t(0,0,0);
    if(posvec.coo[3] == 0)
        glLightfv( GL_LIGHT0+i, GL_SPOT_DIRECTION, &t.coo[0] );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        glRotatef( alpha, 1.0, 0, 0 );
        glRotatef( beta, 0, 1.0, 0 );
        glLightfv( GL_LIGHT0+i, GL_POSITION, &posvec.coo[0] );
    glPopMatrix();
}


void FuenteLuz::desactivar(int i)
{
    glDisable(GL_LIGHT0+i);
}

float FuenteLuz::getAlpha()
{
    return alpha;
}

void FuenteLuz::setAlpha(float a)
{
    alpha = a;
}

float FuenteLuz::getBeta()
{
    return beta;
}


void FuenteLuz::setBeta(float b)
{
    beta = b;
}

Tupla4f * FuenteLuz::getPos()
{
    return &posvec;
}

