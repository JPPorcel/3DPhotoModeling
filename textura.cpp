#include "textura.h"


Textura::Textura(const Textura & otra)
{
    this->id_text = otra.id_text;
    this->modo = otra.modo;
}


Textura::Textura(string imagen, int m)
{
    glGenTextures(1,&id_text);
    glBindTexture(GL_TEXTURE_2D,id_text);


    jpg::Imagen * pimg = new jpg::Imagen(imagen.c_str());
    cout << "Cargando textura \"" << imagen << "\" (" << pimg->tamX() << "x" << pimg->tamY() << " px)" << endl;


    //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, pimg->tamX(), pimg->tamY(), GL_BGR, GL_UNSIGNED_BYTE, pimg->leerPixels());

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pimg->tamX(), pimg->tamY(), 0, GL_BGR, GL_UNSIGNED_BYTE, pimg->leerPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    cout << "Textura enviada a la GPU" << endl;

    modo = m;
}

Textura::Textura(cv::Mat imagen, int m)
{
    glGenTextures(1,&id_text);
    glBindTexture(GL_TEXTURE_2D,id_text);

    imagen.copyTo(this->imagen);

    //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, pimg->tamX(), pimg->tamY(), GL_BGR, GL_UNSIGNED_BYTE, pimg->leerPixels());

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagen.cols, imagen.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, imagen.ptr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    modo = m;
}



void Textura::activar()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id_text);
    if(modo == 1)
    {
        glEnable( GL_TEXTURE_GEN_S );
        glEnable( GL_TEXTURE_GEN_T );
        glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR ) ;
        glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR ) ;
    }
    else if(modo == 2)
    {
        glEnable( GL_TEXTURE_GEN_S );
        glEnable( GL_TEXTURE_GEN_T );
        glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR ) ;
        glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR ) ;
    }
}

int Textura::getModo()
{
    return modo;
}

