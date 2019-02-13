///////////////////////////////////////////////////////////////////////////
// Práctica 2 leer fichero ply y objetos de revolución
///////////////////////////////////////////////////////////////////////////

#include "objetos.h"

///////////////////////////////////////////////////////////////////////////
// Puntos3D
///////////////////////////////////////////////////////////////////////////

_puntos3D::_puntos3D(){

}

///////////////////////////////////////////////////////////////////////////
// Triángulo3D
///////////////////////////////////////////////////////////////////////////

_triangulos3D::_triangulos3D(){

}

void _triangulos3D::draw(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez){
    switch(mode){
        case GL_POINT: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); glPointSize(grosor); break;  
        case GL_LINE: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glLineWidth(grosor); break;
        case GL_FILL: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    }

    isSelected? glColor3f(1.000, 0.412, 0.706) : glColor3f(r1, g1, b1);

    glBegin(GL_TRIANGLES);
        for(unsigned int i = 0; i<caras.size(); ++i){

            if(mode == GL_FILL && ajedrez){
                if(i % 2 == 0)
                    isSelected? glColor3f(0.294, 0.000, 0.510) : glColor3f(r1, g1, b1);
                else
                    isSelected? glColor3f(1.000, 0.412, 0.706) : glColor3f(r2, g2, b2);
            }

            glVertex3f(vertices[caras[i]._0].x, vertices[caras[i]._0].y, vertices[caras[i]._0].z);
            glVertex3f(vertices[caras[i]._1].x, vertices[caras[i]._1].y, vertices[caras[i]._1].z);
            glVertex3f(vertices[caras[i]._2].x, vertices[caras[i]._2].y, vertices[caras[i]._2].z);
        }
    glEnd();
}

///////////////////////////////////////////////////////////////////////////
// Cubo
///////////////////////////////////////////////////////////////////////////

_cubo::_cubo(float tam){
    vertices.resize(8);

    vertices[0].x = -tam;    vertices[0].y = -tam;     vertices[0].z = tam;
    vertices[1].x = tam;     vertices[1].y = -tam;     vertices[1].z = tam;
    vertices[2].x = tam;     vertices[2].y = -tam;     vertices[2].z = -tam;
    vertices[3].x = -tam;    vertices[3].y = -tam;     vertices[3].z = -tam;
    vertices[4].x = -tam;    vertices[4].y = tam;      vertices[4].z = tam;
    vertices[5].x = tam;     vertices[5].y = tam;      vertices[5].z = tam;
    vertices[6].x = tam;     vertices[6].y = tam;      vertices[6].z = -tam;
    vertices[7].x = -tam;    vertices[7].y = tam;      vertices[7].z = -tam;

    caras.resize(12);

    caras[0]._0 = 0;       caras[0]._1 = 1;       caras[0]._2 = 5;
    caras[1]._0 = 0;       caras[1]._1 = 5;       caras[1]._2 = 4;
    caras[2]._0 = 1;       caras[2]._1 = 6;       caras[2]._2 = 5;
    caras[3]._0 = 1;       caras[3]._1 = 2;       caras[3]._2 = 6;
    caras[4]._0 = 4;       caras[4]._1 = 5;       caras[4]._2 = 6;
    caras[5]._0 = 4;       caras[5]._1 = 6;       caras[5]._2 = 7;
    caras[6]._0 = 0;       caras[6]._1 = 4;       caras[6]._2 = 3;
    caras[7]._0 = 3;       caras[7]._1 = 4;       caras[7]._2 = 7;
    caras[8]._0 = 2;       caras[8]._1 = 3;       caras[8]._2 = 7;
    caras[9]._0 = 2;       caras[9]._1 = 7;       caras[9]._2 = 6;
    caras[10]._0 = 0;      caras[10]._1 = 2;      caras[10]._2 = 1;
    caras[11]._0 = 0;      caras[11]._1 = 3;      caras[11]._2 = 2;
}

///////////////////////////////////////////////////////////////////////////
// Pirámide:
///////////////////////////////////////////////////////////////////////////

_piramide::_piramide(float tam, float al){
    vertices.resize(5);

    vertices[0].x = -tam;   vertices[0].y = 0;     vertices[0].z = tam;
    vertices[1].x = tam;    vertices[1].y = 0;     vertices[1].z = tam;
    vertices[2].x = tam;    vertices[2].y = 0;     vertices[2].z = -tam;
    vertices[3].x = -tam;   vertices[3].y = 0;     vertices[3].z = -tam;
    vertices[4].x = 0;      vertices[4].y = al;    vertices[4].z = 0;

    caras.resize(6);

    caras[0]._0 = 0;     caras[0]._1 = 1;    caras[0]._2 = 4;
    caras[1]._0 = 1;     caras[1]._1 = 2;    caras[1]._2 = 4;
    caras[2]._0 = 2;     caras[2]._1 = 3;    caras[2]._2 = 4;
    caras[3]._0 = 3;     caras[3]._1 = 0;    caras[3]._2 = 4;
    caras[4]._0 = 3;     caras[4]._1 = 1;    caras[4]._2 = 0;
    caras[5]._0 = 3;     caras[5]._1 = 2;    caras[5]._2 = 1;
}

///////////////////////////////////////////////////////////////////////////
// Objeto PLY
///////////////////////////////////////////////////////////////////////////

_objetoPLY::_objetoPLY(){

}

void _objetoPLY::leer(char* archivo){
    _file_ply File_ply;
    
    File_ply.open(archivo);
    File_ply.read(vertices, caras);
    File_ply.close();
}

///////////////////////////////////////////////////////////////////////////
// Objeto revolución
///////////////////////////////////////////////////////////////////////////

_revolucion::_revolucion(){

}

/** Representación:
 * Filas (j) -> rotaciones
 * Columnas (i) -> puntos
 * vertices[j][i] -> punto i-ésimo del perfil j-ésimo
**/ 
void _revolucion::rotar(vector<_vertex3f> perfil, int n){
    int npp = perfil.size();
    int n_vertices = npp * n;
    vertices.resize(n_vertices);
    _vertex3f aux;
    _vertex3i cara;

    //Rellenar el vector vértices con los puntos del perfil rotados:
    for(int i = 0; i<npp; ++i){
        for(int j = 0; j<n; ++j){
            aux.x = perfil[i].x * cos(2.0*M_PI*j/n) + perfil[i].z * sin(2.0*M_PI*j/n);
            aux.y = perfil[i].y;
            aux.z = -perfil[i].x * sin(2.0*M_PI*j/n) + perfil[i].z * sin(2.0*M_PI*j/n);
            vertices[j * npp + i] = aux;
        }
    }

    //Crear las caras (menos la última):
    for(int j = 0; j<n-1; ++j){
        for(int i = 0; i<npp-1; ++i){
            cara._0 = j * npp +i;
            cara._1 = (j+1) * npp + i;
            cara._2 = j * npp + (i+1);
            caras.push_back(cara);
            cara._0 = (j+1) * npp + i;
            cara._1 = (j+1) * npp + (i+1);
            cara._2 = j * npp + (i+1);
            caras.push_back(cara);
        }
    }

    //Tratamiento de la última cara:
    for(int i = 0; i<npp-1; ++i){
        cara._0 = (n-1) * npp + i;
        cara._1 = i;
        cara._2 = (n-1) * npp + (i+1);
        caras.push_back(cara);
        cara._0 = i;
        cara._1 = i+1;
        cara._2 = (n-1) * npp + (i+1);
        caras.push_back(cara);
    }
}

void _revolucion::rotar_con_tapa(vector<_vertex3f> perfil, int n){
    this->rotar(perfil, n);
    int npp = perfil.size();
    _vertex3f aux;
    _vertex3i cara;

    //Construir centro de la tapa inferior:
    aux.x = 0;
    aux.y = vertices[0].y;
    aux.z = 0;
    vertices.push_back(aux);
    //Construir centro de la tapa superior:
    aux.x = 0;
    aux.y = vertices[npp-1].y;
    aux.z = 0;
    vertices.push_back(aux);

    //Crear la tapa inferior:
    for(int j = 0; j<n-1; ++j){
        cara._0 = vertices.size() - 2;
        cara._1 = j * npp;
        cara._2 = (j+1) * npp;
        caras.push_back(cara);
    }
    //Tratar la última cara de la tapa inferior:
    cara._0 = vertices.size() - 2;
    cara._1 = (n-1) * npp;
    cara._2 = 0;
    caras.push_back(cara);

    //Crear la tapa superior:
    for(int j = 0; j<n-1; ++j){
        cara._0 = vertices.size() - 1;
        cara._1 = j * npp + (npp-1);
        cara._2 = (j+1) * npp + (npp -1);
        caras.push_back(cara);
    }
    //Tratar la última cara de la tapa superior:
    cara._0 = vertices.size() - 1;
    cara._1 = (n-1) * npp + (npp-1);
    cara._2 = npp - 1;
    caras.push_back(cara);
}

/** Representación:
 * Filas (j) -> rotaciones
 * Columnas (i) -> puntos
 * vertices[j][i] -> punto i-ésimo del perfil j-ésimo
**/ 
void _revolucion::rotar_X(vector<_vertex3f> perfil, int n){
    int npp = perfil.size();
    int n_vertices = npp * n;
    vertices.resize(n_vertices);
    _vertex3f aux;
    _vertex3i cara;

    //Rellenar el vector vértices con los puntos del perfil rotados:
    for(int i = 0; i<npp; ++i){
        for(int j = 0; j<n; ++j){
            aux.x = perfil[i].x;
            aux.y = perfil[i].y * cos(2.0*M_PI*j/n) - perfil[i].z * sin(2.0*M_PI*j/n);
            aux.z = perfil[i].y * sin(2.0*M_PI*j/n) + perfil[i].z * cos(2.0*M_PI*j/n);
            vertices[j * npp + i] = aux;
        }
    }

    //Crear las caras (menos la última):
    for(int j = 0; j<n-1; ++j){
        for(int i = 0; i<npp-1; ++i){
            cara._0 = j * npp +i;
            cara._1 = (j+1) * npp + i;
            cara._2 = j * npp + (i+1);
            caras.push_back(cara);
            cara._0 = (j+1) * npp + i;
            cara._1 = (j+1) * npp + (i+1);
            cara._2 = j * npp + (i+1);
            caras.push_back(cara);
        }
    }

    //Tratamiento de la última cara:
    for(int i = 0; i<npp-1; ++i){
        cara._0 = (n-1) * npp + i;
        cara._1 = i;
        cara._2 = (n-1) * npp + (i+1);
        caras.push_back(cara);
        cara._0 = i;
        cara._1 = i+1;
        cara._2 = (n-1) * npp + (i+1);
        caras.push_back(cara);
    }
}

void _revolucion::rotar_con_tapa_X(vector<_vertex3f> perfil, int n){
    this->rotar_X(perfil, n);
    int npp = perfil.size();
    _vertex3f aux;
    _vertex3i cara;

    //Construir centro de la tapa inferior:
    aux.x = vertices[0].x;
    aux.y = 0;
    aux.z = 0;
    vertices.push_back(aux);
    //Construir centro de la tapa superior:
    aux.x = vertices[npp-1].x;
    aux.y = 0;
    aux.z = 0;
    vertices.push_back(aux);

    //Crear la tapa inferior:
    for(int j = 0; j<n-1; ++j){
        cara._0 = vertices.size() - 2;
        cara._1 = j * npp;
        cara._2 = (j+1) * npp;
        caras.push_back(cara);
    }
    //Tratar la última cara de la tapa inferior:
    cara._0 = vertices.size() - 2;
    cara._1 = (n-1) * npp;
    cara._2 = 0;
    caras.push_back(cara);

    //Crear la tapa superior:
    for(int j = 0; j<n-1; ++j){
        cara._0 = vertices.size() - 1;
        cara._1 = j * npp + (npp-1);
        cara._2 = (j+1) * npp + (npp -1);
        caras.push_back(cara);
    }
    //Tratar la última cara de la tapa superior:
    cara._0 = vertices.size() - 1;
    cara._1 = (n-1) * npp + (npp-1);
    cara._2 = npp - 1;
    caras.push_back(cara);
}

///////////////////////////////////////////////////////////////////////////
// Clase objeto jerárquico: articulado
///////////////////////////////////////////////////////////////////////////

_articulado::_articulado(){
    vector<_vertex3f> perfil;

    cubo = new _cubo(0.5);
    cubo2 = new _cubo(0.5);
    cilindro = new _revolucion();
    base = new _revolucion();
    bisagra = new _revolucion();

    perfil.resize(2);
    perfil[0].x = 0.5; perfil[0].y = -0.5; perfil[0].z = 0.0;
    perfil[1].x = 0.5; perfil[1].y =  0.5; perfil[1].z = 0.0;

    cilindro->rotar_con_tapa(perfil, 30);
    bisagra->rotar_con_tapa(perfil, 30);

    perfil[0].x = 0.35; perfil[0].y =  0.0; perfil[0].z = 0.0;
    perfil[1].x =  1.5; perfil[1].y = -0.5; perfil[1].z = 0.0;
    
    base->rotar_con_tapa(perfil, 30);

    alfa = 0.0;
    beta = 0.0;
    gamma = 1.25;
}

_articulado::~_articulado(){
    delete cubo;
    delete cubo2;
    delete cilindro;
    delete base;
    delete bisagra;
}

void _articulado::draw_silla(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez){
    glPushMatrix();
        glTranslatef(0.0, 0.0, -1.75);
        glRotatef(90.0, 0, 0, 1);
        glScalef(0.5, 3.0, 0.5);
        if(setNames) glLoadName(6);
        bisagra->draw(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez);
    glPopMatrix();

    this->draw_asiento(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez);
    this->draw_respaldo(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez);
}

void _articulado::draw_asiento(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez){
    glPushMatrix();
        glRotatef(90.0, 1, 0, 0);
        glScalef(3.0, 3.0, 0.5);
        if(setNames) glLoadName(6);
        cubo2->draw(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez);
    glPopMatrix();
}

void _articulado::draw_respaldo(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez){
    glPushMatrix();
        glTranslatef(0.0, 0.0, -1.75);
        glRotatef(alfa, 1, 0, 0);
        glTranslatef(0.0, 1.75, 0.0);
        glScalef(3.0, 3.0, 0.5);
        if(setNames) glLoadName(5);
        cubo->draw(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez);
    glPopMatrix();
}

void _articulado::draw_hierarchy(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez){
    glPushMatrix();
        glTranslatef(0.0, 0.25+gamma, 0.0);
        glRotatef(beta, 0, 1, 0);
        this->draw_silla(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez);
    glPopMatrix();

    glPushMatrix();
        glScalef(0.7, gamma, 0.7);
        glTranslatef(0.0, 0.5, 0.0);
        if(setNames) glLoadName(7);
        cilindro->draw(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez);
    glPopMatrix();

    if(setNames) glLoadName(7);
    base->draw(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez);
}

void _articulado::change_parameters(float alfa, float beta, float gamma){
    if(this->alfa + alfa > MAX_ALFA)
        this->alfa = MAX_ALFA;
    else if(this->alfa + alfa < MIN_ALFA)
        this->alfa = MIN_ALFA;
    else
        this->alfa += alfa;

    this->beta += beta;

    if(this->gamma + gamma > MAX_GAMMA)
        this->gamma = MAX_GAMMA;
    else if(this->gamma + gamma < MIN_GAMMA)
        this->gamma = MIN_GAMMA;
    else
        this->gamma += gamma;
    
}
