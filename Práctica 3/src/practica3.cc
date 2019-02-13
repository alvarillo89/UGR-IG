//**************************************************************************
// Práctica 2
//
// Domingo Martin Perandres 2013-2016
//
// GPL
//**************************************************************************

#include "stdlib.h"
#include "stdio.h"
#include <GL/glut.h>
#include <ctype.h>
#include "objetos.h"

// tamaño de los ejes
//const int AXIS_SIZE=5000;

////////////////////////////////////////////////////////////////////////////
//Declaración de objetos.
_piramide miPiramide(5.0, 6.0);
_cubo miCubo(3.0);
_objetoPLY objeto;
_revolucion peon;
_articulado silla;
const float INC = 1.5;
const float INC2 = 0.05;

//Variables para el modo de visualización y la figura:
int figura = 4;
int view_mode = 0;  // 0 -> líneas, 1 -> puntos, 2 -> sólido, 3 -> ajedrez.

////////////////////////////////////////////////////////////////////////////

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Window_width,Window_height,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int UI_window_pos_x=50,UI_window_pos_y=50,UI_window_width=500,UI_window_height=500;

//**************************************************************************
//
//***************************************************************************

void clear_window()
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

glMatrixMode(GL_PROJECTION);
glLoadIdentity();

// formato(x_minimo,x_maximo, y_minimo, y_maximo,Front_plane, plano_traser)
//  Front_plane>0  Back_plane>PlanoDelantero)
glFrustum(-Window_width,Window_width,-Window_height,Window_height,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

// posicion del observador
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0,0,-Observer_distance);
glRotatef(Observer_angle_x,1,0,0);
glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
glBegin(GL_LINES);
// eje X, color rojo
glColor3f(1,0,0);
glVertex3f(-AXIS_SIZE,0,0);
glVertex3f(AXIS_SIZE,0,0);
// eje Y, color verde
glColor3f(0,1,0);
glVertex3f(0,-AXIS_SIZE,0);
glVertex3f(0,AXIS_SIZE,0);
// eje Z, color azul
glColor3f(0,0,1);
glVertex3f(0,0,-AXIS_SIZE);
glVertex3f(0,0,AXIS_SIZE);
glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//***************************************************************************

void draw_objects()
{
	if(figura == 0){
		switch(view_mode){
			case 0: miPiramide.draw(GL_LINE, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: miPiramide.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: miPiramide.draw(GL_FILL, 0.294, 0.000, 0.510, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: miPiramide.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else if(figura == 1){
		switch(view_mode){
			case 0: miCubo.draw(GL_LINE, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: miCubo.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: miCubo.draw(GL_FILL, 0.294, 0.000, 0.510, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: miCubo.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else if(figura == 2){
		switch(view_mode){
			case 0: objeto.draw(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: objeto.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: objeto.draw(GL_FILL, 0.294, 0.000, 0.510, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: objeto.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else if(figura == 3){
		switch(view_mode){
			case 0: peon.draw(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: peon.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: peon.draw(GL_FILL, 0.294, 0.000, 0.510, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: peon.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else{
		switch(view_mode){
			case 0: silla.draw_hierarchy(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: silla.draw_hierarchy(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: silla.draw_hierarchy(GL_FILL, 0.294, 0.000, 0.510, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: silla.draw_hierarchy(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
}


//**************************************************************************
//
//***************************************************************************

void draw_scene(void)
{

clear_window();
change_observer();
draw_axis();
draw_objects();
glutSwapBuffers();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
change_projection();
glViewport(0,0,Ancho1,Alto1);
glutPostRedisplay();
}


//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_keys(unsigned char Tecla1,int x,int y)
{

if (toupper(Tecla1)=='Q') exit(0);
else if(toupper(Tecla1) == '1') figura = 0; 
else if(toupper(Tecla1) == '2') figura = 1;
else if(toupper(Tecla1) == '3') figura = 2;
else if(toupper(Tecla1) == '4') figura = 3;
else if(toupper(Tecla1) == '5') figura = 4;
else if(toupper(Tecla1) == 'P') view_mode = 1;
else if(toupper(Tecla1) == 'L') view_mode = 0;
else if(toupper(Tecla1) == 'F') view_mode = 2;
else if(toupper(Tecla1) == 'C') view_mode = 3;
else if(Tecla1 == 'Z') silla.change_parameters(INC, 0.0, 0.0);
else if(Tecla1 == 'z') silla.change_parameters(-INC, 0.0, 0.0);
else if(Tecla1 == 'X') silla.change_parameters(0.0, INC, 0.0);
else if(Tecla1 == 'x') silla.change_parameters(0.0, -INC, 0.0);
else if(Tecla1 == 'A') silla.change_parameters(0.0, 0.0, INC2);
else if(Tecla1 == 'a') silla.change_parameters(0.0, 0.0, -INC2);
draw_scene();

}

//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_keys(int Tecla1,int x,int y)
{

switch (Tecla1){
	case GLUT_KEY_LEFT:Observer_angle_y--;break;
	case GLUT_KEY_RIGHT:Observer_angle_y++;break;
	case GLUT_KEY_UP:Observer_angle_x--;break;
	case GLUT_KEY_DOWN:Observer_angle_x++;break;
	case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
	case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;
	}
glutPostRedisplay();
}



//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
// se inicalizan la ventana y los planos de corte
Window_width=5;
Window_height=5;
Front_plane=10;
Back_plane=1000;

// se inicia la posicion del observador, en el eje z
Observer_distance=2*Front_plane;
Observer_angle_x=0;
Observer_angle_y=0;

// se indica cual sera el color para limpiar la ventana	(r,v,a,al)
// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
glClearColor(1,1,1,1);

// se habilita el z-bufer
glEnable(GL_DEPTH_TEST);
//
change_projection();
//
glViewport(0,0,UI_window_width,UI_window_height);
}


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char **argv)
{

// se llama a la inicialización de glut
glutInit(&argc, argv);

if(argc != 2){
	cout << "Modo de empleo: " << argv[0] << " <archivo>" << endl;
	exit(-1);
}

// Lectura de .ply
objeto.leer(argv[1]);

// Creación de los objetos por revolución.
vector<_vertex3f> perfil;
perfil.resize(12);
perfil[0].x = 4.0;		perfil[0].y = -5.0;		perfil[0].z = 0;
perfil[1].x = 4.0; 		perfil[1].y = -4.0;		perfil[1].z = 0;
perfil[2].x = 2.55; 	perfil[2].y = -3.29; 	perfil[2].z = 0;
perfil[3].x = 1.5; 		perfil[3].y = -2.0;		perfil[3].z = 0;
perfil[4].x = 1.5; 		perfil[4].y = 2.0; 		perfil[4].z = 0;
perfil[5].x = 2.1; 		perfil[5].y = 2.6; 		perfil[5].z = 0;
perfil[6].x = 1.1; 		perfil[6].y = 2.6; 		perfil[6].z = 0;
perfil[7].x = 1.89; 	perfil[7].y = 3.51; 	perfil[7].z = 0;
perfil[8].x = 2.08; 	perfil[8].y = 4.34; 	perfil[8].z = 0;
perfil[9].x = 1.89; 	perfil[9].y = 5.25; 	perfil[9].z = 0;
perfil[10].x = 1.3; 	perfil[10].y = 5.83; 	perfil[10].z = 0;
perfil[11].x = 0.38; 	perfil[11].y = 6.1; 	perfil[11].z = 0;
peon.rotar_con_tapa(perfil, 30);

// se indica las caracteristicas que se desean para la visualización con OpenGL
// Las posibilidades son:
// GLUT_SIMPLE -> memoria de imagen simple
// GLUT_DOUBLE -> memoria de imagen doble
// GLUT_INDEX -> memoria de imagen con color indizado
// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
// GLUT_DEPTH -> memoria de profundidad o z-bufer
// GLUT_STENCIL -> memoria de estarcido
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

// posicion de la esquina inferior izquierdad de la ventana
glutInitWindowPosition(UI_window_pos_x,UI_window_pos_y);

// tamaño de la ventana (ancho y alto)
glutInitWindowSize(UI_window_width,UI_window_height);

// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
// al bucle de eventos)
glutCreateWindow("Practica 3");

// asignación de la funcion llamada "dibujar" al evento de dibujo
glutDisplayFunc(draw_scene);
// asignación de la funcion llamada "cambiar_tamanio_ventana" al evento correspondiente
glutReshapeFunc(change_window_size);
// asignación de la funcion llamada "tecla_normal" al evento correspondiente
glutKeyboardFunc(normal_keys);
// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
glutSpecialFunc(special_keys);

// funcion de inicialización
initialize();

// inicio del bucle de eventos
glutMainLoop();
return 0;
}
