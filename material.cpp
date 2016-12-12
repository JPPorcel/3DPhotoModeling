#include "material.h"


/*********************************************************************/

Material::Material()
{
    Tupla3f color[4] = {Tupla3f(0, 0, 0), Tupla3f(0.2, 0.2, 0.2), Tupla3f(0.5, 0.5, 0.5), Tupla3f(0, 0, 0)};
    vector<Tupla3f> col(color, color+sizeof(color)/sizeof(Tupla3f));
    float e = 0.0;

    colores = col;
    exponente = e;
    text = NULL;
}


Material::Material(const Material & otro)
{
    if(otro.text != NULL)
        this->text = new Textura(*otro.text);
    else
        this->text = NULL;
    this->colores = otro.colores;
    this->exponente = otro.exponente;
}


/*********************************************************************/

Material::Material(string imagen, int modo)
{
    Tupla3f color[4] = {Tupla3f(0, 0, 0), Tupla3f(0.2, 0.2, 0.2), Tupla3f(0.5, 0.5, 0.5), Tupla3f(0, 0, 0)};
    vector<Tupla3f> col(color, color+sizeof(color)/sizeof(Tupla3f));
    float e = 0.0;

    colores = col;
    exponente = e;

    if(imagen == "")
    {
        text = NULL;
    }
    else
    {
        text = new Textura(imagen, modo);
    }
}

Material::Material(cv::Mat imagen, int modo)
{
    Tupla3f color[4] = {Tupla3f(0, 0, 0), Tupla3f(0.2, 0.2, 0.2), Tupla3f(0.5, 0.5, 0.5), Tupla3f(0, 0, 0)};
    vector<Tupla3f> col(color, color+sizeof(color)/sizeof(Tupla3f));
    float e = 0.0;

    colores = col;
    exponente = e;

    text = new Textura(imagen, modo);
}


/*********************************************************************/

Material::Material(vector<Tupla3f> c, float e, string imagen, int modo)
{
    colores = c;
    exponente = e;

    if(imagen == "")
    {
        text = NULL;
    }
    else
    {
        text = new Textura(imagen, modo);
    }
}


/*********************************************************************/

void Material::definir(vector<Tupla3f> c, float e, string imagen, int modo)
{
    colores = c;
    exponente = e;

    if(imagen == "")
    {
        text = NULL;
    }
    else
    {
        text = new Textura(imagen, modo);
    }
}


/*********************************************************************/

void Material::activar()
{
    glEnable( GL_NORMALIZE );
    glDisable( GL_COLOR_MATERIAL );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR ) ;

    if(text != NULL)
    {
        text->activar();
    }

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &colores[0].coo[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &colores[1].coo[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &colores[2].coo[0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &colores[3].coo[0]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, exponente);
}


/*********************************************************************/

bool Material::textura()
{
    return (text != NULL);
}


/*********************************************************************/

int Material::getModo()
{
    if(text != NULL)
        return text->getModo();
    else
        return -1;
}


/*********************************************************************/

// Sobrecarga del operador =
Material & Material::operator = (const Material & otro)
{
    if (this != &otro) // Evitar autoasignacion
    {
        text = otro.text;
        colores = otro.colores;
        exponente = exponente;
    }
    return (*this);
}


/*********************************************************************/

Material Material::operator * (float cte)
{
    Material tmp = (*this);

    for(unsigned int i=0; i<tmp.colores.size(); i++)
    {
        tmp.colores[i] = colores[i]*cte;
    }

    return (tmp);
}


/*********************************************************************/

void Material::pintarColores()
{
    cout << "Emision: " << colores[0].coo[0] << " " << colores[0].coo[1] << " " << colores[0].coo[2] << endl;
    cout << "Ambiente: " << colores[1].coo[0] << " " << colores[1].coo[1] << " " << colores[1].coo[2] << endl;
    cout << "Difuso: " << colores[2].coo[0] << " " << colores[2].coo[1] << " " << colores[2].coo[2] << endl;
    cout << "Especular: " << colores[3].coo[0] << " " << colores[3].coo[1] << " " << colores[3].coo[2] << endl;
}
