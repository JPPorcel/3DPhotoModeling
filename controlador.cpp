#include "controlador.h"

Controlador * Controlador::instance = NULL;
Tupla3f Controlador::colorX = tupla3f(1,0,0);
Tupla3f Controlador::colorY = tupla3f(0,1,0);
Tupla3f Controlador::colorZ = tupla3f(0,0,1);

Controlador::Controlador()
{
    src = "/home/juanpi/QtProjects/project/images/cubo.jpg";

    leftButtonPressed = false;

    modo = MODO_REVOLUCION;
    indiceFigura = -1;
    control = false;
    shift = false;
    modoPintado = 2;
    camara = NULL;
    lastPosition = NULL;
    modeloSeleccionado = -1;
    figuraCopia = NULL;
    crearModelo = true;
}

void Controlador::setSize(int w, int h)
{
    width = w;
    height = h;

    clear();
}


/*
 * Al reescalar la pantalla, se llama a este método que dado el tamaño disponible
 * calcula cuanto ocupará la imagen, teniendo en cuenta sus dimensiones
 *
 */
void Controlador::sizeAvailable(int width, int height)
{
    cv::Mat m = cv::imread(src);

    int w = m.rows;
    int h = m.cols;

    double f1 = width*1.0/w;
    double f2 = height*1.0/h;

    double f = f1; if(f2 < f) f = f2;

    main->resizeGLWidget(w*f, h*f);
}


void Controlador::changeModo(MODO_CONTROLADOR m)
{
    if(m != modo)
    {
        clear();
        modo = m;
        std::cout << "Modo: " << m << std::endl;
    }
}


/*********************************************************************/

void Controlador::initialize()
{
    glEnable( GL_DEPTH_TEST );
    glClearColor( 255.0, 255.0, 255.0, 1.0 ) ;
    glColor3f( 0.7, 0.2, 0.4 ) ;
    glLineWidth( 1.0 );
    glPointSize( 2.0 );

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glEnable( GL_NORMALIZE );
    glDisable( GL_COLOR_MATERIAL );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
    glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );


    // Luz
    Tupla3f luz_ambiental(10.0,10.0,10.0);
    Tupla3f luz_difusa(0.0,0.0,0.0);
    Tupla3f luz_especular(0.0,0.0,0.0);

    Tupla4f pos(0,0,0,0);
    std::vector<Tupla3f> vlp;
    vlp.push_back(luz_ambiental);
    vlp.push_back(luz_difusa);
    vlp.push_back(luz_especular);

    luz = new FuenteLuz(pos, vlp, 0, 0);

    if(camara == NULL)
        camara = new Camara(src, width, height);
    this->camara->fijarProyeccion(width, height);
}


/*********************************************************************/

void Controlador::resize(int width, int height)
{
    std::cout << "Resize to: " << width << "x" << height << std::endl;
    this->width = width;
    this->height = height;
    clear();
}


/*********************************************************************/

void Controlador::pintarEjes()
{
    const float long_ejes = 2;

    glColor3f( 1.0, 0.0, 0.0 );
    glBegin(GL_LINES);
        glVertex3f(0,0,0) ;
        glVertex3f(long_ejes,0,0);
    glEnd();

    glColor3f( 0.0, 0.0, 1.0 );
    glBegin(GL_LINES);
        glVertex3f(0,0,0) ;
        glVertex3f(0,long_ejes,0);
    glEnd();

    glColor3f( 0.0, 1.0, 0.0 );
    glBegin(GL_LINES);
        glVertex3f(0,0,0) ;
        glVertex3f(0,0,long_ejes);
    glEnd();
}


/*********************************************************************/

void Controlador::pintar(bool luzActiva)
{
    camara->fijarProyeccion(width, height);

    if(luzActiva)
    {
        glDisable(GL_DEPTH_TEST);
        cv::Mat tempimage;
        cv::flip(camara->img, tempimage, 0);
        glDrawPixels(tempimage.size().width, tempimage.size().height, GL_BGR, GL_UNSIGNED_BYTE, tempimage.ptr());
        glEnable(GL_DEPTH_TEST);
    }

    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(pintarModelo)
    {
        if(modeloSeleccionado != -1)
        {
            glPushMatrix();
                glMatrixMode(GL_PROJECTION);
                camara->setFocal(figuras[modeloSeleccionado]->getFocal());
                glLoadMatrixf(camara->projectionMatrix);
                glTranslatef(0,0,0);
                // flip Z, regla mano izquierda
                glScalef( 1, 1, -1 );

                // Limpiar buffer de profundidad
                //glClear(GL_DEPTH_BUFFER_BIT);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glMultMatrixf( figuras[modeloSeleccionado]->getPose() );
                //pintarEjes();
                if(luzActiva) luz->activar(0);
                else glColor3fv(figuras[modeloSeleccionado]->getColor());

                glDisable(GL_DEPTH_TEST);
                luz->desactivar(0);
                    float * color = new float[3];
                    color[0] = 1; color[1] = 1; color[2] = 0;
                    figuras[modeloSeleccionado]->getModelo()->escalar(1.02);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
                    figuras[modeloSeleccionado]->getModelo()->draw(2, false, false, true);
                    figuras[modeloSeleccionado]->getModelo()->escalar(1/1.02);
                luz->activar(0);
                glEnable(GL_DEPTH_TEST);
                if(luzActiva) luz->desactivar(0);
            glPopMatrix();
        }

        // pintar modelos        
        for(unsigned i=0; i<figuras.size() && figuras[i]->getModelo() != NULL;  i++)
        {
            glPushMatrix();
                glMatrixMode(GL_PROJECTION);
                camara->setFocal(figuras[i]->getFocal());
                glLoadMatrixf( camara->projectionMatrix );
                glTranslatef(0,0,0);
                // flip Z, regla mano izquierda
                glScalef( 1, 1, -1 );

                // Limpiar buffer de profundidad
                //glClear(GL_DEPTH_BUFFER_BIT);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glMultMatrixf( figuras[i]->getPose() );
                if(luzActiva) luz->activar(0);
                else glColor3fv(figuras[i]->getColor());
                figuras[i]->getModelo()->draw(modoPintado, false, false, !luzActiva);

                if(luzActiva) luz->desactivar(0);
            glPopMatrix();
        }

    }

    if(luzActiva)
    {
        if(modeloSeleccionado != -1)
        {
            glMatrixMode(GL_PROJECTION);
            camara->setFocal(figuras[modeloSeleccionado]->getFocal());
            glLoadMatrixf( camara->projectionMatrix );
            glTranslatef(0,0,0);
            // flip Z, regla mano izquierda
            glScalef( 1, 1, -1 );

            // Limpiar buffer de profundidad
            //glClear(GL_DEPTH_BUFFER_BIT);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glMultMatrixf( figuras[modeloSeleccionado]->getPose() );
        }
        capaAyuda = cv::Mat(camara->img.rows, camara->img.cols, CV_8UC4, cv::Scalar(0,0,0,0));

        if(modo == MODO_ROTACION)
            pintarAyudaRotacion();
        else if(modo == MODO_ESCALADO)
            pintarAyudaEscalado();
        else if(modo == MODO_SELECCION)
            pintarAyudaModelo();
        else if(modo == MODO_TRASLACION)
            pintarAyudaTraslacion();

        glEnable( GL_ALPHA_TEST );
        glAlphaFunc( GL_GREATER, 0.5 );
        cv::flip(capaAyuda, capaAyuda, 0);
        glDrawPixels(capaAyuda.size().width, capaAyuda.size().height, GL_BGRA, GL_UNSIGNED_BYTE, capaAyuda.ptr());
        glDisable( GL_ALPHA_TEST );
    }
}


void Controlador::recalcularTextura()
{
    ((Figura*) figuras[modeloSeleccionado])->extractTexture();
}

void Controlador::pintarCV()
{
    if(figuras.size() > 0)
    {
        camara->clear();
        for(unsigned i=0; i<figuras.size(); i++)
        {
            if(indiceFigura != i)
                figuras[i]->pintar(camara->img);
        }
        if(puntos.size() == 0)
            figuras[indiceFigura]->pintar(camara->img);
        pintar();
    }

    if(modo == MODO_REVOLUCION)
    {
        modoRevolucion(*lastPosition);
        figuras[indiceFigura]->pintar(camara->img);
        ((Cilindro*)figuras[indiceFigura])->deleteElipse(1);
    }
}

/*********************************************************************/

void Controlador::mousePressEvent(QMouseEvent *event)
{    
    if(event->button() == Qt::LeftButton)
    {
        int x = event->pos().x();
        int y = event->pos().y();

        // si es la primera vez que pulsamos, inicializamos la variable lastPosition
        if(lastPosition == NULL)
            lastPosition = new cv::Point(x,y);


        // añadimos el punto a la lista de puntos
        cv::Point p(x, y);

        if(puntos.size() != 2)
            p = keyControlFunction(p);
        else if(control)
        {
            cv::Point punto = pmedio;
            double d = Util::distanciaPuntos(punto, p);
            double a = Util::anguloVectores(cv::Point(-1,0), cv::Point(puntos[0].x-puntos[1].x,
                    puntos[0].y-puntos[1].y));
            p = Util::rotar(punto, cv::Point(punto.x+d, punto.y), (a-90)*M_PI/180.0);
        }

        puntos.push_back(p);


        if(modo == MODO_REVOLUCION)
        {
            // si llevamos dos pulsaciones de ratón
            // tenemos el eje mayor de la primera elipse
            if(puntos.size() == 2)
            {
                cv::Point p = puntos[0];
                cv::Point p1 = puntos[1];
                // calculamos el punto medio entre los dos puntos
                pmedio.x = (p.x + p1.x) * 0.5;
                pmedio.y = (p.y + p1.y) * 0.5;
                // medimos la distancia entre los puntos
                // este será el eje mayor de la elipse base
                distancia = Util::distanciaPuntos(p, p1);
            }
        }

        puntos.pop_back();

        if(modo == MODO_REVOLUCION)
        {
            if(puntos.size() == 0)
            {
                indiceFigura++;
                figuras.push_back(new Cilindro());
                asignarColor();
            }
            modoRevolucion(p);

            if(puntos.size() >= 2)
            {
                Evento e(Evento::ADD_ELIPSE, "Añadir elipse");
                std::vector<Cilindro::Elipse> elipses = ((Cilindro *)figuras[indiceFigura])->getElipses();
                e.elipse = elipses[elipses.size()-1];
                e.puntos.push_back(puntos[puntos.size()-1]);
                pilaEventos.pushEvento(e);
            }
            else
            {
                Evento e(Evento::ADD_PUNTO, "Añadir punto");
                e.puntos.push_back(puntos[puntos.size()-1]);
                pilaEventos.pushEvento(e);
            }
        }
        else if(modo == MODO_CUBOIDE)
        {
            modoCuboide(p);
            Evento e(Evento::ADD_PUNTO_CUBOIDE, "Añadir punto");
            e.puntos.push_back(puntos[puntos.size()-1]);
            pilaEventos.pushEvento(e);
        }
        else if(modo == MODO_ESFERA)
        {
            modoEsfera(p);
        }
        else if(modo == MODO_SELECCION)
        {
            for(unsigned i=0; i<figuras.size(); i++)
            {
                figuras[i]->getModelo()->deseleccionar();
            }

            modeloSeleccionado = -1;

            float * c = pick(p.x, p.y);

            for(unsigned i=0; i<figuras.size(); i++)
            {
                if(Util::compararColor(c, figuras[i]->getColor()))
                {
                    std::cout << "Seleccionado modelo " << i << std::endl;
                    figuras[i]->getModelo()->seleccionar();
                    main->updateFocalLabel(figuras[i]->getFocal());
                    modeloSeleccionado = i;
                }
            }
        }
        else if(modo == MODO_ROTACION)
        {
            modo = MODO_SELECCION;
        }
        else if(modo == MODO_ESCALADO)
        {
            modo = MODO_SELECCION;
        }
        else if(modo == MODO_TRASLACION)
        {
            modo = MODO_SELECCION;
        }

        puntos.push_back(p);


        leftButtonPressed = true;


        cv::circle(camara->img, p, 4, cv::Scalar( 0, 0, 255 ), -1, 8);



        lastPosition->x = x;
        lastPosition->y = y;
    }
}


void Controlador::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        leftButtonPressed = false;
        if(modo == MODO_SELECCION)
            camara->clear();
    }
}

/*********************************************************************/

bool Controlador::mouseMoveEvent(QMouseEvent *event)
{
    // inicialmente ponemos update a false
    // si no se produce una actualización de la vista al mover el ratón no se
    // llamará al método UpdateGL
    bool update = false;

    int x = event->pos().x();
    int y = event->pos().y();

    // si lastPosition no está inicializado
    if(lastPosition == NULL)
        lastPosition = new cv::Point(x,y);


    // cargamos la imagen de nuevo para limpiarla
    camara->clear();


    // guardamos el punto en la lista de puntos
    cv::Point p(x, y);

    p = keyControlFunction(p);

    puntos.push_back(p);


    // pintamos la línea del nuevo punto
    if(puntos.size() > 1 && shift)
    {
        cv::line(camara->img, puntos[puntos.size()-2], p, cv::Scalar(255,0,0), 1, CV_AA);
        cross(p);
        cross(puntos[puntos.size()-2]);
        update = true;
    }
    else if(shift)
    {
        cross(p);
        update = true;
    }


    // eliminamos el punto antes de llamar al método modoRevolución
    puntos.pop_back();

    if(figuras.size() > 0 && (modo != MODO_SELECCION && modo != MODO_ROTACION && modo != MODO_TRASLACION &&
                              modo != MODO_ESCALADO))
    {
        for(unsigned i=0; i<figuras.size(); i++)
        {
            if(indiceFigura != i && figuras[i]->getModelo() == NULL)
                figuras[i]->pintar(camara->img);
        }
        if(puntos.size() == 0 && figuras[indiceFigura]->getModelo() == NULL)
            figuras[indiceFigura]->pintar(camara->img);
        if(puntos.size() == 0 && modo == MODO_CUBOIDE && !pintarModelo)
            figuras[indiceFigura]->pintar(camara->img);
        update = true;
    }

    if(modo == MODO_REVOLUCION)
    {
        // método gestor del modo revolución
        modoRevolucion(p);

        if(indiceFigura >= 0 && figuras[indiceFigura]->getModelo() == NULL)
        {
            figuras[indiceFigura]->pintar(camara->img);
            update = true;
        }

        // eliminamos la elipse, ya que es temporal
        if(puntos.size() >= 2 && !leftButtonPressed)
        {
            ((Cilindro*)figuras[indiceFigura])->deleteElipse(1);
        }
        else if(puntos.size() == 2)
        {
            ((Cilindro*)figuras[indiceFigura])->deleteElipse(1);
        }
    }
    else if(modo == MODO_CUBOIDE)
    {
        if(puntos.size() > 0)
        {
            modoCuboide(p);
            /*
            if(puntos.size() >= 3)
            {
                pintarModelo = true;
                modoPintado = 1;
            }
            */
        }

        if(indiceFigura >= 0 && puntos.size() > 0)
        {
            figuras[indiceFigura]->pintar(camara->img);
            update = true;

            ((Cuboide*)figuras[indiceFigura])->deletePunto();
        }
    }
    else if(modo == MODO_ESFERA)
    {
        modoEsfera(p);
        update = true;
    }
    else if(modo == MODO_SELECCION)
    {
        if(puntos.size() >= 1 && leftButtonPressed)
        {
            cv::rectangle(camara->img, puntos[puntos.size()-1], p, cv::Scalar(50,205,50), 1);
            update = true;
        }
    }
    else if(modo == MODO_ROTACION)
    {
        if(modeloSeleccionado != -1)
        {
            rotarModelo(p);
            update = true;
        }
    }
    else if(modo == MODO_ESCALADO)
    {
        if(modeloSeleccionado != -1)
        {
            escalarModelo(p);
            update = true;
        }
    }
    else if(modo == MODO_TRASLACION)
    {
        if(modeloSeleccionado != -1)
        {
            trasladarModelo(p);
            update = true;
        }
    }


    lastPosition->x = x;
    lastPosition->y = y;

    return update;
}

void Controlador::rotarModelo(cv::Point p)
{
    Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();

    cv::Point pMedio = ThreeDtoTwoD(m->getPuntoMedio());
    int a = Util::anguloPuntos(pMedio, posicionInicial);
    int a1 = Util::anguloPuntos(pMedio, p);

    int r = a1-a;

    if(ejeRotacion == MatrizR3D::EJE_Y) r = -r;

    m->rotar(ejeRotacion, (-rotacionAnterior)*M_PI/180.0);
    m->rotar(ejeRotacion, (r)*M_PI/180.0);

    rotacionAnterior = r;
}

void Controlador::escalarModelo(cv::Point p)
{
    Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();

    cv::Point pMedio = ThreeDtoTwoD(m->getPuntoMedio());

    double d = Util::distanciaPuntos(posicionInicial, pMedio);

    double d1 = Util::distanciaPuntos(pMedio, p);

    double s = d1/d;

    if(s < 0.01) s = 0.01;

    if(ejeEscalado == -1)
    {
        m->escalar(1/escaladoAnterior);
        m->escalar(s);
    }
    else
    {
        m->escalar(ejeEscalado, 1/escaladoAnterior);
        m->escalar(ejeEscalado, s);
    }

    escaladoAnterior = s;
}

void Controlador::trasladarModelo(cv::Point p)
{
    Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();
    m->mover(tupla3f(-traslacionAnterior.coo[0], -traslacionAnterior.coo[1], -traslacionAnterior.coo[2]));

    Tupla3f pMedio = m->getPuntoMedio();

    Tupla3f tupla = pMedio;

    switch (ejeTraslacion) {
        case MatrizR3D::EJE_X:
        {
            tupla.coo[0] += 1;
            cv::Point p1 = ThreeDtoTwoD(pMedio);
            cv::Point p2 = ThreeDtoTwoD(tupla);

            double d = Util::distanciaPuntos(p1, p2);

            cv::Point p3 = Util::proyeccionPerpendicular(p1, p2, p);
            Tupla3f tupla2 = pMedio;
            tupla2.coo[0] -= 1;
            cv::Point p4 = ThreeDtoTwoD(tupla2);

            double a2 = Util::anguloPuntos(p1, p4);
            double a = Util::anguloPuntos(p1, p3);

            double d1 = Util::distanciaPuntos(posicionInicial, p3);

            int factor = 1;
            if((a+25) > a2 && (a-25) < a2) factor = -1;

            if(d == 0) factor = 0;
            if(posicionInicial == p) factor = 0;

            m->mover(tupla3f((d1/d)*factor, 0, 0));
            traslacionAnterior = tupla3f((d1/d)*factor, 0, 0);

            break;
        }
        case MatrizR3D::EJE_Y:
        {
            tupla.coo[1] += 1;
            cv::Point p1 = ThreeDtoTwoD(pMedio);
            cv::Point p2 = ThreeDtoTwoD(tupla);

            double d = Util::distanciaPuntos(p1, p2);

            cv::Point p3 = Util::proyeccionPerpendicular(p1, p2, p);
            Tupla3f tupla2 = pMedio;
            tupla2.coo[1] -= 1;
            cv::Point p4 = ThreeDtoTwoD(tupla2);

            double a2 = Util::anguloPuntos(p1, p4);
            double a = Util::anguloPuntos(p1, p3);

            double d1 = Util::distanciaPuntos(posicionInicial, p3);

            int factor = 1;
            if((a+25) > a2 && (a-25) < a2) factor = -1;

            if(a == 0) factor = 0;
            if(posicionInicial == p) factor = 0;

            m->mover(tupla3f(0, (d1/d)*factor, 0));
            traslacionAnterior = tupla3f(0, (d1/d)*factor, 0);

            break;
        }
        case MatrizR3D::EJE_Z:
        {
            tupla.coo[2] += 1;
            cv::Point p1 = ThreeDtoTwoD(pMedio);
            cv::Point p2 = ThreeDtoTwoD(tupla);

            double d = Util::distanciaPuntos(p1, p2);

            cv::Point p3 = Util::proyeccionPerpendicular(p1, p2, p);
            Tupla3f tupla2 = pMedio;
            tupla2.coo[2] -= 1;
            cv::Point p4 = ThreeDtoTwoD(tupla2);

            double a2 = Util::anguloPuntos(p1, p4);
            double a = Util::anguloPuntos(p1, p3);

            double d1 = Util::distanciaPuntos(posicionInicial, p3);

            int factor = 1;
            if((a+25) > a2 && (a-25) < a2) factor = -1;

            if(a == 0) factor = 0;
            if(posicionInicial == p) factor = 0;

            m->mover(tupla3f(0, 0, (d1/d)*factor));
            traslacionAnterior = tupla3f(0, 0, (d1/d)*factor);

            break;
        }
        default:
            break;
    }
}

void Controlador::cross(cv::Point p)
{
    cv::Point p1(p.x-7, p.y);
    cv::Point p2(p.x+7, p.y);
    cv::line(camara->img, p1, p2, cv::Scalar(50,205,50), 2, CV_AA);

    cv::Point p3(p.x, p.y+7);
    cv::Point p4(p.x, p.y-7);
    cv::line(camara->img, p3, p4, cv::Scalar(50,205,50), 2, CV_AA);
}


void Controlador::generarModelo()
{
    modoPintado = 2;
    ((Figura * )figuras[indiceFigura])->generarModelo();
    pintarModelo = true;
    changeModo(MODO_SELECCION);
    modeloSeleccionado = indiceFigura;
    main->updateFocalLabel(figuras[indiceFigura]->getFocal());
    std::cout << "Seleccionado modelo " << indiceFigura << std::endl;
    ((Figura * )figuras[indiceFigura])->getModelo()->seleccionar();
}


cv::Point Controlador::keyControlFunction(cv::Point p)
{
    if(control && puntos.size() > 0)
    {
        if(modo == MODO_REVOLUCION && puntos.size() >= 3)
        {
            std::vector<Cilindro::Elipse> elipses = ((Cilindro*)figuras[indiceFigura])->getElipses();
            Cilindro::Elipse elipse = elipses[elipses.size()-1];
            double a1 = Util::anguloPuntos(elipse.punto, p);
            double a = a1 - elipse.angle;

            if(a >= 180) a = elipse.angle-90;
            else a = elipse.angle+90;

            double d = Util::distanciaPuntos(elipse.punto, p);
            p = Util::rotar(elipse.punto, cv::Point(elipse.punto.x+d, elipse.punto.y), a*M_PI/180.0);
        }
        else
        {
            cv::Point punto = puntos[puntos.size()-1];
            int d = Util::distanciaPuntos(punto, p);
            double a = Util::anguloPuntos(punto, p);

            if(a > 352.5) a = 0;
            int f = (a)/15;
            int r = ((int)(a))%15;

            if(r > 7.5) f++;


            p = Util::rotar(punto, cv::Point(punto.x+d, punto.y), (f*15)*M_PI/180.0);
        }
    }
    return p;
}


void Controlador::keyPressEvent(QKeyEvent *event)
{
    int key = tolower(event->key());

    if(event->key() == Qt::Key_Z && event->modifiers().testFlag(Qt::ControlModifier)
            && event->modifiers().testFlag(Qt::ShiftModifier))
    {
        pilaEventos.redoEvento();
        pintarCV();
    }
    else if(event->key() == Qt::Key_Z && event->modifiers().testFlag(Qt::ControlModifier))
    {
        pilaEventos.popEvento();
        pintarCV();
    }
    else if(event->key() == Qt::Key_C && event->modifiers().testFlag(Qt::ControlModifier))
    {
        figuraCopia = figuras[modeloSeleccionado];
        std::cout << "Figura copiada" << std::endl;
    }
    else if(event->key() == Qt::Key_V && event->modifiers().testFlag(Qt::ControlModifier))
    {
        if(figuraCopia != NULL)
        {
            indiceFigura++;
            figuras.push_back(figuras[modeloSeleccionado]->getCopia());
            asignarColor();
            modeloSeleccionado = indiceFigura;
            figuras[modeloSeleccionado]->getModelo()->seleccionar();
            std::cout << "Figura pegada" << std::endl;
            std::cout << "Seleccionado modelo " << modeloSeleccionado << std::endl;
        }
    }
    else if(key == 's')
    {
        if(modo != MODO_ESCALADO && modeloSeleccionado != -1)
        {
            changeModo(MODO_ESCALADO);
            posicionInicial = *lastPosition;
            ejeEscalado = -1;
            escaladoAnterior = 1;
        }
    }
    else if(key == 'r')
    {
        // faltan cosas de rotar la elipse
        if(modo != MODO_ROTACION && modeloSeleccionado != -1)
        {
            changeModo(MODO_ROTACION);
            posicionInicial = *lastPosition;
            ejeRotacion = MatrizR3D::EJE_X;
            rotacionAnterior = 0;
        }
    }
    else if(key == 'x')
    {
        if(modo == MODO_ROTACION && ejeRotacion != MatrizR3D::EJE_X)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();
            m->rotar(ejeRotacion, (-rotacionAnterior)*M_PI/180.0);
            rotacionAnterior = 0;
            ejeRotacion = MatrizR3D::EJE_X;
            posicionInicial = *lastPosition;
        }
        else if(modo == MODO_ESCALADO && ejeEscalado != MatrizR3D::EJE_X)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();
            if(ejeEscalado == -1)
                m->escalar(1/escaladoAnterior);
            else
                m->escalar(ejeEscalado, 1/escaladoAnterior);

            escaladoAnterior = 1;
            ejeEscalado = MatrizR3D::EJE_X;
            posicionInicial = *lastPosition;
        }
        else if(modo == MODO_TRASLACION && ejeTraslacion != MatrizR3D::EJE_X)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();

            m->mover(tupla3f(-traslacionAnterior.coo[0],-traslacionAnterior.coo[1],-traslacionAnterior.coo[2]));

            traslacionAnterior = tupla3f(0,0,0);
            ejeTraslacion = MatrizR3D::EJE_X;
            Tupla3f pMedio = m->getPuntoMedio();
            posicionInicial = ThreeDtoTwoD(pMedio);
            main->setPositionCursor(posicionInicial.x, posicionInicial.y);
        }
    }
    else if(key == 'y')
    {
        if(modo == MODO_ROTACION && ejeRotacion != MatrizR3D::EJE_Y)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();
            m->rotar(ejeRotacion, (-rotacionAnterior)*M_PI/180.0);
            rotacionAnterior = 0;
            ejeRotacion = MatrizR3D::EJE_Y;
            posicionInicial = *lastPosition;
        }
        else if(modo == MODO_ESCALADO && ejeEscalado != MatrizR3D::EJE_Y)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();
            if(ejeEscalado == -1)
                m->escalar(1/escaladoAnterior);
            else
                m->escalar(ejeEscalado, 1/escaladoAnterior);

            escaladoAnterior = 1;
            ejeEscalado = MatrizR3D::EJE_Y;
            posicionInicial = *lastPosition;
        }
        else if(modo == MODO_TRASLACION && ejeTraslacion != MatrizR3D::EJE_Y)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();

            m->mover(tupla3f(-traslacionAnterior.coo[0],-traslacionAnterior.coo[1],-traslacionAnterior.coo[2]));

            traslacionAnterior = tupla3f(0,0,0);
            ejeTraslacion = MatrizR3D::EJE_Y;
            Tupla3f pMedio = m->getPuntoMedio();
            posicionInicial = ThreeDtoTwoD(pMedio);
            main->setPositionCursor(posicionInicial.x, posicionInicial.y);
        }
    }
    else if(key == 'z')
    {
        if(modo == MODO_ROTACION && ejeRotacion != MatrizR3D::EJE_Z)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();
            m->rotar(ejeRotacion, (-rotacionAnterior)*M_PI/180.0);
            rotacionAnterior = 0;
            ejeRotacion = MatrizR3D::EJE_Z;
            posicionInicial = *lastPosition;
        }
        else if(modo == MODO_ESCALADO && ejeEscalado != MatrizR3D::EJE_Z)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();
            if(ejeEscalado == -1)
                m->escalar(1/escaladoAnterior);
            else
                m->escalar(ejeEscalado, 1/escaladoAnterior);

            escaladoAnterior = 1;
            ejeEscalado = MatrizR3D::EJE_Z;
            posicionInicial = *lastPosition;
        }
        else if(modo == MODO_TRASLACION && ejeTraslacion != MatrizR3D::EJE_Z)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();

            m->mover(tupla3f(-traslacionAnterior.coo[0],-traslacionAnterior.coo[1],-traslacionAnterior.coo[2]));

            traslacionAnterior = tupla3f(0,0,0);
            ejeTraslacion = MatrizR3D::EJE_Z;
            Tupla3f pMedio = m->getPuntoMedio();
            posicionInicial = ThreeDtoTwoD(pMedio);
            main->setPositionCursor(posicionInicial.x, posicionInicial.y);
        }
    }
    else if(key == 't')
    {
        if(modo != MODO_TRASLACION && modeloSeleccionado != -1)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();
            Tupla3f pMedio = m->getPuntoMedio();

            changeModo(MODO_TRASLACION);
            posicionInicial = ThreeDtoTwoD(pMedio);
            ejeTraslacion = MatrizR3D::EJE_X;
            traslacionAnterior = tupla3f(0,0,0);

            cv::Point p = ThreeDtoTwoD(pMedio);
            main->setPositionCursor(p.x, p.y);
        }
    }
    else if(key == Qt::Key_Control)
    {
        control = true;
    }
    else if(key == Qt::Key_Shift)
    {
        shift = true;
    }
    else if(key == Qt::Key_Escape)
    {
        if(modo == MODO_ROTACION)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();
            changeModo(MODO_SELECCION);
            m->rotar(ejeRotacion, (-rotacionAnterior)*M_PI/180.0);
        }
        else if(modo == MODO_ESCALADO)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();
            if(ejeEscalado == -1)
                m->escalar(1/escaladoAnterior);
            else
                m->escalar(ejeEscalado, 1/escaladoAnterior);
            changeModo(MODO_SELECCION);
        }
        else if(modo == MODO_TRASLACION)
        {
            Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();
            m->mover(tupla3f(-traslacionAnterior.coo[0],-traslacionAnterior.coo[1],-traslacionAnterior.coo[2]));
            changeModo(MODO_SELECCION);
        }
        else if(modo == MODO_CUBOIDE)
        {
            ((Cuboide * )figuras[indiceFigura])->calcularForma();
        }
        puntos.clear();
    }
}

void Controlador::keyReleaseEvent(QKeyEvent *event)
{
    int key = tolower(event->key());
    if(key == Qt::Key_Control)
        control = false;
    else if(key == Qt::Key_Shift)
    {
        shift = false;
    }

}


void Controlador::wheelEvent(QWheelEvent *event)
{
    /*
    if (modelo != NULL)
    {
        if(modelo->isSelecc() && event->orientation() == Qt::Orientation::Vertical)
        {
            modelo->escalar(event->delta()*0.001);
        }
    }
    */

    if(event->orientation() == Qt::Orientation::Vertical && modo == MODO_REVOLUCION)
    {
        if(event->delta() > 0)
            ejeMayor += 1;
        else
            ejeMayor -= 1;


        if(indiceFigura >= 0)
        {

            modoRevolucion(*lastPosition);

            std::vector<Cilindro::Elipse> elipses = ((Cilindro*)figuras[indiceFigura])->getElipses();

            elipses[elipses.size()-1].axes.width = ejeMayor;

            camara->clear();

            if(indiceFigura >= 0)
            {
                figuras[indiceFigura]->pintar(camara->img);
            }

            ((Cilindro*)figuras[indiceFigura])->deleteElipse(1);
        }
    }
    else if(event->orientation() == Qt::Orientation::Vertical && modo == MODO_CUBOIDE)
    {
        double f = ((Cuboide*)figuras[indiceFigura])->getFactor();
        if(event->delta() > 0)
            ((Cuboide*)figuras[indiceFigura])->setFactor(f+0.01);
        else
            ((Cuboide*)figuras[indiceFigura])->setFactor(f-0.01);

        ((Cuboide*)figuras[indiceFigura])->addPunto(*lastPosition);
        camara->clear();
        figuras[indiceFigura]->pintar(camara->img);
        ((Cuboide*)figuras[indiceFigura])->deletePunto();
    }
}

/*********************************************************************/

void Controlador::setFocal(int f)
{
    this->camara->setFocal(f);
    if(modeloSeleccionado != -1)
        figuras[modeloSeleccionado]->setFocal(f);
}



/*********************************************************************/

void Controlador::clear()
{
    if(camara == NULL)
    {
        camara = new Camara(src, width, height);
    }

    camara->setImage(src, width, height);


    elipseCalculada = false;

    puntos.clear();
}


/*********************************************************************/

void Controlador::setImage(std::string src)
{
    this->src = src;
    camara->setImage(src, width, height);
    clear();
    figuras.clear();
    indiceFigura = -1;
}

void Controlador::importPLY(string src)
{
    indiceFigura++;
    figuras.push_back(new ModeloImportado(src));
    asignarColor();

    modoPintado = 2;
    ((Figura * )figuras[indiceFigura])->generarModelo();
    pintarModelo = true;
    changeModo(MODO_SELECCION);
    modeloSeleccionado = indiceFigura;
    main->updateFocalLabel(figuras[indiceFigura]->getFocal());
    std::cout << "Seleccionado modelo " << indiceFigura << std::endl;
    ((Figura * )figuras[indiceFigura])->getModelo()->seleccionar();
}


void Controlador::exportPLY(string src)
{
    ((Figura * )figuras[modeloSeleccionado])->getModelo()->exportarPLY(src);
}


void Controlador::modoCuboide(cv::Point p)
{
    if(puntos.size() == 0 && crearModelo)
    {
        std::cout << "Nuevo cuboide" << std::endl;
        indiceFigura++;
        figuras.push_back(new Cuboide());
        asignarColor();
        ((Cuboide*)figuras[indiceFigura])->addPunto(p);
        crearModelo = true;
    }
    else
    {
        crearModelo = true;
        ((Cuboide*)figuras[indiceFigura])->addPunto(p);
    }
}

// cambiar modo revolución
// vamos Juan Pablo!
void Controlador::modoRevolucion(cv::Point p)
{
    if(puntos.size() == 2)
    {
        // la distancia entre el punto medio y la última posición
        // será la mitad del eje menor de la elipse base
        double d = Util::distanciaPuntos(p, pmedio);

        // inicializamos los ejes de la elipse
        axes = cv::Size(distancia/2, d);

        int a = Util::anguloPuntos(puntos[0], puntos[1]);

        angle = a;

        // calculamos la elipse
        Cilindro::Elipse e;
        e.angle = angle;
        e.axes = axes;
        e.punto = pmedio;
        e.ancla1 = puntos[0];
        e.ancla2 = puntos[1];

        // calcular bien ancla3 y ancla4

        int a1 = Util::anguloPuntos(e.punto, p);

        int a2 = (a-a1)%360;
        if(a2 < 0) a2 = 360 + a2;

        if(a2 < 180) a = a + 90;
        else a = a - 90;

        cv::Point p3 = e.punto; p3.x -= e.axes.height; p3 = Util::rotar(e.punto, p3, a, true);
        e.ancla3 = p3;
        cv::Point p4 = e.punto; p4.x -= e.axes.height; p4 = Util::rotar(e.punto, p4, a+180, true);
        e.ancla4 = p4;

        ejeMayor = distancia/2;

        ((Cilindro*)figuras[indiceFigura])->addElipse(e);
    }

    if(puntos.size() >= 3 && !leftButtonPressed)
    {
        std::vector<Cilindro::Elipse> elipses = ((Cilindro*)figuras[indiceFigura])->getElipses();
        Cilindro::Elipse elipse = elipses[elipses.size()-1];

        if(puntos.size() == 3)
        {
            cv::Point m = p-elipse.punto;
            Cilindro::Elipse e;
            e.angle = elipse.angle;
            e.punto = elipse.punto + m;
            e.axes = elipse.axes;
            e.axes.width = ejeMayor;
            cv::Point p1 = e.punto; p1.x -= e.axes.width; p1 = Util::rotar(e.punto, p1, e.angle, true);
            cv::Point p2 = e.punto; p2.x += e.axes.width; p2 = Util::rotar(e.punto, p2, e.angle, true);
            e.ancla1 = p1;
            e.ancla2 = p2;

            int a = e.angle;
            int a1 = Util::anguloPuntos(e.punto, p);

            int a2 = (a-a1)%360;
            if(a2 < 0) a2 = 360 + a2;

            if(a2 < 180) a = a + 90;
            else a = a - 90;

            cv::Point p3 = e.punto; p3.x -= e.axes.height; p3 = Util::rotar(e.punto, p3, a, true);
            e.ancla3 = p3;
            cv::Point p4 = e.punto; p4.x -= e.axes.height; p4 = Util::rotar(e.punto, p4, a+180, true);
            e.ancla4 = p4;

            ((Cilindro*)figuras[indiceFigura])->addElipse(e);
            ultimaElipse = e;
        }
        else
        {
            Cilindro::Elipse elipse1 = elipses[elipses.size()-2];
            int a1 = Util::anguloPuntos(elipse.punto, elipse1.punto);
            int a2 = Util::anguloPuntos(p, elipse.punto);

            if(abs(a1-a2) > 90) p = elipse.punto;

            cv::Point m = p-elipse.punto;
            Cilindro::Elipse e;
            e.angle = elipse.angle;
            e.punto = elipse.punto + m;
            e.axes = elipse.axes;
            e.axes.width = ejeMayor;
            cv::Point p1 = e.punto; p1.x -= e.axes.width; p1 = Util::rotar(e.punto, p1, e.angle, true);
            cv::Point p2 = e.punto; p2.x += e.axes.width; p2 = Util::rotar(e.punto, p2, e.angle, true);
            e.ancla1 = p1;
            e.ancla2 = p2;

            int a = e.angle;
            a1 = Util::anguloPuntos(e.punto, p);

            a2 = (a-a1)%360;
            if(a2 < 0) a2 = 360 + a2;

            if(a2 < 180) a = a + 90;
            else a = a - 90;

            cv::Point p3 = e.punto; p3.x -= e.axes.height; p3 = Util::rotar(e.punto, p3, a, true);
            e.ancla3 = p3;
            cv::Point p4 = e.punto; p4.x -= e.axes.height; p4 = Util::rotar(e.punto, p4, a+180, true);
            e.ancla4 = p4;

            ((Cilindro*)figuras[indiceFigura])->addElipse(e);
            ultimaElipse = e;
        }
    }

    if(puntos.size() >= 3 && leftButtonPressed)
    {
        std::vector<Cilindro::Elipse> elipses = ((Cilindro*)figuras[indiceFigura])->getElipses();
        Cilindro::Elipse e = elipses[elipses.size()-1];

        int a = e.angle;
        int a1 = Util::anguloPuntos(e.punto, p);

        int a2 = (a-a1)%360;
        if(a2 < 0) a2 = 360 + a2;

        if(a2 < 180) a = a + 90;
        else a = a - 90;

        cv::Point p3 = e.punto; p3.x -= e.axes.height; p3 = Util::rotar(e.punto, p3, a, true);
        e.ancla3 = p3;
        cv::Point p4 = e.punto; p4.x -= e.axes.height; p4 = Util::rotar(e.punto, p4, a+180, true);
        e.ancla4 = p4;
        e.axes.height = Util::distanciaPuntos(e.punto, p);

        ((Cilindro*)figuras[indiceFigura])->deleteElipse(1);

        ((Cilindro*)figuras[indiceFigura])->addElipse(e);
    }
}


void Controlador::modoEsfera(cv::Point p)
{
    if(puntos.size() >= 1)
    {
        if(puntos.size() > 1) p = puntos[1];
        cv::Point pmedio = (p + puntos[0]) * 0.5;
        double r = Util::distanciaPuntos(pmedio, p);
        cv::circle(camara->img, pmedio, r, cv::Scalar(255, 0, 0), 1, CV_AA);
    }
}


/*********************************************************************/

cv::Point Controlador::ThreeDtoTwoD(Tupla3f p)
{

    GLdouble modelview[16], projection[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble x, y, z;
    gluProject(p.coo[0], p.coo[1], p.coo[2],
        modelview, projection, viewport,
        &x, &y, &z);

    return cv::Point(x, height-y);
}


/*********************************************************************/

GLfloat* Controlador::pick(int x, int y)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    pintar(false);

    glEnable(GL_DITHER);
    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);


    GLint viewport[4];
    GLfloat * pixel = new GLfloat[3];
    glGetIntegerv(GL_VIEWPORT,viewport);
    glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_FLOAT,pixel);

    pixel[0] = trunc(100*pixel[0])/100.0;
    pixel[1] = trunc(100*pixel[1])/100.0;
    pixel[2] = trunc(100*pixel[2])/100.0;

    //std::cout << "rgb(" << pixel[0] << ", " << pixel[1] << ", " << pixel[2] << ")" << std::endl;

    return pixel;
}

void Controlador::asignarColor()
{
    float * c;

    bool existe = false;
    do
    {
        c = Util::generarColor();
        existe = false;
        for(int i=0; i<colores.size() && !existe; i++)
        {
            if(Util::compararColor(c, colores[i]))
                existe = true;
        }
    }while(existe);

    colores.push_back(c);

    figuras[indiceFigura]->setColor(c);
}

/*********************************************************************/

void Controlador::pintarAyudaRotacion()
{
    if(modeloSeleccionado != -1)
    {
        Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();

        cv::Point pMedio = ThreeDtoTwoD(m->getPuntoMedio());
        cv::Point p = cv::Point(lastPosition->x,lastPosition->y);

        // pintar eje de rotación en OpenGL
        Tupla3f color = colorX;
        if(ejeRotacion == MatrizR3D::EJE_Y) color = colorY;
        if(ejeRotacion == MatrizR3D::EJE_Z) color = colorZ;
        glColor3fv(&color.coo[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &color.coo[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &color.coo[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &color.coo[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &color.coo[0]);
        glPushMatrix();
            Tupla3f t = m->getPuntoMedio();

            glLineWidth(1);
            glBegin(GL_LINES);
                switch (ejeRotacion) {
                case MatrizR3D::EJE_X:
                    glVertex3f(1000, t.coo[1], t.coo[2]);
                    glVertex3f(-1000, t.coo[1], t.coo[2]);
                    break;
                case MatrizR3D::EJE_Y:
                    glVertex3f(t.coo[0], 1000, t.coo[2]);
                    glVertex3f(t.coo[0], -1000, t.coo[2]);
                    break;
                case MatrizR3D::EJE_Z:
                    glVertex3f(t.coo[0], t.coo[1], 1000);
                    glVertex3f(t.coo[0], t.coo[1], -1000);
                    break;
                default:
                    break;
                }
            glEnd();
        glPopMatrix();


        // pintar ayuda sobre la capa de ayuda en OpenCV
        cv::LineIterator it(capaAyuda, pMedio, p, 8);
        for(int i = 0; i < it.count; i++,it++)
            if ( i%5!=0 && i%5!=1) {(*it)[3] = 255;}

        int d = Util::distanciaPuntos(pMedio, p);

        int a = 100/(d*1.0) * 5;

        if(a > 25) a = 25;
        if(a < 3.5) a = 3.5;

        cv::Point p1 = Util::rotar(pMedio, p, 2, true);
        cv::Point p2 = Util::rotar(pMedio, p1, a, true);
        cv::Point p3 = Util::rotar(pMedio, p, -2, true);
        cv::Point p4 = Util::rotar(pMedio, p3, -a, true);

        cv::line(capaAyuda, p1, p2, cv::Scalar(0,0,0,255), 2, CV_AA);
        cv::line(capaAyuda, p3, p4, cv::Scalar(0,0,0,255), 2, CV_AA);

        cv::Point p5 = Util::rotar(p2, p1, 45, true);
        cv::Point p6 = Util::rotar(p2, p1, -45, true);
        cv::line(capaAyuda, p2, p6, cv::Scalar(0,0,0,255), 2, CV_AA);
        cv::line(capaAyuda, p2, p5, cv::Scalar(0,0,0,255), 2, CV_AA);

        cv::Point p7 = Util::rotar(p4, p3, 45, true);
        cv::Point p8 = Util::rotar(p4, p3, -45, true);
        cv::line(capaAyuda, p4, p8, cv::Scalar(0,0,0,255), 2, CV_AA);
        cv::line(capaAyuda, p4, p7, cv::Scalar(0,0,0,255), 2, CV_AA);
    }
}

void Controlador::pintarAyudaEscalado()
{
    if(modeloSeleccionado != -1)
    {
        Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();

        cv::Point pMedio = ThreeDtoTwoD(m->getPuntoMedio());
        cv::Point p = cv::Point(lastPosition->x,lastPosition->y);

        if(ejeEscalado != -1)
        {
            // pintar eje de rotación en OpenGL
            Tupla3f color = colorX;
            if(ejeEscalado == MatrizR3D::EJE_Y) color = colorY;
            if(ejeEscalado == MatrizR3D::EJE_Z) color = colorZ;
            glColor3fv(&color.coo[0]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &color.coo[0]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &color.coo[0]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &color.coo[0]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &color.coo[0]);
            glPushMatrix();
                Tupla3f t = m->getPuntoMedio();

                glLineWidth(1);
                glBegin(GL_LINES);
                    switch (ejeEscalado) {
                    case MatrizR3D::EJE_X:
                        glVertex3f(1000, t.coo[1], t.coo[2]);
                        glVertex3f(-1000, t.coo[1], t.coo[2]);
                        break;
                    case MatrizR3D::EJE_Y:
                        glVertex3f(t.coo[0], 1000, t.coo[2]);
                        glVertex3f(t.coo[0], -1000, t.coo[2]);
                        break;
                    case MatrizR3D::EJE_Z:
                        glVertex3f(t.coo[0], t.coo[1], 1000);
                        glVertex3f(t.coo[0], t.coo[1], -1000);
                        break;
                    default:
                        break;
                    }
                glEnd();
            glPopMatrix();
        }


        // pintar ayuda sobre la capa de ayuda en OpenCV
        cv::LineIterator it(capaAyuda, pMedio, p, 8);
        for(int i = 0; i < it.count; i++,it++)
            if ( i%5!=0 && i%5!=1) {(*it)[3] = 255;}

        int d = Util::distanciaPuntos(pMedio, p);

        int a = Util::anguloPuntos(pMedio, p);

        cv::Point p1 = Util::rotar(p, cv::Point(p.x, p.y + 10), 180-90+a, true);
        cv::Point p2 = Util::rotar(p, cv::Point(p.x, p.y + 20), 180-90+a, true);
        cv::Point p3 = Util::rotar(p, cv::Point(p.x, p.y + 10), 180+90+a, true);
        cv::Point p4 = Util::rotar(p, cv::Point(p.x, p.y + 20), 180+90+a, true);

        cv::line(capaAyuda, p1, p2, cv::Scalar(0,0,0,255), 2, CV_AA);
        cv::line(capaAyuda, p3, p4, cv::Scalar(0,0,0,255), 2, CV_AA);

        cv::Point p5 = Util::rotar(p2, p1, 45, true);
        cv::Point p6 = Util::rotar(p2, p1, -45, true);
        cv::line(capaAyuda, p2, p6, cv::Scalar(0,0,0,255), 2, CV_AA);
        cv::line(capaAyuda, p2, p5, cv::Scalar(0,0,0,255), 2, CV_AA);

        cv::Point p7 = Util::rotar(p4, p3, 45, true);
        cv::Point p8 = Util::rotar(p4, p3, -45, true);
        cv::line(capaAyuda, p4, p8, cv::Scalar(0,0,0,255), 2, CV_AA);
        cv::line(capaAyuda, p4, p7, cv::Scalar(0,0,0,255), 2, CV_AA);
    }
}


void Controlador::pintarAyudaTraslacion()
{
    Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();

    cv::Point pMedio = ThreeDtoTwoD(m->getPuntoMedio());
    cv::Point p = cv::Point(lastPosition->x,lastPosition->y);

    if(ejeTraslacion != -1)
    {
        // pintar eje de rotación en OpenGL
        Tupla3f color = colorX;
        if(ejeTraslacion == MatrizR3D::EJE_Y) color = colorY;
        if(ejeTraslacion == MatrizR3D::EJE_Z) color = colorZ;
        glColor3fv(&color.coo[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &color.coo[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &color.coo[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &color.coo[0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &color.coo[0]);
        glPushMatrix();
            Tupla3f t = m->getPuntoMedio();

            glLineWidth(1);
            glBegin(GL_LINES);
                switch (ejeTraslacion) {
                case MatrizR3D::EJE_X:
                    glVertex3f(1000, t.coo[1], t.coo[2]);
                    glVertex3f(-1000, t.coo[1], t.coo[2]);
                    break;
                case MatrizR3D::EJE_Y:
                    glVertex3f(t.coo[0], 1000, t.coo[2]);
                    glVertex3f(t.coo[0], -1000, t.coo[2]);
                    break;
                case MatrizR3D::EJE_Z:
                    glVertex3f(t.coo[0], t.coo[1], 1000);
                    glVertex3f(t.coo[0], t.coo[1], -1000);
                    break;
                default:
                    break;
                }
            glEnd();
        glPopMatrix();
    }
}


void Controlador::pintarAyudaModelo()
{
    if(modeloSeleccionado != -1)
    {
        Model * m = ((Figura *)figuras[modeloSeleccionado])->getModelo();

        cv::Point p = ThreeDtoTwoD(m->getPuntoMedio());

        // pintar ayuda sobre la capa de ayuda en OpenCV
        cv::Point p1(p.x+5, p.y);
        cv::Point p2(p.x+18, p.y);
        cv::Point p3(p.x-5, p.y);
        cv::Point p4(p.x-18, p.y);
        cv::line(capaAyuda, p1, p2, cv::Scalar(0,0,0,255), 1, 8);
        cv::line(capaAyuda, p3, p4, cv::Scalar(0,0,0,255), 1, 8);

        cv::Point p5(p.x, p.y+5);
        cv::Point p6(p.x, p.y+18);
        cv::Point p7(p.x, p.y-5);
        cv::Point p8(p.x, p.y-18);
        cv::line(capaAyuda, p5, p6, cv::Scalar(0,0,0,255), 1, 8);
        cv::line(capaAyuda, p7, p8, cv::Scalar(0,0,0,255), 1, 8);

        cv::circle(capaAyuda, p, 12, cv::Scalar(255,255,255,255));
    }
}

void Controlador::exportImage(string src)
{
    cv::Mat img(height, width, CV_8UC3);
    //use fast 4-byte alignment (default anyway) if possible
    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);

    //set length of one complete row in destination data (doesn't need to equal img.cols)
    glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());

    glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);

    cv::flip(img, img, 0);

    cv::imwrite(src, img);
}
