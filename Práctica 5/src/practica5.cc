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
int camera = 0;
int selected_camera = 3;

////////////////////////////////////////////////////////////////////////////

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;
GLfloat alzado_x, alzado_y;
GLfloat planta_x, planta_z;
GLfloat perfil_y, perfil_z;

//Escalas:
GLfloat factor_alzado;
GLfloat factor_planta;
GLfloat factor_perfil;

//Parámetros para la redimensión de la ventana:
GLint Ancho;
GLint Alto;

//Si está a true, el botón derecho del ratón está pulsado:
bool r_mouse_button = false; 

//Variables que capturan la posición del ratón:
int x_antes, y_antes;

//Indica qué parte del objeto jerárquico está seleccionado para poder
//moverlo con el teclado:
int aMover = -1;

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
glViewport(0,0,Ancho,Alto);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();

// formato(x_minimo,x_maximo, y_minimo, y_maximo,Front_plane, plano_traser)
//  Front_plane>0  Back_plane>PlanoDelantero)
glFrustum(-Window_width,Window_width,-Window_height,Window_height,Front_plane,Back_plane);

}

//**************************************************************************
// Funciones para definir las 4 vistas:
//***************************************************************************
void camara_alzado(){
	//Especificamos la matriz de proyección y la inicializamos:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Especificar cámara con proyección ortogonal:
	glOrtho(-10, 10, -10, 10, -100, 100);

	//Ponerlo mirando de frente:
	glScalef(-1, 1, 1);

	//Acercar/alejar cámara:
	glScalef(factor_alzado, factor_alzado, factor_alzado);

	//Especificar el wiew port:
	glViewport(0, Alto/2.0, Ancho/2.0, Alto/2.0);
}

void camara_planta(){
	//Especificamos la matriz de proyección y la inicializamos:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Especificar cámara con proyección ortogonal:
	glOrtho(-10, 10, -10, 10, -100, 100);

	//Colocamos la planta:
	glRotatef(90.0, 1.0, 0.0, 0.0);	

	//Acercar/alejar cámara:
	glScalef(factor_planta, factor_planta, factor_planta);

	//Especificar el wiew port:	
	glViewport(0, 0, Ancho/2.0, Alto/2.0);
}

void camara_perfil(){
	//Especificamos la matriz de proyección y la inicializamos:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Especificar cámara con proyección ortogonal:
	glOrtho(-10, 10, -10, 10, -100, 100);

	//Colocamos el perfil:
	glRotatef(90.0, 0.0, 1.0, 0.0);

	//Acercar/alejar cámara:
	glScalef(factor_perfil, factor_perfil, factor_perfil);

	//Especificar el wiew port:
	glViewport(Ancho/2.0, Alto/2.0, Ancho/2.0, Alto/2.0);
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

void change_observer_planta(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(planta_x, 0, planta_z);
}

void change_observer_alzado(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(alzado_x, alzado_y, 0);
}

void change_observer_perfil(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, perfil_y, perfil_z);
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
			case 0: miPiramide.draw(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: miPiramide.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: miPiramide.draw(GL_FILL, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: miPiramide.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else if(figura == 1){
		switch(view_mode){
			case 0: miCubo.draw(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: miCubo.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: miCubo.draw(GL_FILL, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: miCubo.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else if(figura == 2){
		switch(view_mode){
			case 0: objeto.draw(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: objeto.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: objeto.draw(GL_FILL, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: objeto.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else if(figura == 3){
		switch(view_mode){
			case 0: peon.draw(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: peon.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: peon.draw(GL_FILL, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: peon.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else{
		switch(view_mode){
			case 0: silla.draw_hierarchy(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: silla.draw_hierarchy(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: silla.draw_hierarchy(GL_FILL, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: silla.draw_hierarchy(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
}

//Dibuja los objetos pero colocándole un nombre a cada uno:
void draw_objects_con_nombres()
{
	glInitNames();
	glPushName(0);

	if(figura == 0){
		glLoadName(1);
		switch(view_mode){
			case 0: miPiramide.draw(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: miPiramide.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: miPiramide.draw(GL_FILL, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: miPiramide.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else if(figura == 1){
		glLoadName(2);
		switch(view_mode){
			case 0: miCubo.draw(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: miCubo.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: miCubo.draw(GL_FILL, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: miCubo.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else if(figura == 2){
		glLoadName(3);
		switch(view_mode){
			case 0: objeto.draw(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: objeto.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: objeto.draw(GL_FILL, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: objeto.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else if(figura == 3){
		glLoadName(4);
		switch(view_mode){
			case 0: peon.draw(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: peon.draw(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: peon.draw(GL_FILL, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: peon.draw(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
	}
	else{
		silla.setNames = true;
		switch(view_mode){
			case 0: silla.draw_hierarchy(GL_LINE, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, false); break;
			case 1: silla.draw_hierarchy(GL_POINT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.0, false); break;
			case 2: silla.draw_hierarchy(GL_FILL, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, false); break;
			case 3: silla.draw_hierarchy(GL_FILL, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, true); break;
		}
		silla.setNames = false;
	}
}


//**************************************************************************
//
//***************************************************************************

void draw_scene(void)
{
	if(camera == 0){
		selected_camera = 3;
		clear_window();
		change_projection();
		change_observer();
		draw_axis();
		draw_objects();
	}
	else{
		clear_window();
		camara_alzado();		 //Alzado
		change_observer_alzado();
		draw_axis();
		draw_objects();
		camara_planta();		 //Planta
		change_observer_planta();
		draw_axis();
		draw_objects();
		camara_perfil();		 //Perfil
		change_observer_perfil();
		draw_axis();
		draw_objects();
		change_projection();	 //Perspectiva
		glViewport(Ancho/2.0, 0, Ancho/2.0, Alto/2.0);
		change_observer();
		draw_axis();
		draw_objects();
	}
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
Alto = (GLint) Alto1;
Ancho = (GLint) Ancho1;
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
else if(Tecla1 == 'A' && aMover == 0) silla.change_parameters(INC, 0.0, 0.0);
else if(Tecla1 == 'a' && aMover == 0) silla.change_parameters(-INC, 0.0, 0.0);
else if(Tecla1 == 'A' && aMover == 1) silla.change_parameters(0.0, INC, 0.0);
else if(Tecla1 == 'a' && aMover == 1) silla.change_parameters(0.0, -INC, 0.0);
else if(Tecla1 == 'A' && aMover == 2) silla.change_parameters(0.0, 0.0, INC2);
else if(Tecla1 == 'a' && aMover == 2) silla.change_parameters(0.0, 0.0, -INC2);
glutPostRedisplay();

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
	case GLUT_KEY_LEFT:
		switch(selected_camera){
			case 0: alzado_x-=0.1; break;
			case 1: perfil_z+=0.1; break;
			case 2: planta_x+=0.1; break;
			case 3: Observer_angle_y--; break; 
		}
	break;
	case GLUT_KEY_RIGHT:
		switch(selected_camera){
			case 0: alzado_x+=0.1; break;
			case 1: perfil_z-=0.1; break;
			case 2: planta_x-=0.1; break;
			case 3: Observer_angle_y++; break; 
		}
	break;
	case GLUT_KEY_UP:
		switch(selected_camera){
			case 0: alzado_y-=0.1; break;
			case 1: perfil_y-=0.1; break;
			case 2: planta_z+=0.1; break;
			case 3: Observer_angle_x--; break; 
		}
	break;
	case GLUT_KEY_DOWN:
		switch(selected_camera){
			case 0: alzado_y+=0.1; break;
			case 1: perfil_y+=0.1; break;
			case 2: planta_z-=0.1; break;
			case 3: Observer_angle_x++; break; 
		} 
	break;
	case GLUT_KEY_PAGE_UP:
		switch(selected_camera){
			case 0: factor_alzado/=1.2; break;
			case 1: factor_perfil/=1.2; break;
			case 2: factor_planta/=1.2; break;
			case 3: Observer_distance*=1.2; break; 
		}
	break;
	case GLUT_KEY_PAGE_DOWN:
		switch(selected_camera){
			case 0: factor_alzado*=1.2; break;
			case 1: factor_perfil*=1.2; break;
			case 2: factor_planta*=1.2; break;
			case 3: Observer_distance/=1.2; break; 
		}
	break;
	case GLUT_KEY_F1: camera = (camera + 1) % 2; break;
	case GLUT_KEY_F2: selected_camera = (selected_camera + 1) % 4; break;
	}
glutPostRedisplay();
}

//***************************************************************************
// Funciones para la gestión del ratón:
//***************************************************************************

/////////////////////////////////////////////////////////////////
// Selección:
/////////////////////////////////////////////////////////////////

void procesar_hits(GLint hits, GLuint* buffer){

	//Si está en vista múltiple:
	if(camera == 1){
		cerr << "[!] La selección solo funciona en vista única [!]" << endl;
		return;
	}

	//Mostrar información:
	cout << "Hits: " << hits << endl;
	for(int i = 0; i<hits; ++i){
		cout << "Número: " << buffer[i*4] << endl;
		cout << "Min Z: " << buffer[i*4+1] << endl;
		cout << "Max Z: " << buffer[i*4+2] << endl;
		cout << "Nombre: " << buffer[i*4+3] << endl;
	}
	cout << endl;

	//Procesar cambio de color:
	if(hits == 0){
		miCubo.isSelected = false;
		miPiramide.isSelected = false;
		objeto.isSelected = false;
		peon.isSelected = false;
		silla.cubo->isSelected = false;
		silla.cubo2->isSelected = false;
		silla.bisagra->isSelected =false;
		silla.cilindro->isSelected = false;
		silla.base->isSelected = false;
		aMover = -1;
	}
	else{
		for(int i = 0; i<hits; ++i){
			switch(buffer[i*4+3]){
				case 1: 
					miCubo.isSelected = false;
					miPiramide.isSelected = true;
					objeto.isSelected = false;
					peon.isSelected = false;
					silla.cubo->isSelected = false;
					silla.cubo2->isSelected = false;
					silla.bisagra->isSelected =false;
					silla.cilindro->isSelected = false;
					silla.base->isSelected = false;
					aMover = -1;
				break;
				case 2:
					miCubo.isSelected = true;
					miPiramide.isSelected = false;
					objeto.isSelected = false;
					peon.isSelected = false;
					silla.cubo->isSelected = false;
					silla.cubo2->isSelected = false;
					silla.bisagra->isSelected = false;
					silla.cilindro->isSelected = false;
					silla.base->isSelected = false;
					aMover = -1;
				break;
				case 3:
					miCubo.isSelected = false;
					miPiramide.isSelected = false;
					objeto.isSelected = true;
					peon.isSelected = false;
					silla.cubo->isSelected = false;
					silla.cubo2->isSelected = false;
					silla.bisagra->isSelected = false;
					silla.cilindro->isSelected = false;
					silla.base->isSelected = false;
					aMover = -1;
				break;
				case 4:
					miCubo.isSelected = false;
					miPiramide.isSelected = false;
					objeto.isSelected = false;
					peon.isSelected = true;
					silla.cubo->isSelected = false;
					silla.cubo2->isSelected = false;
					silla.bisagra->isSelected = false;
					silla.cilindro->isSelected = false;
					silla.base->isSelected = false;
					aMover = -1;
				break;	
				case 5:
					silla.cubo->isSelected = true;
					silla.cubo2->isSelected = false;
					silla.bisagra->isSelected = false;
					silla.cilindro->isSelected = false;
					silla.base->isSelected = false;
					miCubo.isSelected = false;
					miPiramide.isSelected = false;
					objeto.isSelected = false;
					peon.isSelected = false;
					aMover = 0;
				break;
				case 6:
					silla.cubo->isSelected = false;
					silla.cubo2->isSelected = true;
					silla.bisagra->isSelected = true;
					silla.cilindro->isSelected = false;
					silla.base->isSelected = false;
					miCubo.isSelected = false;
					miPiramide.isSelected = false;
					objeto.isSelected = false;
					peon.isSelected = false;
					aMover = 1;
				break;
				case 7:
					silla.cubo->isSelected = false;
					silla.cubo2->isSelected = false;
					silla.bisagra->isSelected = false;
					silla.cilindro->isSelected = true;
					silla.base->isSelected = true;
					miCubo.isSelected = false;
					miPiramide.isSelected = false;
					objeto.isSelected = false;
					peon.isSelected = false;
					aMover = 2;
				break;
			}
		}
	}	
}

void pick(int x, int y){
	GLuint buffer[100] = {0};	//Buffer que contendrá lo seleccionado.
	GLint viewport[4];			//Para guardar la información del viewport.
	GLint hits = 0;				//Número de objetos encontrados.

	//Indicar a OpenGL el buffer de selección:
	glSelectBuffer(100, buffer);

	//Obtener los parámetros del viewport:
	glGetIntegerv(GL_VIEWPORT, viewport);

	//Modo selección:
	glRenderMode(GL_SELECT);
	
	// Fijar la transformación de proyección para la selección
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPickMatrix (x, (viewport[3] - y), 5.0, 5.0, viewport);
	glFrustum(-Window_width, Window_width, -Window_height, Window_height, Front_plane, Back_plane);
	
	// Dibujar los objetos con su nombre:
	glMatrixMode(GL_MODELVIEW);
	draw_objects_con_nombres();
	
	// Pasar OpenGL a modo render
	hits = glRenderMode(GL_RENDER);
	
	// Restablecer la transformación de proyección
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-Window_width, Window_width, -Window_height, Window_height, Front_plane, Back_plane);
	
	// Procesar el contenido del buffer de selección
	procesar_hits(hits, buffer);
	
	// Dibujar para actualizar cambios
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}


//Se llama cada vez que se cambia el estado de algún botón del ratón:
void clickRaton(int boton , int estado , int x , int y){
	if(boton == GLUT_RIGHT_BUTTON){
		if(estado == GLUT_DOWN){
			r_mouse_button = true;
			/* Inicializamos x_antes e y_antes con la posición en la que se hizo
			click. */ 
			x_antes = x;
			y_antes = y;
		}
		else
			r_mouse_button = false;	
	}
	else if(boton == GLUT_LEFT_BUTTON)
		if(estado == GLUT_DOWN)
			pick(x, y);
}


//Se llama cada vez que cambia la posición del ratón:
void ratonMovido(int x, int y){
	if(r_mouse_button){
		Observer_angle_x += y - y_antes;
		Observer_angle_y += x - x_antes;
		/* Mientras el ratón está pulsado y moviéndose hay que hacer estos cálculos.
		Como no se pulsa ningún botón del ratón hay que darle valor aquí */
		x_antes = x;	 
		y_antes = y; 
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
Observer_angle_x=30;
Observer_angle_y=20;
alzado_x = alzado_y = 0;
planta_x = planta_z = 0;
perfil_y = perfil_z = 0;
factor_alzado = factor_perfil = factor_planta = 1.0;

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
	cout << "Modo de empleo: " << argv[0] << " <archivo.ply>" << endl;
	exit(-1);
}

// Lectura de .ply
objeto.leer(argv[1]);


//Mostrar información sobre el programa:
cout << "***************************************************************************" << endl
	  << "***************** OBJETOS *************************************************" << endl 
     << "***************************************************************************" << endl
	  << "1 : Visualizar Pirámide" << endl
	  << "2 : Visualizar Cubo" << endl
	  << "3 : Visualizar objeto PLY" << endl
	  << "4 : Visualizar objeto por revolución" << endl
	  << "5 : Visualizar objeto jerárquico" << endl << endl
	  << "***************************************************************************" << endl
	  << "************ MODOS DE VISUALIZACIÓN ***************************************" << endl 
     << "***************************************************************************" << endl
	  << "P : Modo Puntos" << endl
	  << "L : Modo Líneas" << endl
	  << "F : Modo Relleno" << endl
	  << "C : Modo ajedrez" << endl << endl
	  << "***************************************************************************" << endl
	  << "******************* CÁMARAS ***********************************************" << endl 
     << "***************************************************************************" << endl
	  << "Flechas : Mover Cámara" << endl
	  << "AvPag / RePag : Acercar / alejar cámara" << endl
	  << "F1 : alternar entre cámara única y vistas múltiples" << endl
	  << "F2 : cambiar cámara seleccionada en vista múltiple" << endl
	  << "Botón derecho del ratón : mover cámara en perspectiva" << endl << endl
	  << "***************************************************************************" << endl
	  << "************* SELECCIÓN DE OBJETOS ****************************************" << endl 
     << "***************************************************************************" << endl
	  << "Botón izquierdo del ratón : seleccionar objeto" << endl
	  << "En el objeto jerárquico (silla), se pueden seleccionar tres partes :" << endl
	  << "	* Respaldo : mueve el resplado" << endl
	  << "	* Asiento : gira todo el asiento" << endl
	  << "	* Base : cambiar la altura de la silla" << endl
	  << "Una vez seleccionado, se podrá modificar su grado de libertad con" << endl
	  << "la tecla A/a (Distingue Mayúsculas y Minúsculas)" << endl
	  << "[!] Previamente deberá seleccionarse alguna parte del objeto jerárquico [!]" << endl
	  << "[!] La selección solo funciona con cámara única [!]" << endl;


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
glutCreateWindow("Practica 5");

// asignación de la funcion llamada "dibujar" al evento de dibujo
glutDisplayFunc(draw_scene);
// asignación de la funcion llamada "cambiar_tamanio_ventana" al evento correspondiente
glutReshapeFunc(change_window_size);
// asignación de la funcion llamada "tecla_normal" al evento correspondiente
glutKeyboardFunc(normal_keys);
// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
glutSpecialFunc(special_keys);

//asignación de funciones del ratón:
glutMouseFunc(clickRaton);
glutMotionFunc(ratonMovido);

// funcion de inicialización
initialize();

// inicio del bucle de eventos
glutMainLoop();
return 0;
}
