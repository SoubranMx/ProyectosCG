﻿//Semestre 2020 - 2
//************************************************************//
//************************************************************//
//************** Alumno (s): *********************************//
//*************											******//
//*************											******//
//************************************************************//
//************************************************************//

#include "Main.h"
#include "Resources/librerias/texture.h"
#include "Resources/librerias/figuras.h"
#include "Resources/librerias/Camera.h"
#include "Resources/librerias/cmodel/CModel.h"



//Solo para Visual Studio 2015 o mayor
#if (_MSC_VER >= 1900)
#   pragma comment( lib, "legacy_stdio_definitions.lib" )
#endif


// Variables used to calculate frames per second: (Windows)
DWORD dwFrames = 0;
DWORD dwCurrentTime = 0;
DWORD dwLastUpdateTime = 0;
DWORD dwElapsedTime = 0;

CCamera objCamera;
GLfloat g_lookupdown = 16.0f;    // Look Position In The Z-Axis (NEW) 

int font = (int)GLUT_BITMAP_HELVETICA_18;
/*/
GLfloat Diffuse[] = { 0.5f, 0.5f, 0.95, 1.0f };				// Diffuse Light Values
GLfloat Specular[] = { 1.0, 1.0, 1.0, 1.0 };				// Specular Light Values
GLfloat Position[] = { 0.0f, 27.0f, -5.0f, 0.0f };			// Light Position
GLfloat Position2[] = { 0.0f, 0.0f, -5.0f, 1.0f };			// Light Position
*/
/*
	********** LUCES **********
	https://docs.microsoft.com/en-us/windows/win32/opengl/gllightfv
	https://www.cs.brandeis.edu/~cs155/OpenGL%20Lecture_05_6.pdf

	Variables para el uso de Luces.
	Por ejemplo:
	Fuente de Luz Blanca	(para reflejar todo tipo de material y color correctamente)
	GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f }; 			// Ambient Light Values
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				// Diffuse Light Values
	GLfloat LightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };				// Specular Light Values

	Posiciones de la Luz

	LightPosition	si el valor w es 0.0, la Luz es tratada como luz direccional (lejana) {x,y,z,w}
	LightDirection	hacia donde se posiciona la luz

	GLfloat LightPosition[]= { 0.0f, 0.0f, 0.0f, 1.0f };	luz normal
	GLfloat LightPosition1[]= { 0.0f, 0.0f, 1.0f, 0.0f };	luz direccional

	Luz Puntual o Luz Spotlight:

	Con el uso de glLightfv (GL_LIGHTx, DEFINITION, params);
	Definiciones:
	Para definir la luz:
	GL_AMBIENT
	GL_DIFFUSE
	GL_SPECULAR

	Para definir aspectos de la luz:
	GL_POSITION		{x,y,z,w}	si w=0	Direccional
	GL_SPOT_DIRECTION	{x,y,z}		especifica la dirección a la que se dirige el haz de luz (Solo con SPOTLIGHT)
	GL_SPOT_EXPONENT	{?}	Teóricamente implica un valor entre 0 - 128 que dispersa o concentra la luz.
	GL_SPOT_CUTOFF		valor flotante que indica si una luz es de tipo SPOTLIGHT (0-90) o PUNTUAL (180)	Default 180

	Para atenuar:
	La funcion de atenuacion se da como F(d) = 1/ (a + bd + cd^2)
	Donde:
	a = GL_CONSTANT_ATTENUATION
	b = GL_LINEAR_ATTENUATION
	c = GL_QUADRATIC_ATTENUATION
	d = distancia computada

	Por default: no atenuacion => a=1, b=0, c=0

*/
//Luz blanca
GLfloat aten1[] = { 0.1 , 0.0, 0.0 };	//Atenuacion de Luz Puntual
GLfloat aten2[] = { 0.5 , 0.0, 0.0 };	//Atenuacion de Luz Spotlight
GLfloat atenTecho[] = { 0.1 , 0.0, 0.0 };	//Atenuacion de Luz Spotlight

GLfloat exponent[] = { 128.0, 0.0, 0.0 };


GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 			// Ambient Light Values
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };			// Diffuse Light Values
GLfloat LightSpecular[] = { 1.0, 1.0, 1.0, 1.0f };				// Specular Light Values

GLfloat LightPosition1[] = { 0.0f, 0.0f, 0.0f, 1.0f };			// Light Position
GLfloat LightPosition2[] = { 0.0f, 0.0f, 0.0f, 1.0f };			// Light Position
GLfloat LightPosition3[] = { 0.0f, 0.0f, 0.0f, 1.0f };			// Light Position

GLfloat LightDirection1[] = { 0.0f, 0.0f, -1.0f };			// Light Position
GLfloat LightDirection2[] = { 0.0f, 0.0f, -1.0f };			// Light Position
GLfloat LightDirection3[] = { 0.0f, 0.0f, -1.0f };			// Light Position

float LightAngle = 36.0f;

bool light = false;		// Luz ON/OFF
bool foco1 = false;
bool foco2 = false;
bool foco3 = false;

float angleX1 = 0.0;
float angleY1 = 0.0;
float angleX2 = 0.0;
float angleY2 = 0.0;
float angleX3 = 0.0;
float angleY3 = 0.0;

static int spin = 0;

/*
	********** MATERIALES **********

	Variables para el uso de materiales

	GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };					// Color background, las caras menos iluminadas tenderan a ser oscuras con 0,0,0,1
	GLfloat mat_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };					// Object Color main
	GLfloat mat_specular[] = { 0.0, 0.0, 1.0, 1.0 };				// Specular color
	GLfloat mat_shininess[] = { 10.0 };								// Mayor = mas concentrado, menor = mas disperso  MIN = 1 => brillo sobre toda la superficie.

	En cuanto la luz se activa, glColor ya no es utilizado, en cambio se utilizan los materiales.
	GL_FRONT hace referencia a la direccion de la normal. (glNormal3f)

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
*/
typedef struct material
{
	GLfloat Ka[4];
	GLfloat Kd[4];
	GLfloat Ks[4];
	GLfloat n[1];
}materialStruct;

void set_material(materialStruct mat) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat.Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat.Ks);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat.n);
};
// Algunos materiales	http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html

//Laton
materialStruct brass{
	{0.33, 0.22, 0.03, 1.0},	//Ka
	{0.78, 0.57, 0.11, 1.0},	//Kd
	{0.99, 0.91, 0.81, 1.0},	//Ks
	{27.8}						//n
};

materialStruct bronze{
	{0.2125, 0.1275, 0.054, 1.0},	//Ka
	{0.714, 0.4284, 0.18144, 1.0},	//Kd
	{0.3935, 0.272, 0.1667, 1.0},	//Ks
	{25.6}
};

materialStruct chrome{
	{0.25, 0.25, 0.25, 1.0},	//Ka
	{0.4, 0.4, 0.4, 1.0},	//Kd
	{0.7746, 0.7746, 0.7746, 1.0},	//Ks
	{76.8},
};

materialStruct polishedCopper{
	{0.2295, 0.08825, 0.0275, 1.0},	//Ka
	{0.5508, 0.2118, 0.066, 1.0},	//Kd
	{0.5806, 0.223257, 0.0695701, 1.0},	//Ks
	{51.2}
};

materialStruct gold{
	{0.24725, 0.1995, 0.0745, 1.0},	//Ka
	{0.75164, 0.60648, 0.22648, 1.0},	//Kd
	{0.628281, 0.555802, 0.366065, 1.0},	//Ks
	{51.2}
};

materialStruct polishedSilver{
	{0.23125, 0.23125, 0.23125, 1.0},	//Ka
	{0.2775, 0.2775, 0.2775, 1.0},	//Kd
	{0.773911, 0.773911, 0.773911, 1.0},	//Ks
	{89.6},
};

materialStruct ruby{
	{0.1745, 0.01175, 0.01175, 0.55},		//Ka
	{0.6142, 0.04136, 0.04136, 0.55},		//Kd
	{0.727811, 0.626959, 0.626959, 0.55},	//Ks
	{76.8}
};

materialStruct perl{
	{0.25, 0.20725, 0.20725, 0.922},		//Ka
	{1.0, 0.829, 0.829, 0.922},		//Kd
	{0.296648, 0.296648, 0.296648, 0.922},	//Ks
	{11.264}
};
/*
	********** TEXTURAS **********

	Texturas utilizadas en el proyecto.
		En Photoshop, no aparece TGA como export option, por tanto, => guardar como => cambiar extensión a Targa

	Necesito 2 colores diferentes pero del mismo tono (?) para diferenciar el cubo del cilindro, y que se note que es lego.
*/
CTexture cielo;

CTexture t_piso;
CTexture t_piso2;
CTexture t_dado;
CTexture t_madera;
CTexture t_madera2;
CTexture t_maderaO;
CTexture t_maderaO2;
CTexture t_rojo;
CTexture t_rojo2;
CTexture t_naranja;
CTexture t_naranja2;
CTexture t_carbon;
CTexture t_carbon2;
CTexture t_pasto;
CTexture t_pasto2;


/*
	********** MODELOS **********

	Modelos utilizados en el proyecto, tanto manuales como importados de 3DS Max
		Para usar Max, se exporta como 3DS.
		Para exportar solo una selección: export selected
*/
// Usadas para el skybox y pintar texto en pantalla
CFiguras fig1;
CFiguras fig3;

//Figuras a "mano"
CFiguras brick;
CFiguras pisoGeneral;
CFiguras silla;
CFiguras mesa;
CFiguras cuarto;
CFiguras puerta;

//Figuras de 3D Studio
CModel sofa;
CModel muebleTV;
CModel tv;
CModel librero;
CModel foco;
CModel lampara;
CModel lamparaCalle;

CModel chess;

CModel bPeon;
CModel bCaballo;
CModel bTorre;
CModel bAlfil;
CModel bRey;
CModel bReina;

CModel nPeon;
CModel nCaballo;
CModel nTorre;
CModel nAlfil;
CModel nRey;
CModel nReina;

/*
	********** Mover posiciones **********

	Utilizadas para trasladar objetos en tiempo de ejecución.
	Por ejemplo: glTranslatef(0.0 + trax, 0.0 + tray, 0.0 + traz);
*/
float trax = 0.0;
float tray = 0.0;
float traz = 0.0;

float ranX = 0.0;
float ranY = 0.0;
float ranZ = 0.0;

/*
	********** Mover texturas **********

	Utilizadas para mover las texturas de un objeto en tiempo de ejecución.
*/
float textX = 0.0;
float textY = 0.0;

/*
	********** Mover escalas **********

	Utilizadas para escalar en ejecución algún objeto al que se le agreguen estas escalas
	por ejemplo: glScalef(1.0 + scaleX, 1.0 + scaleY, 1.0 + scaleZ);
	Para mover las escalas, se utilizan teclas asignadas
*/
float scaleX = 0.0;
float scaleY = 0.0;
float scaleZ = 0.0;
float sc = 1.0;

/*
	********** BANDERAS **********

	Se utiliza para saber en qué cámara se supone que esta el visualizador.
	*banderaCJ	Camara Juego
	*banderaCC	Camara Cuarto
	*banderaCO	Camara Original

	Se utilizan para saber si alguna tecla de animación fue presionada
	*banderaPuerta	se activa la animación de abrir/cerrar puerta
	*banderaVentana	abrir/cerrar ventana
	*banderaSilla	inicia animación de movimiento de silla con rotación y de vuelta al punto origen
	*banderaUpDown	para saber si: abre o cierra (puerta, ventana), desplaza o gira (silla)
	*banderaTrans	para saber si la silla está en movimiento translate o rotación.
*/
bool banderaCJ = false;	//Visualización de Camara enfocada en el Juego
bool banderaCC = false;	//Visualización de Camara enfocada en el cuarto
bool banderaCO = false;	//Visualización de Camara original
bool banderaPuerta = false;
bool banderaVentana = false;
bool banderaSilla = false;
bool banderaUpDown = false;
bool banderaTrans = true;
bool banderaDado = false;


/*
	********** ANIMACION **********

	*rotPuerta se utiliza como global para hacer rotar la Puerta en el eje Y
	*rotVentana para el eje X
	*rotSilla para el eje Y
	*transSilla para el desplazamiento (translatef) en Z de la silla
	*rotSillon fue utilizada para rotar algun objeto mediante una tecla, en el proyecto final no tiene utilidad
*/
float rotPuerta = 0.0f;
float rotVentana = 0.0f;
float rotSilla = 0.0f;
float transSilla = 0.0f;
float rotDado = 0.0f;

float rotSillon = 0.0;

/*
	********** CAMARA **********

	*C define posiciones de la cámara para la "1a camara" que enfoca el Cuarto general
	*J define posiciones de la cámara para la "2a cámara" que enfoca el juego.

	*Se guardan en estos valores para que al hacer cambio de vista, se quede donde estaba en el estado anterior.
	*Es decir, al cambiar de Cuarto->Juego, la cámara cambia a los valores de Camara de Juego, guardando los valores
	 de la Camara de Cuarto.
	*Cuando se cambie de Juego->Cuarto, la camara cambia a los valores de Camara de Cuarto.
	*Si los valores han sido cambiados anteriormente, la camara se dirigira a las posiciones previamente guardadas.
*/

float pos_xC, pos_yC, pos_zC, view_xC, view_yC, view_zC, up_xC, up_yC, up_zC;
float lookUpDownC;
float pos_xJ, pos_yJ, pos_zJ, view_xJ, view_yJ, view_zJ, up_xJ, up_yJ, up_zJ;
float lookUpDownJ;

/*
	**********	KEY FRAMES	**********

	Variables utilizadas para el uso de KeyFrames.
	Como la animación por KeyFrames solo preveo 6 o 7 movimientos, solo utilizaré 7 piezas de ajedrez.
	Algunas solo deben moverse hacia enfrente, como los peones. Otros, tanto en x como en z, y solo uno en las 3 posiciones para salir del tablero.

	*pBz	pieza B eje z
	*pCx	pieza C eje x
	*pFy	pieza F eje y
	*And so on...

	*MAX_FRAMES indica el numero de frames que se pueden guardar como máximo.
	*i_max_steps	Cantidad de cuadros intermedios
	*i_curr_steps	Cantidad de cuadros actuales
	*FRAME KeyFrame[MAX_FRAMES]	aqui se guardan los valores de los KeyFrames. Deben tomarse en cuenta todos los valores que estarán en rastreo y sus incrementos
	*FrameIndex	¿En qué frame voy?
	*play	Inicia o pausa la animación
	*playIndex	Utiliza el frame actual para acceder a la estructura FRAME y hacer cálculos en la interpolación.
	*w y h no fueron utilizados
	*frame, time, timebase y s... no estoy segura.

	Las funciones:
	*saveFrame	guarda los valores de los KeyFrames en la estructura FRAME[index] e incrementa el index
	*resetElements	para reiniciar la animación, pone en 0 current frame
	*interpolation	calcula los valores de interpolación para la animación fluida del KeyFrame. Más fluido mientras más pasos se pongan en max_steps
*/

float pBz = 0.0, pCx = 0.0, pCz = 0.0, pDx = 0.0, pDz = 0.0, pFx = 0.0, pFy = 0.0, pFz = 0.0, pGz = 0.0;
#define MAX_FRAMES 15		//15 keyframes
int i_max_steps = 90;		//Cantidad de cuadros intermedios	valores peque�os: animacion r�pida, valores grandes: animaci�n m�s pausada
int i_curr_steps = 0;

typedef struct _frame {
	//Variables para guardar KeyFrames
	float pBz;
	float pCx;
	float pCz;
	float pDx;
	float pDz;
	float pFx;
	float pFy;
	float pFz;
	float pGz;

	//Incrementos
	float pBzInc;
	float pCxInc;
	float pCzInc;
	float pDxInc;
	float pDzInc;
	float pFxInc;
	float pFyInc;
	float pFzInc;
	float pGzInc;
} FRAME;
FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;
int w = 500, h = 500;
int frame = 0, time, timebase = 0;
char s[30];


//	*********************	KEYFRAMES ***************			

void saveFrame(void) {
	printf("frameindex %d\n", FrameIndex);
	KeyFrame[FrameIndex].pBz = pBz;
	KeyFrame[FrameIndex].pCx = pCx;
	KeyFrame[FrameIndex].pCz = pCz;
	KeyFrame[FrameIndex].pDx = pDx;
	KeyFrame[FrameIndex].pDz = pDz;
	KeyFrame[FrameIndex].pFx = pFx;
	KeyFrame[FrameIndex].pFy = pFy;
	KeyFrame[FrameIndex].pFz = pFz;
	KeyFrame[FrameIndex].pGz = pGz;

	FrameIndex++;
}
void resetElements(void)
{
	pBz = KeyFrame[0].pBz;
	pCx = KeyFrame[0].pCx;
	pCz = KeyFrame[0].pCz;
	pDx = KeyFrame[0].pDx;
	pDz = KeyFrame[0].pDz;
	pFx = KeyFrame[0].pFx;
	pFy = KeyFrame[0].pFy;
	pFz = KeyFrame[0].pFz;
	pGz = KeyFrame[0].pGz;
}
void interpolation(void)
{
	//Los incrementos
	KeyFrame[playIndex].pBzInc = (KeyFrame[playIndex + 1].pBz - KeyFrame[playIndex].pBz) / i_max_steps;
	KeyFrame[playIndex].pCxInc = (KeyFrame[playIndex + 1].pCx - KeyFrame[playIndex].pCx) / i_max_steps;
	KeyFrame[playIndex].pCzInc = (KeyFrame[playIndex + 1].pCz - KeyFrame[playIndex].pCz) / i_max_steps;
	KeyFrame[playIndex].pDxInc = (KeyFrame[playIndex + 1].pDx - KeyFrame[playIndex].pDx) / i_max_steps;
	KeyFrame[playIndex].pDzInc = (KeyFrame[playIndex + 1].pDz - KeyFrame[playIndex].pDz) / i_max_steps;
	KeyFrame[playIndex].pFxInc = (KeyFrame[playIndex + 1].pFx - KeyFrame[playIndex].pFx) / i_max_steps;
	KeyFrame[playIndex].pFyInc = (KeyFrame[playIndex + 1].pFy - KeyFrame[playIndex].pFy) / i_max_steps;
	KeyFrame[playIndex].pFzInc = (KeyFrame[playIndex + 1].pFz - KeyFrame[playIndex].pFz) / i_max_steps;
	KeyFrame[playIndex].pGzInc = (KeyFrame[playIndex + 1].pGz - KeyFrame[playIndex].pGz) / i_max_steps;
}

//	*************	KEY FRAMES	*********************

void InitGL(GLvoid)     // Inicializamos parametros
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Negro de fondo	

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);									// Configuramos Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Habilitamos Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a realizar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	/* setup blending */
	glEnable(GL_BLEND);			// Turn Blending On

/*
	**********	LUCES	**********
	//gLightfv(light,pname,params)
*/
	//Puntual		Foco de lampara de exterior
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);					// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);					// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, aten1);

	//Spotlight		Foco de lámpara
	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);					// Setup The Ambient Light
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse);					// Setup The Diffuse Light
	glLightfv(GL_LIGHT2, GL_SPECULAR, LightSpecular);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT2, GL_LINEAR_ATTENUATION, aten2);

	//Puntual		Foco del Techo
	glLightfv(GL_LIGHT3, GL_AMBIENT, LightAmbient);					// Setup The Ambient Light
	glLightfv(GL_LIGHT3, GL_DIFFUSE, LightDiffuse);					// Setup The Diffuse Light
	glLightfv(GL_LIGHT3, GL_SPECULAR, LightSpecular);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT3, GL_LINEAR_ATTENUATION, atenTecho);

	/*
		********** TEXTURAS **********

		Carga de texturas.
		Debe ser cuadrada o casi cuadrada porque si no ... crashea el programa.
		CTexture t_piso;
		CTexture t_dado;
		CTexture t_madera;
		CTexture t_maderaO;
		CTexture t_rojo;
		CTexture t_naranja;
		CTexture carbon;
		CTexture pasto;
	*/
	cielo.LoadBMP("Resources/Texturas/cielo.bmp");
	cielo.BuildGLTexture();
	cielo.ReleaseImage();

	t_dado.LoadTGA("Resources/Texturas/piso.tga");
	t_dado.BuildGLTexture();
	t_dado.ReleaseImage();

	t_piso.LoadTGA("Resources/Texturas/cemento.tga");
	t_piso.BuildGLTexture();
	t_piso.ReleaseImage();

	t_piso2.LoadTGA("Resources/Texturas/piso2.tga");
	t_piso2.BuildGLTexture();
	t_piso2.ReleaseImage();

	t_carbon.LoadTGA("Resources/Texturas/carbon.tga");
	t_carbon.BuildGLTexture();
	t_carbon.ReleaseImage();
	t_carbon2.LoadTGA("Resources/Texturas/carbon2.tga");
	t_carbon2.BuildGLTexture();
	t_carbon2.ReleaseImage();

	t_naranja.LoadTGA("Resources/Texturas/naranja.tga");
	t_naranja.BuildGLTexture();
	t_naranja.ReleaseImage();
	t_naranja2.LoadTGA("Resources/Texturas/naranja2.tga");
	t_naranja2.BuildGLTexture();
	t_naranja2.ReleaseImage();
	
	t_rojo.LoadTGA("Resources/Texturas/rojo.tga");
	t_rojo.BuildGLTexture();
	t_rojo.ReleaseImage();
	t_rojo2.LoadTGA("Resources/Texturas/rojo2.tga");
	t_rojo2.BuildGLTexture();
	t_rojo2.ReleaseImage();

	t_madera.LoadTGA("Resources/Texturas/madera.tga");
	t_madera.BuildGLTexture();
	t_madera.ReleaseImage();
	t_madera2.LoadTGA("Resources/Texturas/madera2.tga");
	t_madera2.BuildGLTexture();
	t_madera2.ReleaseImage();

	t_maderaO.LoadTGA("Resources/Texturas/maderaOscura.tga");
	t_maderaO.BuildGLTexture();
	t_maderaO.ReleaseImage();
	t_maderaO2.LoadTGA("Resources/Texturas/maderaOscura2.tga");
	t_maderaO2.BuildGLTexture();
	t_maderaO2.ReleaseImage();

	t_pasto.LoadTGA("Resources/Texturas/pasto.tga");
	t_pasto.BuildGLTexture();
	t_pasto.ReleaseImage();
	t_pasto2.LoadTGA("Resources/Texturas/pasto2.tga");
	t_pasto2.BuildGLTexture();
	t_pasto2.ReleaseImage();
	/*
		********** FIGURAS **********

		Carga de figuras 3DS.
		Casi siempre debe ir escrito como .3DS ,en mayusculas.
		Algunas veces crashea en la ejecución, asi que se abre en 3D MAX y se vuelve a exportar como 3DS, esperando que funcione
	*/

	

	/*
		********** CAMARAS **********

		Inicialización de las posiciones de las cámaras: original (cuando carga), cuarto y juego.
		Teniendo en cuenta los parámteros:

		Posición de cámara inicial

			mPosX 0.13	mPosY 3.2	mPosZ 8.95
			mViewX 0.13	mViewY 3.2	mViewZ 5.95
			mUpX 0.0	mUpY 1.0	mUpZ 0.0
			glookupdown 0.0
	*/

	objCamera.Position_Camera(4.36f, 15.8f, 30.82f, 4.15f, 15.8f, 27.82f, 0, 1, 0);

	//Posiciones de cámaras para Juego y Cuarto iniciales
	pos_xC = 6.945;
	pos_yC = 17.2;
	pos_zC = 7.034;
	view_xC = 5.35;
	view_yC = 17.2;
	view_zC = 4.485;
	up_xC = 0.0;
	up_yC = 1.0;
	up_zC = 0.0;
	lookUpDownC = 46.0;

	pos_xJ = -4.1;
	pos_yJ = 9.5;
	pos_zJ = 0.04;
	view_xJ = -4.1;
	view_yJ = 9.5;
	view_zJ = -2.96;
	up_xJ = 0.0;
	up_yJ = 1.0;
	up_zJ = 0.0;
	lookUpDownJ = 73.0;
	/*
			********** KEY FRAMES **********

			Inicialización de KeyFrames.

			Primero pone todos los valores permitidos en FRAME como 0
			Luego, añadimos manualmente frames previamente analizados.

	*/
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].pBz = 0;
		KeyFrame[i].pCx = 0;
		KeyFrame[i].pCz = 0;
		KeyFrame[i].pDx = 0;
		KeyFrame[i].pDz = 0;
		KeyFrame[i].pFx = 0;
		KeyFrame[i].pFy = 0;
		KeyFrame[i].pFz = 0;
		KeyFrame[i].pGz = 0;

		KeyFrame[i].pBzInc = 0;
		KeyFrame[i].pCxInc = 0;
		KeyFrame[i].pCzInc = 0;
		KeyFrame[i].pDxInc = 0;
		KeyFrame[i].pDzInc = 0;
		KeyFrame[i].pFxInc = 0;
		KeyFrame[i].pFyInc = 0;
		KeyFrame[i].pFzInc = 0;
		KeyFrame[i].pGzInc = 0;
	}
}

void pintaTexto(float x, float y, float z, void* font, char* string)
{

	char* c;     //Almacena los caracteres a escribir
	glRasterPos3f(x, y, z);	//Posicion apartir del centro de la ventana
	//glWindowPos2i(150,100);
	for (c = string; *c != '\0'; c++) //Condicion de fin de cadena
	{
		glutBitmapCharacter(font, *c); //imprime
	}
}

/*********** PLANCHA ***********/
void createCemento() {
	glPushMatrix();
		glTranslatef(-82.0, 0.0, 0.0);
		//Parte A
		glPushMatrix();
			glTranslatef(2.0, 0.0, 0.0);
			brick.flatV(4.0, 90.0, t_piso.GLindex, t_piso2.GLindex);
		glPopMatrix();
		//Parte B
		glPushMatrix();
			glTranslatef(34.0, 0.0, 0.0);
			brick.flatV(60.0, 30.0, t_piso.GLindex, t_piso2.GLindex);
		glPopMatrix();
		//Parte C
		glPushMatrix();
			glTranslatef(74.0, 0.0, 0.0);
			brick.flatV(20.0, 90.0, t_piso.GLindex, t_piso2.GLindex);
		glPopMatrix();
		//Parte D
		glPushMatrix();
			glTranslatef(124.0, 0.0, 0.0);
			brick.flatV(80.0, 30.0, t_piso.GLindex, t_piso2.GLindex);
		glPopMatrix();
	glPopMatrix();
}
void createCesped() {
	glPushMatrix();
		glPushMatrix();
			glTranslatef(-82.0, 0.0, 0.0);
			//Parte B
			glPushMatrix();
				glTranslatef(34.0, 0.0, 30.0);
				brick.flatV(60.0, 30.0, t_pasto.GLindex, t_pasto2.GLindex);
			glPopMatrix();
			//Parte A
			glPushMatrix();
				glTranslatef(34.0, 0.0, -30.0);
				brick.flatV(60.0, 30.0, t_pasto.GLindex, t_pasto2.GLindex);
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(82.0, 0.0, 0.0);
			//Parte D
			glPushMatrix();
				glTranslatef(-40.0, 0.0, 30.0);
				brick.flatV(80.0, 30.0, t_pasto.GLindex, t_pasto2.GLindex);
			glPopMatrix();
			//Parte C
			glPushMatrix();
				glTranslatef(-40.0, 0.0, -30.0);
				brick.flatV(80.0, 30.0, t_pasto.GLindex, t_pasto2.GLindex);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}
void createPlancha() {
	glPushMatrix();
		set_material(brass);
		createCesped();
		set_material(polishedSilver);
		createCemento();
	glPopMatrix();
}
/*********** FENCES ***********/
void fence3(GLuint text, GLuint text2) {
	glPushMatrix();
		//brick.brick1_1_1(text, text2);
		brick.prisma(1.0, 1.0, 1.0, text);
		glTranslatef(0.0, 1.0, 0.0);
		//brick.brick1_1_1(text, text2);
		brick.prisma(1.0, 1.0, 1.0, text);
		glPushMatrix();
			glTranslatef(1.0, 0.0, 0.0);
			brick.brick1_1_1(text, text2);
			glTranslatef(1.0, 0.0, 0.0);
			brick.brick1_1_1(text, text2);
		glPopMatrix();
		glTranslatef(0.0, 1.0, 0.0);
		brick.brick1_1_1(text, text2);
	glPopMatrix();
}
void fence4(GLuint text, GLuint text2) {
	glPushMatrix();
		//brick.brick1_1_1(text, text2);
		brick.prisma(1.0, 1.0, 1.0, text);
		glTranslatef(0.0, 1.0, 0.0);
		brick.prisma(1.0, 1.0, 1.0, text);
		//brick.brick1_1_1(text, text2);
		glPushMatrix();
			glTranslatef(1.0, 0.0, 0.0);
			brick.brick1_1_1(text, text2);
			glTranslatef(1.0, 0.0, 0.0);
			brick.brick1_1_1(text, text2);
			glTranslatef(1.0, 0.0, 0.0);
			brick.brick1_1_1(text, text2);
		glPopMatrix();
		glTranslatef(0.0, 1.0, 0.0);
		brick.brick1_1_1(text, text2);
	glPopMatrix();
}
void fenceA() {
	glPushMatrix();
		glTranslatef(23.0, 0.0, 0.0);
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		for (int i = 0; i < 10; i++) {
			fence3(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(3.0, 0.0, 0.0);
		}
		glRotatef(90.0, 0.0, 1.0, 0.0);
		for (int j = 0; j < 10; j++) {
			fence4(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(4.0, 0.0, 0.0);
		}
		glRotatef(90.0, 0.0, 1.0, 0.0);
		for (int i = 0; i < 10; i++) {
			fence3(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(3.0, 0.0, 0.0);
		}
		glPushMatrix();
			brick.prisma(1.0, 1.0, 1.0, t_madera.GLindex);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, 1.0, 1.0, t_madera.GLindex);
			glTranslatef(0.0, 1.0, 0.0);
			brick.brick1_1_1(t_madera.GLindex, t_maderaO.GLindex);
		glPopMatrix();
	glPopMatrix();
}
void fenceB() {
	glPushMatrix();
		glTranslatef(83.0, 0.0, 0.0);
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		for (int i = 0; i < 10; i++) {
			fence3(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(3.0, 0.0, 0.0);
		}
		glRotatef(90.0, 0.0, 1.0, 0.0);
		for (int j = 0; j < 20; j++) {
			fence4(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(4.0, 0.0, 0.0);
		}
		glRotatef(90.0, 0.0, 1.0, 0.0);
		for (int i = 0; i < 10; i++) {
			fence3(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(3.0, 0.0, 0.0);
		}
		glPushMatrix();
			brick.prisma(1.0, 1.0, 1.0, t_madera.GLindex);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, 1.0, 1.0, t_madera.GLindex);
			glTranslatef(0.0, 1.0, 0.0);
			brick.brick1_1_1(t_madera.GLindex, t_maderaO.GLindex);
		glPopMatrix();
	glPopMatrix();
}
void fenceC() {
	glPushMatrix();
		glTranslatef(83.0, 0.0, 89.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		for (int i = 0; i < 10; i++) {
			fence3(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(3.0, 0.0, 0.0);
		}
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		for (int j = 0; j < 20; j++) {
			fence4(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(4.0, 0.0, 0.0);
		}
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		for (int i = 0; i < 10; i++) {
			fence3(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(3.0, 0.0, 0.0);
		}
		glPushMatrix();
			brick.prisma(1.0, 1.0, 1.0, t_madera.GLindex);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, 1.0, 1.0, t_madera.GLindex);
			glTranslatef(0.0, 1.0, 0.0);
			brick.brick1_1_1(t_madera.GLindex, t_maderaO.GLindex);
		glPopMatrix();
	glPopMatrix();
}
void fenceD() {
	glPushMatrix();
		glTranslatef(23.0, 0.0, 89.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		for (int i = 0; i < 10; i++) {
			fence3(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(3.0, 0.0, 0.0);
		}
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		for (int j = 0; j < 10; j++) {
			fence4(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(4.0, 0.0, 0.0);
		}
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		for (int i = 0; i < 10; i++) {
			fence3(t_madera.GLindex, t_maderaO.GLindex);
			glTranslatef(3.0, 0.0, 0.0);
		}
		glPushMatrix();
			brick.prisma(1.0, 1.0, 1.0, t_madera.GLindex);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, 1.0, 1.0, t_madera.GLindex);
			glTranslatef(0.0, 1.0, 0.0);
			brick.brick1_1_1(t_madera.GLindex, t_maderaO.GLindex);
		glPopMatrix();
	glPopMatrix();
}
void createFences() {
	glPushMatrix();
		glTranslatef(-81.5, 0.0, -44.5);
		set_material(bronze);
		fenceA();
		fenceB();
		fenceC();
		fenceD();
	glPopMatrix();
};
/*********** TIENDAS ***********/
/*********** BANCAS	 ***********/
void createBanca(GLuint text, GLuint text2, GLuint text3) {
	glPushMatrix();
		// Pata izquierda
		glPushMatrix();
			glTranslatef(-1.5, 0.5, 0.0);
			brick.prisma(1.0, 1.0, 1.0, text);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, 1.0, 1.0, text);
		glPopMatrix();
		// Pata Derecha
		glPushMatrix();
			glTranslatef(1.5, 0.5, 0.0);
			brick.prisma(1.0, 1.0, 1.0, text);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, 1.0, 1.0, text);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 2.15, 0.0);
			brick.flatV(8.0, 3.0, text3, text2);
		glPopMatrix();
	glPopMatrix();
}
void createBancas(GLuint text, GLuint text2, GLuint text3) {
	// textura de prisma, textura de flat, textura de cilindro
	glPushMatrix();
		glTranslatef(-82.0, -0.5, 0.0);
		// Sector A1
		glTranslatef(24.0 + 10.0, 0.0, -12.0);
		createBanca(text, text2, text3);
		glPushMatrix();
			// Sector B1
			glTranslatef(0.0, 0.0, 24.0);
			createBanca(text, text2, text3);
		glPopMatrix();
		//Sector A2
		glTranslatef(19.0, 0.0, 0.0);
		createBanca(text, text2, text3);
		glPushMatrix();
			//Sector B2
			glTranslatef(0.0, 0.0, 24.0);
			createBanca(text, text2, text3);
		glPopMatrix();
		//Sector C1
		glTranslatef(42.0, 0.0, 0.0);
		createBanca(text, text2, text3);
		glPushMatrix();
			//Sector D1
			glTranslatef(0.0, 0.0, 24.0);
			createBanca(text, text2, text3);
		glPopMatrix();
		//Sector C2
		glTranslatef(18.0, 0.0, 0.0);
		createBanca(text, text2, text3);
		glPushMatrix();
			//Sector D2
			glTranslatef(0.0, 0.0, 24.0);
			createBanca(text, text2, text3);
		glPopMatrix();
		//Sector C3
		glTranslatef(18.0, 0.0, 0.0);
		createBanca(text, text2, text3);
		glPushMatrix();
			//Sector D3
			glTranslatef(0.0, 0.0, 24.0);
			createBanca(text, text2, text3);
		glPopMatrix();
	glPopMatrix();
}
/*********** KIOSKO	 ***********/
/*********** FUENTE	 ***********/
/*********** FACHADA ***********/
/*********** AVATAR1 ***********/
/*********** AVATAR2 ***********/
/*********** LUCES	 ***********/
void createFoco1() {
	glPushMatrix();
	float s = 0.001;
	//glTranslatef(-1.2 + trax, 15.9 + tray, 0.0 + traz);
	glTranslatef(-1.2, 15.9, 0.0);
	glScalef(s, s, s);
	foco.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();
}
void createFoco2() {
	glPushMatrix();
	//float s = 0.01;
	glTranslatef(-5.7, 6.4, 0.6);
	//glTranslatef(0.0 + trax, 0.0 + tray, -1.0 + traz);
	//glScalef(sc, sc, sc);
	//glRotatef(sc, 0.0, 1.0, 0.0);
	glRotatef(-44.0, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.02, 0.02);
	lampara.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();
}
void createFoco3() {
	glPushMatrix();
	float s = 0.04;
	glTranslatef(-0.3, 0.0, 17.2);
	//glTranslatef(0.0 + trax, 0.0 + tray, -1.0 + traz);
	glScalef(s, s, s);
	lamparaCalle.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();
}
void foc1() {
	if (foco1)
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);			// Position The Light
	//	IMPORTANTE		180.0 es el valor en el que se convierte en una luz puntual
	glPushMatrix();
	glTranslatef(-0.4, 13.6, 17.2);		//En el techo mismo
	//glTranslatef(-0.4 + trax, 13.6 + tray, 17.2 + traz);
	//glutSolidSphere(0.1, 10, 10);	//Representa el foco, solo para prueba

	if (foco1) {
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_POSITION, LightPosition3);
	}
	else {
		glDisable(GL_LIGHT1);
	}
	glPopMatrix();
}
void foc2() {
	//	LUZ SPOTLIGHT
	if (foco2)
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, LightAngle);			// Position The Light
	glPushMatrix();
	//glTranslatef(0.0 + trax, 0.0 + tray, 0.0 + traz);
	glTranslatef(-5.0, 9.1, -0.1);
	//glutSolidSphere(0.1, 10, 10);
	//glRotatef(angleX2, 1.0f, 0.0f, 0.0f);
	//glRotatef(angleY2, 0.0f, 1.0f, 0.0f);
	glRotatef(-66.0, 1.0f, 0.0f, 0.0f);
	glRotatef(-16.0, 0.0f, 1.0f, 0.0f);


	if (foco2) {
		glEnable(GL_LIGHT2);
		glLightfv(GL_LIGHT2, GL_POSITION, LightPosition2);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, LightDirection2);

	}
	else {
		glDisable(GL_LIGHT2);
	}
	glPopMatrix();
}
void foc3() {
	if (foco3)
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 180.0);			// Position The Light
	//	IMPORTANTE		180.0 es el valor en el que se convierte en una luz puntual
	glPushMatrix();
	glTranslatef(-1.0, 17.4, 0.0);		//En el techo mismo
	//glTranslatef(-1.0 + trax, 17.4 + tray, 0.0 + traz);
	//glutSolidSphere(0.1, 10, 10);	//Representa el foco, solo para prueba

	if (foco3) {
		glEnable(GL_LIGHT3);
		glLightfv(GL_LIGHT3, GL_POSITION, LightPosition3);
	}
	else {
		glDisable(GL_LIGHT3);
	}
	glPopMatrix();
}
void luz() {
	glPushMatrix();
		glPushMatrix();
			if (!light) {
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT1);
				glDisable(GL_LIGHT2);
				glDisable(GL_LIGHT3);
			}
			else {
				glEnable(GL_LIGHTING);
			}
		glPopMatrix();
		foc1();
		foc2();
		foc3();
	glPopMatrix();
}

void display(void)   // Creamos la funcion donde se dibuja
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glPushMatrix();	//General

	glRotatef(g_lookupdown, 1.0f, 0, 0);
	gluLookAt(objCamera.mPos.x, objCamera.mPos.y, objCamera.mPos.z,
		objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,
		objCamera.mUp.x, objCamera.mUp.y, objCamera.mUp.z);

	luz();
	glPushMatrix();
		glPushMatrix(); //Creamos cielo
			glTranslatef(0, 60, 0);
			fig1.skybox(130.0, 130.0, 130.0, cielo.GLindex);
		glPopMatrix();
	glPopMatrix();


	glPushMatrix(); //Piso primario
		glTranslatef(0.0, 0.0, 0.0);
		glScalef(0.5, 0.5, 0.5);
		createPlancha();
		glTranslatef(0.0, 0.6, 0.0);
		createFences();
		glPushMatrix();
			//glTranslatef(0.0, 2.0, 0.0);
			//brick.slope1_2(t_madera.GLindex, t_madera2.GLindex);
			//brick.slopeV(10.0, 1.0, t_madera.GLindex, t_madera2.GLindex);
			createBancas(t_carbon.GLindex, t_madera.GLindex, t_maderaO.GLindex);
		glPopMatrix();
	glPopMatrix();


	glPopMatrix();	//General

	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_LIGHTING);

	// Pintar texto en pantalla
	glColor3f(1.0, 0.0, 0.0);
	pintaTexto(-12, 12.0, -14.0, (void*)font, "Proyecto Final");
	pintaTexto(-12, 10.5, -14, (void*)font, "Alameda Lego");
	glColor3f(1.0, 1.0, 1.0);
	//glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glutSwapBuffers();

}

void animacion()
{
	fig3.text_izq -= 0.001;
	fig3.text_der -= 0.001;
	if (fig3.text_izq < -1)
		fig3.text_izq = 0;
	if (fig3.text_der < 0)
		fig3.text_der = 1;
	// Calculate the number of frames per one second:
	//dwFrames++;
	dwCurrentTime = GetTickCount(); // Even better to use timeGetTime()
	dwElapsedTime = dwCurrentTime - dwLastUpdateTime;

	if (dwElapsedTime >= 150)
	{
		//Animacion Puerta
		/*if (rotPuerta == 0)
			rotPuerta += 1;
		else if (rotPuerta == 90)
			rotPuerta -= 1;*/
		dwLastUpdateTime = dwCurrentTime;
	}
	if (dwElapsedTime >= 30)
	{
		if (banderaPuerta == true) {
			if (banderaUpDown == true) {	//Va de 0 a 90
				rotPuerta += 2.0;
				if (rotPuerta == 90.0)
					banderaPuerta = false;
			}
			else {
				rotPuerta -= 2.0;
				if (rotPuerta == 0.0)
					banderaPuerta = false;
			}
		}
		if (banderaVentana == true) {
			if (banderaUpDown == true) {	//Va de 0 a 90
				rotVentana += 2.0;
				if (rotVentana == 66.0)
					banderaVentana = false;
			}
			else {
				rotVentana -= 2.0;
				if (rotVentana == 0.0)
					banderaVentana = false;
			}
		}
		/*
			Basicamente, si banderaSilla es true, se presionó la tecla 5. Como Z = 0.0:
			Primero se mueve en -z hasta -2.0, luego rota hasta -90.0 y pone banderaSilla en false

			Si banderaSilla vuelve a ser true, ahora como Z = -2.0:
			Primero rota hasta 0.0, luego se mueve en Z hasta 0.0
		*/
		if (banderaSilla == true) {
			if (banderaTrans == true) {
				transSilla -= 0.1;
				if (transSilla <= -2.0) {
					transSilla = -2.0f;
					banderaUpDown = true;
				}
				if (banderaUpDown == true) {	//Va de 0 a 90
					rotSilla -= 2.0;
					if (rotSilla <= -90.0) {
						rotSilla = -90.0;
						banderaSilla = false;
					}

				}
			}
			else {

				rotSilla += 2.0;
				if (rotSilla >= 0.0) {
					rotSilla = 2.0;
					banderaUpDown = false;
				}
				if (banderaUpDown == false) {
					transSilla += 0.1;
					if (transSilla >= 0.0) {
						transSilla = 0.0f;
						banderaSilla = false;
					}
				}
			}
		}

		ranX = (float)(rand() % 100) / 100.0;
		ranY = (float)(rand() % 100) / 100.0;
		ranZ = (float)(rand() % 100) / 100.0;
		rotDado = (float)(rand() % 180 + 45);
		dwLastUpdateTime = dwCurrentTime;
	}
	if (play)
	{

		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();

			}
		}
		else
		{
			//Draw animation
			pBz += KeyFrame[playIndex].pBzInc;
			pCx += KeyFrame[playIndex].pCxInc;
			pCz += KeyFrame[playIndex].pCzInc;
			pDx += KeyFrame[playIndex].pDxInc;
			pDz += KeyFrame[playIndex].pDzInc;
			pFx += KeyFrame[playIndex].pFxInc;
			pFy += KeyFrame[playIndex].pFyInc;
			pFz += KeyFrame[playIndex].pFzInc;
			pGz += KeyFrame[playIndex].pGzInc;

			i_curr_steps++;
		}

	}


	glutPostRedisplay();
}

void reshape(int width, int height)   // Creamos funcion Reshape
{
	if (height == 0)										// Prevenir division entre cero
	{
		height = 1;
	}

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);						// Seleccionamos Projection Matrix
	glLoadIdentity();

	// Tipo de Vista

	//glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 170.0);
	glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 200.0);

	glMatrixMode(GL_MODELVIEW);							// Seleccionamos Modelview Matrix
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)  // Create Keyboard Function
{
	switch (key) {
	case 'w':   //Movimientos de camara
	case 'W':
		objCamera.Move_Camera(CAMERASPEED + 0.2);
		break;

	case 's':
	case 'S':
		objCamera.Move_Camera(-(CAMERASPEED + 0.2));
		break;

	case 'a':
	case 'A':
		objCamera.Strafe_Camera(-(CAMERASPEED + 0.4));
		break;

	case 'd':
	case 'D':
		objCamera.Strafe_Camera(CAMERASPEED + 0.4);
		break;

		/*case ' ':		//Poner algo en movimiento
			//Commit?
			printf("mPos.x = %f\tmPos.y = %f\tmPos.z = %f\n",objCamera.mPos.x, objCamera.mPos.y, objCamera.mPos.z);
			printf("mView.x = %f\tmView.y = %f\tmView.z = %f\n", objCamera.mView.x, objCamera.mView.y, objCamera.mView.z);
			printf("mUp.x = %f\tmUp.y = %f\tmUp.z = %f\n", objCamera.mUp.x, objCamera.mUp.y, objCamera.mUp.z);
			printf("glookupdown = %f\n", g_lookupdown);
			printf("CAMERASPEED: %f\n", CAMERASPEED);
			printf("******************************************\n");

			break;*/
	case '0':	//Original
		banderaCO = true;	//Estamos en camara original
		//Posiciona la camara a la posición original. Si viene de cualquier otra cámara, no debe borrar los anteriores estados.
		if (banderaCC == true) {
			//Viene de Camara Juego
			banderaCC = false;//quitamos bandera CJ
			//Guardo valores de la camara anterior (Cuarto)
			pos_xC = objCamera.mPos.x;
			pos_yC = objCamera.mPos.y;
			pos_zC = objCamera.mPos.z;
			view_xC = objCamera.mView.x;
			view_yC = objCamera.mView.y;
			view_zC = objCamera.mView.z;
			up_xC = objCamera.mUp.x;
			up_yC = objCamera.mUp.y;
			up_zC = objCamera.mUp.z;
			lookUpDownC = g_lookupdown;
		}
		if (banderaCJ == true) {
			//Viene de Camara Juego
			banderaCJ = false;//quitamos bandera CJ
			//Guardo valores de la camara anterior (Juego)
			pos_xJ = objCamera.mPos.x;
			pos_yJ = objCamera.mPos.y;
			pos_zJ = objCamera.mPos.z;
			view_xJ = objCamera.mView.x;
			view_yJ = objCamera.mView.y;
			view_zJ = objCamera.mView.z;
			up_xJ = objCamera.mUp.x;
			up_yJ = objCamera.mUp.y;
			up_zJ = objCamera.mUp.z;
			lookUpDownJ = g_lookupdown;
		}
		g_lookupdown = 16.0;
		//objCamera.Position_Camera(0.13f, 3.2f, 8.95f, 0.13f, 3.2f, 5.95f, 0, 1, 0);
		objCamera.Position_Camera(4.36f, 15.8f, 30.82f, 4.15f, 15.8f, 27.82f, 0, 1, 0);
		break;
	case '1':	//Cuarto
		banderaCC = true;	//Estamos en camara cuarto.
		if (banderaCO == true) {
			//Si viene de cámara original, no debe cambiar nada de los estados anteriores de la cámara para Cuarto y Juego
			pos_xC = pos_xC;
			pos_yC = pos_yC;
			pos_zC = pos_zC;
			view_xC = view_xC;
			view_yC = view_yC;
			view_zC = view_zC;
			up_xC = up_xC;
			up_yC = up_yC;
			up_zC = up_zC;
			lookUpDownC = lookUpDownC;

			banderaCO = false;
			g_lookupdown = lookUpDownC;
			objCamera.Position_Camera(pos_xC, pos_yC, pos_zC, view_xC, view_yC, view_zC, up_xC, up_yC, up_zC);
		}
		else {
			//Reviso el estado de las otras cámaras. De por si, banderaCO ya se sabe es false.
			//Primero, reviso si CJ estaba previamente true, de modo que se sabe que viene de esa cámara.
			if (banderaCJ == true) {
				//Viene de Camara Juego
				banderaCJ = false;//quitamos bandera CJ
				//Guardo valores de la camara anterior (Juego)
				pos_xJ = objCamera.mPos.x;
				pos_yJ = objCamera.mPos.y;
				pos_zJ = objCamera.mPos.z;
				view_xJ = objCamera.mView.x;
				view_yJ = objCamera.mView.y;
				view_zJ = objCamera.mView.z;
				up_xJ = objCamera.mUp.x;
				up_yJ = objCamera.mUp.y;
				up_zJ = objCamera.mUp.z;
				lookUpDownJ = g_lookupdown;
			}
			//Si es false, quiere decir que es o la primera vez que entra o viene de CO sin haberse metido a CJ
			g_lookupdown = lookUpDownC;
			objCamera.Position_Camera(pos_xC, pos_yC, pos_zC, view_xC, view_yC, view_zC, up_xC, up_yC, up_zC);
		}

		break;
	case '2':	//Juego
		banderaCJ = true;
		if (banderaCO == true) {
			//Si viene de cámara original, no debe cambiar nada de los estados anteriores de la cámara para Cuarto y Juego
			pos_xJ = pos_xJ;
			pos_yJ = pos_yJ;
			pos_zJ = pos_zJ;
			view_xJ = view_xJ;
			view_yJ = view_yJ;
			view_zJ = view_zJ;
			up_xJ = up_xJ;
			up_yJ = up_yJ;
			up_zJ = up_zJ;
			lookUpDownJ = lookUpDownJ;
			banderaCO = false;
			g_lookupdown = lookUpDownJ;
			objCamera.Position_Camera(pos_xJ, pos_yJ, pos_zJ, view_xJ, view_yJ, view_zJ, up_xJ, up_yJ, up_zJ);
		}
		else {
			//Reviso el estado de las otras cámaras. De por si, banderaCO ya se sabe es false.
			//Primero, reviso si CC estaba previamente true, de modo que se sabe que viene de esa cámara.
			if (banderaCC == true) {
				//Viene de Camara Juego
				banderaCC = false;//quitamos bandera CJ
				//Guardo valores de la camara anterior (Cuarto)
				pos_xC = objCamera.mPos.x;
				pos_yC = objCamera.mPos.y;
				pos_zC = objCamera.mPos.z;
				view_xC = objCamera.mView.x;
				view_yC = objCamera.mView.y;
				view_zC = objCamera.mView.z;
				up_xC = objCamera.mUp.x;
				up_yC = objCamera.mUp.y;
				up_zC = objCamera.mUp.z;
				lookUpDownC = g_lookupdown;
			}
			//Si es false, quiere decir que es o la primera vez que entra o viene de CO sin haberse metido a CC
			g_lookupdown = lookUpDownJ;
			objCamera.Position_Camera(pos_xJ, pos_yJ, pos_zJ, view_xJ, view_yJ, view_zJ, up_xJ, up_yJ, up_zJ);
		}
		break;
	case '3':	//
		if (banderaPuerta == false) {
			banderaPuerta = true;
			if (rotPuerta == 0)
				banderaUpDown = true;	//Va de 0 a 90
			else
				banderaUpDown = false;	//Va de 90 a 0;
		}
		break;
	case '4':	//
		if (banderaVentana == false) {
			banderaVentana = true;
			if (rotVentana == 0)
				banderaUpDown = true;	//Va de 0 a 90
			else
				banderaUpDown = false;
		}
		break;
	case '5':
		if (banderaSilla == false) {
			banderaSilla = true;
			if (transSilla == 0.0)	//Va de 0 a -2.0
				banderaTrans = true;	//Va de 0 a 90
			else
				banderaTrans = false;
		}
		break;
		/*case ' ':	//Salvar Frame
			if (FrameIndex < MAX_FRAMES)
			{
				saveFrame();
			}

			break;*/
	case '6':
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}
		break;

		/*case '.':
			for (int i = 0; i < FrameIndex; i++) {
				printf("Frame [%d]\n", i);
				printf("pBz = %f  pCx = %f  pCz = %f  pDx = %f  pDz = %f\n", KeyFrame[i].pBz, KeyFrame[i].pCx, KeyFrame[i].pCz, KeyFrame[i].pDx, KeyFrame[i].pDz);
				printf("pFx = %f  pFy = %f  pFz = %f  pGz = %f\n", KeyFrame[i].pFz, KeyFrame[i].pFy, KeyFrame[i].pFz, KeyFrame[i].pGz);

				printf("\nIncrementos:\n\n");
				printf("pBzInc = %f  pCxInc = %f  pCzInc = %f  pDxInc = %f  pDzInc = %f\n", KeyFrame[i].pBzInc, KeyFrame[i].pCxInc, KeyFrame[i].pCzInc, KeyFrame[i].pDxInc, KeyFrame[i].pDzInc);
				printf("pFxInc = %f  pFyInc = %f  pFzInc = %f  pGzInc = %f\n", KeyFrame[i].pFzInc, KeyFrame[i].pFyInc, KeyFrame[i].pFzInc, KeyFrame[i].pGzInc);
				printf("******************************************\n\n");
			}
			break;*/

	case 'l':   //Activamos/desactivamos luz
	case 'L':
		light = !light;
		if (light == true) {
			foco1 = true;
			foco2 = true;
			foco3 = true;
		}
		else {
			foco1 = false;
			foco2 = false;
			foco3 = false;
		}
		break;
	case '7':   //Activamos/desactivamos luz
	//case 'P':
		//positional = !positional;
		if (light == true)
			foco1 = !foco1;
		break;
	case '8':
		if (light == true)
			foco2 = !foco2;
		break;
	case '9':
		if (light == true)
			foco3 = !foco3;
		break;

	case 'c':
		LightAngle += 2.0;
		printf("Ang = %f\n", LightAngle);
		break;
	case 'v':
		LightAngle -= 2.0;
		printf("Ang = %f\n", LightAngle);
		break;


	case 27:        // Cuando Esc es presionado...
		exit(0);   // Salimos del programa
		break;
	default:        // Cualquier otra
		break;
	}

	glutPostRedisplay();
}

void arrow_keys(int a_keys, int x, int y)  // Funcion para manejo de teclas especiales (arrow keys)
{
	switch (a_keys) {
	case GLUT_KEY_PAGE_UP:
		objCamera.UpDown_Camera(CAMERASPEED);
		break;

	case GLUT_KEY_PAGE_DOWN:
		objCamera.UpDown_Camera(-CAMERASPEED);
		break;

	case GLUT_KEY_UP:     // Presionamos tecla ARRIBA...
		g_lookupdown -= 1.0f;
		break;

	case GLUT_KEY_DOWN:               // Presionamos tecla ABAJO...
		g_lookupdown += 1.0f;
		break;

	case GLUT_KEY_LEFT:
		objCamera.Rotate_View(-CAMERASPEED);
		break;

	case GLUT_KEY_RIGHT:
		objCamera.Rotate_View(CAMERASPEED);
		break;

	default:
		break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv)   // Main Function
{
	glutInit(&argc, argv); // Inicializamos OpenGL
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode (Clores RGB y alpha | Buffer Doble )
	glutInitWindowSize(1600, 900);	// Tamaño de la Ventana
	glutInitWindowPosition(0, 0);	//Posicion de la Ventana
	glutCreateWindow("Proyecto Final"); // Nombre de la Ventana
	//glutFullScreen     ( );         // Full Screen
	InitGL();						// Parametros iniciales de la aplicacion
	glutDisplayFunc(display);  //Indicamos a Glut función de dibujo
	glutReshapeFunc(reshape);	//Indicamos a Glut función en caso de cambio de tamano
	glutKeyboardFunc(keyboard);	//Indicamos a Glut función de manejo de teclado
	glutSpecialFunc(arrow_keys);	//Otras
	glutIdleFunc(animacion);
	glutMainLoop();          // 

	return 0;
}
