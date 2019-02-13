/////////////////////////////////////////////////////////////////////////////
// Práctica 2 leer ficheros ply y objetos de revolución
/////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <stdlib.h>
#include <GL/gl.h>
#include "vertex.h"
#include "file_ply_stl.h"
#include "CImg.h"

using namespace std;
using namespace cimg_library;

const float AXIS_SIZE=5000;

///////////////////////////////////////////////////////////////////////////
// Clase punto
///////////////////////////////////////////////////////////////////////////

class _puntos3D {
public:
	_puntos3D();

	vector<_vertex3f> vertices;
};

///////////////////////////////////////////////////////////////////////////
// Clase triángulo
///////////////////////////////////////////////////////////////////////////

class _triangulos3D: public _puntos3D{
public:
	_triangulos3D();
	void draw(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm);
	void calcularNormales();
	void calcularNormalesCaras();
	void calcularNormalesVertices();
	void iluminacionPlana();
	void iluminacionSuave();
	void setMaterials(_vertex4f ambiental, _vertex4f difusa, _vertex4f especular, float brillo);

	vector<_vertex3i> caras;
	vector<_vertex3f> normales_caras;
	vector<_vertex3f> normales_vertices;
	
	//Materiales:
	_vertex4f ambiental;
	_vertex4f difusa;
	_vertex4f especular;
	float brillo;
};


///////////////////////////////////////////////////////////////////////////
// Clase cubo
///////////////////////////////////////////////////////////////////////////

class _cubo: public _triangulos3D{
public:
	_cubo(float tam=0.5);
};


///////////////////////////////////////////////////////////////////////////
// Clase pirámide
///////////////////////////////////////////////////////////////////////////

class _piramide: public _triangulos3D{
public:
	_piramide(float tam=0.5, float al=0.75);
};


///////////////////////////////////////////////////////////////////////////
// Clase tablero:
///////////////////////////////////////////////////////////////////////////
class _tablero: public _triangulos3D{
public:
	_tablero(float tam=1.0);
	void draw_textures(const char *img);
	void recalculate();

	float tam;
	//Si central está a true, se colocará solo el centro de la imagen.
	//En caso contrario se colocará toda la imagen.
	bool central = false; 
};


///////////////////////////////////////////////////////////////////////////
// Clase objeto ply
///////////////////////////////////////////////////////////////////////////

class _objetoPLY: public _triangulos3D{
public:
	_objetoPLY();
	void leer(const char* archivo);	
};

///////////////////////////////////////////////////////////////////////////
// Clase objeto revolución
///////////////////////////////////////////////////////////////////////////

class _revolucion: public _triangulos3D{
public:
	_revolucion();
	void rotar(vector<_vertex3f> perfil, int n);
	void rotar_con_tapa(vector<_vertex3f> perfil, int n);
	void rotar_X(vector<_vertex3f> perfil, int n);
	void rotar_con_tapa_X(vector<_vertex3f> perfil, int n);		
};

///////////////////////////////////////////////////////////////////////////
// Clase objeto jerárquico: articulado
///////////////////////////////////////////////////////////////////////////

class _articulado: public _triangulos3D{
public:
	_articulado();
	~_articulado(); //Destructor
	void draw_hierarchy(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm);
	void change_parameters(float alfa, float beta, float gamma);
	void set_hierarchy_materials(_vertex4f ambiental, _vertex4f difusa, _vertex4f especular, float brillo);

private:
	//Objetos simples que componen la jerarquía:
	_cubo *cubo;
	_revolucion *cilindro;
	_revolucion *base;

	//Parámetros de movimiento:
	float alfa;		//Respaldo
	float beta; 	//Giro
	float gamma; 	//Altura 

	//Límites de las articulaciones:
	const float MAX_ALFA = 0.0;
	const float MIN_ALFA = -50.0;
	const float MAX_GAMMA = 2.0;
	const float MIN_GAMMA = 0.5;

	//Funciones privadas utilizadas por draw_hierarchy:
	void draw_silla(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm);
	void draw_asiento(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm);
	void draw_respaldo(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm);
	void draw_bases(GLenum mode, float r1, float g1, float b1, float r2, float g2, float b2, int grosor, bool ajedrez, int ilm);

};
