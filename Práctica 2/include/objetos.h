/////////////////////////////////////////////////////////////////////////////
// Práctica 2 leer ficheros ply y objetos de revolución
/////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include "file_ply_stl.h"

using namespace std;

const float AXIS_SIZE=5000;

///////////////////////////////////////////////////////////////////////////
// Clase punto
///////////////////////////////////////////////////////////////////////////

class _puntos3D {
public:
	_puntos3D();
	void draw_puntos(float r, float g, float b, int grosor);

	vector<_vertex3f> vertices;
};

///////////////////////////////////////////////////////////////////////////
// Clase triángulo
///////////////////////////////////////////////////////////////////////////

class _triangulos3D: public _puntos3D{
public:
	_triangulos3D();
	void draw_aristas(float r, float g, float b, int grosor);
	void draw_solido(float r, float g, float b);
	void draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2);

	vector<_vertex3i> caras;
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
// Clase objeto ply
///////////////////////////////////////////////////////////////////////////

class _objetoPLY: public _triangulos3D{
public:
	_objetoPLY();
	void leer(char* archivo);	
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