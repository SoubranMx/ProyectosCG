//#include <windows.h>  //Solo para Windows
//#include <GL/gl.h>     // The GL Header File
//#include <stdio.h>

//#include <math.h>
#include "Main.h"


class CFiguras
{
	public:

	float text_der;
	float text_izq;

	void esfera(GLfloat radio, int meridianos, int paralelos, GLuint text); //Funcíon creacion esfera
	void cilindro(float radio, float altura, int resolucion, GLuint text);	//Funcíon creacion cilindro
	void cono(float altura, float radio, int resolucion, GLuint text);		//Funcíon creacion cono
	void prisma_anun (GLuint text, GLuint text2);							//Funcíon creacion movimiento
	void prisma (float altura, float largo, float profundidad, GLuint text);//Funcíon creacion prisma
	void prisma2 (GLuint text, GLuint text2);
	void prisma3 (GLuint text, GLuint text2);
	void skybox(float altura, float largo, float profundidad, GLuint text);	//Funcion creacion cielo
	//void torus(GLfloat radioM, GLfloat radiom, int meridianos, int paralelos, GLuint text );
	void torus(GLfloat radioM, GLfloat radiom, int meridianos, int paralelos);

	/***************	A MANO	***************/

	void brick1_1_1(GLuint text, GLuint text2);	//Brick 1x1x1
	void brick1_2_1(GLuint text, GLuint text2);	//Brick 1x2x1
	void brick1_4_1(GLuint text, GLuint text2);	//Brick 1x4x1
	void brick1_8_1(GLuint text, GLuint text2);	//Brick 1x8x1
	void brick2_2_1(GLuint text, GLuint text2);	//Brick 2x2x1
	void brick2_3_1(GLuint text, GLuint text2);	//Brick 2x3x1
	void slope1_2(GLuint text, GLuint text2);
	void cornerBrick2_2(GLuint text, GLuint text2);
	void flat4_4(GLuint text, GLuint text2);
	void CFiguras::plancha(GLuint text, GLuint text2);
};