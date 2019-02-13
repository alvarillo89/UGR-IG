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

void _triangulos3D::draw(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm){
    switch(mode){
        case GL_POINT: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); glPointSize(grosor); break;  
        case GL_LINE: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glLineWidth(grosor); break;
        case GL_FILL: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
    }

    if(ilm == 0)
        glColor3f(r1, g1, b1);
    else if(ilm == 1)
        glShadeModel(GL_FLAT);
    else
        glShadeModel(GL_SMOOTH);

    if(ilm != 0){
        glEnable(GL_LIGHTING);
        glEnable(GL_NORMALIZE);

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*) &ambiental);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*) &difusa);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*) &especular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);
    }

    glBegin(GL_TRIANGLES);
        for(int i = 0; i<caras.size(); ++i){

            if(mode == GL_FILL && ajedrez && ilm == 0){
                if(i % 2 == 0)
                    glColor3f(r1, g1, b1);
                else
                    glColor3f(r2, g2, b2);
            }

            if(ilm == 0){
                glVertex3f(vertices[caras[i]._0].x, vertices[caras[i]._0].y, vertices[caras[i]._0].z);
                glVertex3f(vertices[caras[i]._1].x, vertices[caras[i]._1].y, vertices[caras[i]._1].z);
                glVertex3f(vertices[caras[i]._2].x, vertices[caras[i]._2].y, vertices[caras[i]._2].z);
            }
            else if(ilm == 1){
                glNormal3fv((GLfloat*) &normales_caras[i]);
                glVertex3f(vertices[caras[i]._0].x, vertices[caras[i]._0].y, vertices[caras[i]._0].z);
                glVertex3f(vertices[caras[i]._1].x, vertices[caras[i]._1].y, vertices[caras[i]._1].z);
                glVertex3f(vertices[caras[i]._2].x, vertices[caras[i]._2].y, vertices[caras[i]._2].z);
            }
            else{
                glNormal3f(normales_vertices[caras[i]._0].x, normales_vertices[caras[i]._0].y, normales_vertices[caras[i]._0].z);
                glVertex3f(vertices[caras[i]._0].x, vertices[caras[i]._0].y, vertices[caras[i]._0].z);
                glNormal3f(normales_vertices[caras[i]._1].x, normales_vertices[caras[i]._1].y, normales_vertices[caras[i]._1].z);
                glVertex3f(vertices[caras[i]._1].x, vertices[caras[i]._1].y, vertices[caras[i]._1].z);
                glNormal3f(normales_vertices[caras[i]._2].x, normales_vertices[caras[i]._2].y, normales_vertices[caras[i]._2].z);
                glVertex3f(vertices[caras[i]._2].x, vertices[caras[i]._2].y, vertices[caras[i]._2].z);
            }
        }
    
    glEnd();
    glDisable(GL_LIGHTING);

}

void _triangulos3D::calcularNormalesCaras(){
    _vertex3f u, v, normal, normalizado;
    float mod;
    normales_caras.resize(caras.size());

    for(unsigned long i = 0; i<caras.size(); ++i){
        u.x = vertices[caras[i]._1].x - vertices[caras[i]._0].x;
        u.y = vertices[caras[i]._1].y - vertices[caras[i]._0].y;
        u.z = vertices[caras[i]._1].z - vertices[caras[i]._0].z;
        
        v.x = vertices[caras[i]._2].x - vertices[caras[i]._1].x;
        v.y = vertices[caras[i]._2].y - vertices[caras[i]._1].y;
        v.z = vertices[caras[i]._2].z - vertices[caras[i]._1].z;

        normal = u.cross_product(v);
        mod = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        normalizado.x = normal.x / mod;
        normalizado.y = normal.y / mod;
        normalizado.z = normal.z / mod;

        normales_caras[i] = normalizado;
    }
}

void _triangulos3D::calcularNormalesVertices(){
    normales_vertices.resize(vertices.size());
    float mod;

    for(unsigned long i = 0; i<normales_vertices.size(); ++i)
        normales_vertices[i] = _vertex3f(0.0, 0.0, 0.0);

    for(unsigned long i = 0; i<caras.size(); ++i){
        normales_vertices[caras[i]._0].x += normales_caras[i].x;
        normales_vertices[caras[i]._0].y += normales_caras[i].y;
        normales_vertices[caras[i]._0].z += normales_caras[i].z;
        
        normales_vertices[caras[i]._1].x += normales_caras[i].x;
        normales_vertices[caras[i]._1].y += normales_caras[i].y;
        normales_vertices[caras[i]._1].z += normales_caras[i].z;
        
        normales_vertices[caras[i]._2].x += normales_caras[i].x;
        normales_vertices[caras[i]._2].y += normales_caras[i].y;
        normales_vertices[caras[i]._2].z += normales_caras[i].z;
    }

    for(unsigned long i = 0; i<normales_vertices.size(); ++i){

        mod = sqrt(normales_vertices[i].x * normales_vertices[i].x 
                   + normales_vertices[i].y * normales_vertices[i].y 
                   + normales_vertices[i].z * normales_vertices[i].z);
        
        normales_vertices[i].x = normales_vertices[i].x / mod;
        normales_vertices[i].y = normales_vertices[i].y / mod;
        normales_vertices[i].z = normales_vertices[i].z / mod;
    }

}

void _triangulos3D::calcularNormales(){
    this->calcularNormalesCaras();
    this->calcularNormalesVertices();
}

void _triangulos3D::iluminacionPlana(){
    glShadeModel(GL_FLAT);
    glEnable(GL_NORMALIZE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_LIGHTING);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*) &ambiental);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*) &difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*) &especular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

    glBegin(GL_TRIANGLES);
        for(int i = 0; i<caras.size(); ++i){
            glNormal3fv((GLfloat*) &normales_caras[i]);
            glVertex3f(vertices[caras[i]._0].x, vertices[caras[i]._0].y, vertices[caras[i]._0].z);
            glVertex3f(vertices[caras[i]._1].x, vertices[caras[i]._1].y, vertices[caras[i]._1].z);
            glVertex3f(vertices[caras[i]._2].x, vertices[caras[i]._2].y, vertices[caras[i]._2].z);
        }
    glEnd();

    glDisable(GL_LIGHTING);
}
    
void _triangulos3D::iluminacionSuave(){
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_LIGHTING);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*) &ambiental);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*) &difusa);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*) &especular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

    glBegin(GL_TRIANGLES);
        for(int i = 0; i<caras.size(); ++i){
            glNormal3f(normales_vertices[caras[i]._0].x, normales_vertices[caras[i]._0].y, normales_vertices[caras[i]._0].z);
            glVertex3f(vertices[caras[i]._0].x, vertices[caras[i]._0].y, vertices[caras[i]._0].z);
            glNormal3f(normales_vertices[caras[i]._1].x, normales_vertices[caras[i]._1].y, normales_vertices[caras[i]._1].z);
            glVertex3f(vertices[caras[i]._1].x, vertices[caras[i]._1].y, vertices[caras[i]._1].z);
            glNormal3f(normales_vertices[caras[i]._2].x, normales_vertices[caras[i]._2].y, normales_vertices[caras[i]._2].z);
            glVertex3f(vertices[caras[i]._2].x, vertices[caras[i]._2].y, vertices[caras[i]._2].z);
        }
    glEnd();

    glDisable(GL_LIGHTING);
}


void _triangulos3D::setMaterials(_vertex4f ambiental, _vertex4f difusa, _vertex4f especular, float brillo){
    this->ambiental = ambiental;
    this->difusa = difusa;
    this->especular = especular;
    this->brillo = brillo;
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

    this->calcularNormalesCaras();

    //Las  normales a los vértices se calculan manualmente:
    normales_vertices.resize(vertices.size());
    float mod = sqrt(3 * (tam * tam));

    for(int i = 0; i<vertices.size(); ++i){
        normales_vertices[i].x = vertices[i].x / mod;
        normales_vertices[i].y = vertices[i].y / mod;
        normales_vertices[i].z = vertices[i].z / mod;  
    }

}

///////////////////////////////////////////////////////////////////////////
// Tablero
///////////////////////////////////////////////////////////////////////////

void _tablero::recalculate(){
    vertices[0].x = -tam/2.0; vertices[0].y = -tam/2.0; vertices[0].z = -0.01;
    vertices[1].x =  tam/2.0; vertices[1].y = -tam/2.0; vertices[1].z = -0.01;
    vertices[2].x =  tam/2.0; vertices[2].y =  tam/2.0; vertices[2].z = -0.01;
    vertices[3].x = -tam/2.0; vertices[3].y =  tam/2.0; vertices[3].z = -0.01;
    vertices[4].x = -tam/2.0+1; vertices[4].y = -tam/2.0+1; vertices[4].z = 0.0;
    vertices[5].x =  tam/2.0-1; vertices[5].y = -tam/2.0+1; vertices[5].z = 0.0;
    vertices[6].x =  tam/2.0-1; vertices[6].y =  tam/2.0-1; vertices[6].z = 0.0;
    vertices[7].x = -tam/2.0+1; vertices[7].y =  tam/2.0-1; vertices[7].z = 0.0;
}

_tablero::_tablero(float tam){
    
    vertices.resize(8);
    this->tam = tam;
    this->recalculate();

}

void _tablero::draw_textures(const char *img){
    vector<unsigned char> data;
    CImg<unsigned char> imagen;

    //Leer la imagen:
    imagen.load(img);

	//Cargamos el RGB de la imagen en el vector data:
	for(long y = 0; y < imagen.height(); y ++)
    	for(long x = 0; x < imagen.width(); x ++){
			unsigned char *r = imagen.data(x, y, 0, 0);
			unsigned char *g = imagen.data(x, y, 0, 1);
			unsigned char *b = imagen.data(x, y, 0, 2);
			data.push_back(*r);
			data.push_back(*g);
			data.push_back(*b);
		}
    
    //Colocado sobre el polígono: (Repetición)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Filtros:  (Filtrado)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //Activar las texturas:
    glEnable(GL_TEXTURE_2D);
    
    //Cargar la imagen que se va a user como textura:
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagen.width(), imagen.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);

    //Por si ha cambiado el tam:
    this->recalculate();

    //Dibujamos:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_POLYGON);
        glVertex3fv((GLfloat*) &vertices[0]);
        glVertex3fv((GLfloat*) &vertices[1]);
        glVertex3fv((GLfloat*) &vertices[2]);
        glVertex3fv((GLfloat*) &vertices[3]);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON);
        central? glTexCoord2f(0.25, 0.75) : glTexCoord2f(0.0, 1.0);
        glVertex3fv((GLfloat*) &vertices[4]);
        central? glTexCoord2f(0.75, 0.75) : glTexCoord2f(1.0, 1.0);
        glVertex3fv((GLfloat*) &vertices[5]);
        central? glTexCoord2f(0.75, 0.25) : glTexCoord2f(1.0, 0.0);
        glVertex3fv((GLfloat*) &vertices[6]);
        central? glTexCoord2f(0.25, 0.25) : glTexCoord2f(0.0, 0.0);
        glVertex3fv((GLfloat*) &vertices[7]);
    glEnd();

    //Desactivar Texturas:
    glDisable(GL_TEXTURE_2D);
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

    this->calcularNormales();
}

///////////////////////////////////////////////////////////////////////////
// Objeto PLY
///////////////////////////////////////////////////////////////////////////

_objetoPLY::_objetoPLY(){

}

void _objetoPLY::leer(const char* archivo){
    _file_ply File_ply;
    
    File_ply.open(archivo);
    File_ply.read(vertices, caras);
    File_ply.close();
    this->calcularNormales();
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

    this->calcularNormales();
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

    this->calcularNormales();
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

    this->calcularNormales();
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

    this->calcularNormales();
}

///////////////////////////////////////////////////////////////////////////
// Clase objeto jerárquico: articulado
///////////////////////////////////////////////////////////////////////////

_articulado::_articulado(){
    vector<_vertex3f> perfil;

    cubo = new _cubo(0.5);
    cilindro = new _revolucion();
    base = new _revolucion();

    perfil.resize(2);
    perfil[0].x = 0.5; perfil[0].y = -0.5; perfil[0].z = 0.0;
    perfil[1].x = 0.5; perfil[1].y =  0.5; perfil[1].z = 0.0;

    cilindro->rotar_con_tapa(perfil, 30);

    perfil[0].x = 0.35; perfil[0].y =  0.0; perfil[0].z = 0.0;
    perfil[1].x =  1.5; perfil[1].y = -0.5; perfil[1].z = 0.0;
    
    base->rotar_con_tapa(perfil, 30);

    alfa = 0.0;
    beta = 0.0;
    gamma = 1.25;
}

_articulado::~_articulado(){
    delete cubo;
    delete cilindro;
    delete base;
}

void _articulado::draw_silla(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm){
    glPushMatrix();
        glTranslatef(0.0, 0.0, -1.75);
        glRotatef(90.0, 0, 0, 1);
        glScalef(0.5, 3.0, 0.5);
        cilindro->draw(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez, ilm);
    glPopMatrix();

    this->draw_asiento(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez, ilm);
    this->draw_respaldo(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez, ilm);
}

void _articulado::draw_asiento(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm){
    glPushMatrix();
        glRotatef(90.0, 1, 0, 0);
        this->draw_bases(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez, ilm);
    glPopMatrix();
}

void _articulado::draw_respaldo(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm){
    glPushMatrix();
        glTranslatef(0.0, 0.0, -1.75);
        glRotatef(alfa, 1, 0, 0);
        glTranslatef(0.0, 1.75, 0.0);
        this->draw_bases(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez, ilm);
    glPopMatrix();
}

void _articulado::draw_bases(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm){
    glPushMatrix();
        glScalef(3.0, 3.0, 0.5);
        cubo->draw(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez, ilm);
    glPopMatrix();
}

void _articulado::draw_hierarchy(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm){
    glPushMatrix();
        glTranslatef(0.0, 0.25+gamma, 0.0);
        glRotatef(beta, 0, 1, 0);
        this->draw_silla(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez, ilm);
    glPopMatrix();

    glPushMatrix();
        glScalef(0.7, gamma, 0.7);
        glTranslatef(0.0, 0.5, 0.0);
        cilindro->draw(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez, ilm);
    glPopMatrix();

    base->draw(mode, r1, g1, b1, r2, g2, b2, grosor, ajedrez, ilm);
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
    
void _articulado::set_hierarchy_materials(_vertex4f ambiental, _vertex4f difusa, _vertex4f especular, float brillo){
    cubo->setMaterials(ambiental, difusa, especular, brillo);
    cilindro->setMaterials(ambiental, difusa, especular, brillo);
    base->setMaterials(ambiental, difusa, especular, brillo);
}
