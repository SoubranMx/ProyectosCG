//Semestre 2020 - 2
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
GLfloat aten1[] = { 0.35 , 0.0, 0.0 };	//Atenuacion de Luz Puntual
GLfloat aten2[] = { 0.1 , 0.0, 0.0 };	//Atenuacion de Luz Spotlight
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

float LightAngle = 18.0f;

bool light = false;		// Luz ON/OFF
bool foco1 = false;
bool foco2 = false;
bool foco3 = false;
bool sol0 = false;
bool dia = false;

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

materialStruct yellow_plastic{
	{0.0, 0.0, 0.0, 1.0},	//Ka
	{0.5, 0.5, 0.0, 1.0},	//Kd
	{0.6, 0.6, 0.6, 1.0},	//Ks
	{32.0}						//n
};

materialStruct morado{
	{0.11, 0.06, 0.17, 1.0},	//Ka
	{0.83, 0.43, 0.8, 1.0},	//Kd
	{0.61, 0.55, 0.36, 1.0},	//Ks
	{51.2}						//n
};

materialStruct rosa_palido{
	{0.11, 0.06, 0.17, 1.0},	//Ka
	{1.0, 0.43, 0.59, 1.0},	//Kd
	{0.61, 0.55, 0.36, 1.0},	//Ks
	{51.2}						//n
};

materialStruct azul{
	{0.11, 0.1, 0.2, 1.0},	//Ka
	{0.3, 0.47, 0.95, 1.0},	//Kd
	{0.61, 0.55, 0.36, 1.0},	//Ks
	{51.2}						//n
};

materialStruct turquesa{
	{0.1, 0.187, 0.174, 1.0},	//Ka
	{0.396, 0.7415, 0.691, 1.0},	//Kd
	{0.297254, 0.30829, 0.306678, 1.0},	//Ks
	{12.8}						//n
};

materialStruct madera{
	{0.0, 0.08, 0.07, 1.0},	//Ka
	{0.83, 0.4, 0.06, 1.0},	//Kd
	{0.3327, 0.3286, 0.3464, 1.0},	//Ks
	{36.0}						//n
};

materialStruct obsidiana{
	{0.05375, 0.05, 0.06625, 1.0},	//Ka
	{0.18275, 0.17, 0.22525, 1.0},	//Kd
	{0.3327, 0.3286, 0.3464, 1.0},	//Ks
	{38.4}						//n
};

materialStruct emerald{
	{0.0215, 0.1745, 0.0215, 0.55},	//Ka
	{0.07568, 0.61424, 0.07568, 0.55},	//Kd
	{0.633, 0.727811, 0.633, 0.55},	//Ks
	{76.8}						//n
};

materialStruct jade{
	{0.135, 0.2225, 0.1575, 0.95},	//Ka
	{0.54f, 0.89f, 0.63f, 0.95f },	//Kd
	{0.316228f, 0.316228f, 0.316228f, 0.95f },	//Ks
	{12.8}						//n
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
CTexture t_amarillo;
CTexture t_amarillo2;
CTexture t_morado;
CTexture t_morado2;
CTexture t_rosa;
CTexture t_rosa2;
CTexture t_azul;
CTexture t_azul2;
CTexture t_turquesa;
CTexture t_turquesa2;
CTexture t_blanco;
CTexture t_banoMujer;
CTexture t_banoHombre;

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
//CFiguras pisoGeneral;
//CFiguras silla;
//CFiguras mesa;
//CFiguras cuarto;
//CFiguras puerta;

//Figuras de 3D Studio
CModel wc;
CModel lavaManos;
CModel taza;
CModel sofa;
CModel muebleTV;
CModel tv;
CModel librero;
CModel foco;
CModel lampara;
CModel lamparaCalle;
/*
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
*/

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
bool banderaModelos = false;
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
float rotSol = 0.0f;
float rotDado = 0.0f;


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
	Como la animación por KeyFrames solo preveo 6 o 7 movimientos, solo utilizaré 6 variables.

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
float rotLampX = 0.0, rotLampY = 0.0, rotLampZ = 0.0;
float traxFrame = 0.0, trayFrame = 0.0, trazFrame = 0.0;
#define MAX_FRAMES 15		//15 keyframes
int i_max_steps = 90;		//Cantidad de cuadros intermedios	valores peque�os: animacion r�pida, valores grandes: animaci�n m�s pausada
int i_curr_steps = 0;

typedef struct _frame {
	//Variables para guardar KeyFrames
	float traxFrame;
	float trayFrame;
	float trazFrame;
	float rotLampX;
	float rotLampY;
	float rotLampZ;

	//Incrementos
	float traxFrameInc;
	float trayFrameInc;
	float trazFrameInc;
	float rotLampXInc;
	float rotLampYInc;
	float rotLampZInc;
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
	KeyFrame[FrameIndex].traxFrame = traxFrame;
	KeyFrame[FrameIndex].trayFrame = trayFrame;
	KeyFrame[FrameIndex].trazFrame = trazFrame;
	KeyFrame[FrameIndex].rotLampX = rotLampX;
	KeyFrame[FrameIndex].rotLampY = rotLampY;
	KeyFrame[FrameIndex].rotLampZ = rotLampZ;
	FrameIndex++;
}
void resetElements(void)
{
	traxFrame = KeyFrame[0].traxFrame;
	trayFrame = KeyFrame[0].trayFrame;
	trazFrame = KeyFrame[0].trazFrame;
	rotLampX = KeyFrame[0].rotLampX;
	rotLampY = KeyFrame[0].rotLampY;
	rotLampZ = KeyFrame[0].rotLampZ;
}
void interpolation(void)
{
	//Los incrementos
	KeyFrame[playIndex].traxFrameInc = (KeyFrame[playIndex + 1].traxFrame - KeyFrame[playIndex].traxFrame) / i_max_steps;
	KeyFrame[playIndex].trayFrameInc = (KeyFrame[playIndex + 1].trayFrame - KeyFrame[playIndex].trayFrame) / i_max_steps;
	KeyFrame[playIndex].trazFrameInc = (KeyFrame[playIndex + 1].trazFrame - KeyFrame[playIndex].trazFrame) / i_max_steps;
	KeyFrame[playIndex].rotLampXInc = (KeyFrame[playIndex + 1].rotLampX - KeyFrame[playIndex].rotLampX) / i_max_steps;
	KeyFrame[playIndex].rotLampYInc = (KeyFrame[playIndex + 1].rotLampY - KeyFrame[playIndex].rotLampY) / i_max_steps;
	KeyFrame[playIndex].rotLampZInc = (KeyFrame[playIndex + 1].rotLampZ - KeyFrame[playIndex].rotLampZ) / i_max_steps;
}

//	*************	KEY FRAMES	*********************

void InitGL(GLvoid)     // Inicializamos parametros
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Negro de fondo	

	glEnable(GL_TEXTURE_2D);
	//glShadeModel(GL_SMOOTH);
	glShadeModel(GL_FLAT);
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
	//SOL
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, LightSpecular);
	//Puntual		Foco de lampara de exterior
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);					// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);					// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, aten1);

	//Puntual		Foco de lampara de exterior
	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);					// Setup The Ambient Light
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse);					// Setup The Diffuse Light
	glLightfv(GL_LIGHT2, GL_SPECULAR, LightSpecular);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT2, GL_LINEAR_ATTENUATION, aten1);

	//Puntual		Foco de lampara de exterior
	glLightfv(GL_LIGHT3, GL_AMBIENT, LightAmbient);					// Setup The Ambient Light
	glLightfv(GL_LIGHT3, GL_DIFFUSE, LightDiffuse);					// Setup The Diffuse Light
	glLightfv(GL_LIGHT3, GL_SPECULAR, LightSpecular);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT3, GL_LINEAR_ATTENUATION, aten1);

	//Puntual		Foco de lampara de exterior
	glLightfv(GL_LIGHT4, GL_AMBIENT, LightAmbient);					// Setup The Ambient Light
	glLightfv(GL_LIGHT4, GL_DIFFUSE, LightDiffuse);					// Setup The Diffuse Light
	glLightfv(GL_LIGHT4, GL_SPECULAR, LightSpecular);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT4, GL_LINEAR_ATTENUATION, aten1);

	//Puntual		Foco de lampara de exterior
	glLightfv(GL_LIGHT5, GL_AMBIENT, LightAmbient);					// Setup The Ambient Light
	glLightfv(GL_LIGHT5, GL_DIFFUSE, LightDiffuse);					// Setup The Diffuse Light
	glLightfv(GL_LIGHT5, GL_SPECULAR, LightSpecular);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT5, GL_LINEAR_ATTENUATION, aten1);
	

	// Spotligh Kiosko

	//Spotlight		Foco de lámpara
	glLightfv(GL_LIGHT6, GL_AMBIENT, LightAmbient);					// Setup The Ambient Light
	glLightfv(GL_LIGHT6, GL_DIFFUSE, LightDiffuse);					// Setup The Diffuse Light
	glLightfv(GL_LIGHT6, GL_SPECULAR, LightSpecular);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT6, GL_LINEAR_ATTENUATION, aten2);

	//Spotlight		Foco de lámpara
	glLightfv(GL_LIGHT7, GL_AMBIENT, LightAmbient);					// Setup The Ambient Light
	glLightfv(GL_LIGHT7, GL_DIFFUSE, LightDiffuse);					// Setup The Diffuse Light
	glLightfv(GL_LIGHT7, GL_SPECULAR, LightSpecular);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT7, GL_LINEAR_ATTENUATION, aten2);

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

	t_amarillo.LoadTGA("Resources/Texturas/amarillo.tga");
	t_amarillo.BuildGLTexture();
	t_amarillo.ReleaseImage();
	t_amarillo2.LoadTGA("Resources/Texturas/amarillo2.tga");
	t_amarillo2.BuildGLTexture();
	t_amarillo2.ReleaseImage();

	t_morado.LoadTGA("Resources/Texturas/morado.tga");
	t_morado.BuildGLTexture();
	t_morado.ReleaseImage();
	t_morado2.LoadTGA("Resources/Texturas/morado2.tga");
	t_morado2.BuildGLTexture();
	t_morado2.ReleaseImage();

	t_rosa.LoadTGA("Resources/Texturas/rosa.tga");
	t_rosa.BuildGLTexture();
	t_rosa.ReleaseImage();
	t_rosa2.LoadTGA("Resources/Texturas/rosa2.tga");
	t_rosa2.BuildGLTexture();
	t_rosa2.ReleaseImage();

	t_azul.LoadTGA("Resources/Texturas/azul.tga");
	t_azul.BuildGLTexture();
	t_azul.ReleaseImage();
	t_azul2.LoadTGA("Resources/Texturas/azul2.tga");
	t_azul2.BuildGLTexture();
	t_azul2.ReleaseImage();

	t_turquesa.LoadTGA("Resources/Texturas/turquesa.tga");
	t_turquesa.BuildGLTexture();
	t_turquesa.ReleaseImage();
	t_turquesa2.LoadTGA("Resources/Texturas/turquesa2.tga");
	t_turquesa2.BuildGLTexture();
	t_turquesa2.ReleaseImage();

	t_blanco.LoadTGA("Resources/Texturas/blanco.tga");
	t_blanco.BuildGLTexture();
	t_blanco.ReleaseImage();

	t_banoHombre.LoadTGA("Resources/Texturas/banoHombre.tga");
	t_banoHombre.BuildGLTexture();
	t_banoHombre.ReleaseImage();
	t_banoMujer.LoadTGA("Resources/Texturas/banoMujer.tga");
	t_banoMujer.BuildGLTexture();
	t_banoMujer.ReleaseImage();


	/*
		********** FIGURAS **********

		Carga de figuras 3DS.
		Casi siempre debe ir escrito como .3DS ,en mayusculas.
		Algunas veces crashea en la ejecución, asi que se abre en 3D MAX y se vuelve a exportar como 3DS, esperando que funcione
	*/
	taza._3dsLoad("Resources/Modelos/Toilet_3DS.3DS");
	taza.VertexNormals();
	
	wc._3dsLoad("Resources/Modelos/hsdc00.3DS");
	wc.VertexNormals();

	lavaManos._3dsLoad("Resources/Modelos/washbasin-01.3DS");
	lavaManos.VertexNormals();

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
		KeyFrame[i].traxFrame = 0;
		KeyFrame[i].trayFrame = 0;
		KeyFrame[i].trazFrame = 0;
		KeyFrame[i].rotLampX = 0;
		KeyFrame[i].rotLampY = 0;
		KeyFrame[i].rotLampZ = 0;

		KeyFrame[i].traxFrameInc = 0;
		KeyFrame[i].trayFrameInc = 0;
		KeyFrame[i].trazFrameInc = 0;
		KeyFrame[i].rotLampXInc = 0;
		KeyFrame[i].rotLampYInc = 0;
		KeyFrame[i].rotLampZInc = 0;
	}
	// FRAME 1
	KeyFrame[1].traxFrame = 0.0;
	KeyFrame[1].trayFrame = 0.0;
	KeyFrame[1].trazFrame = -3.0;
	KeyFrame[1].rotLampX = 40.0;
	KeyFrame[1].rotLampY = 0.0;
	KeyFrame[1].rotLampZ = -15.0;//-10.0

	KeyFrame[1].traxFrameInc = 0.0;
	KeyFrame[1].trayFrameInc = 0.0;
	KeyFrame[1].trazFrameInc = 0.0;
	KeyFrame[1].rotLampXInc = 0.0;
	KeyFrame[1].rotLampYInc = 0.0;
	KeyFrame[1].rotLampZInc = 0.0;

	// FRAME 2
	KeyFrame[2].traxFrame = 0.0;
	KeyFrame[2].trayFrame = 0.0;
	KeyFrame[2].trazFrame = -3.0;
	KeyFrame[2].rotLampX = 30.0;
	KeyFrame[2].rotLampY = 40.0;
	KeyFrame[2].rotLampZ = 60.0;//-10.0

	KeyFrame[2].traxFrameInc = 0.0;
	KeyFrame[2].trayFrameInc = 0.0;
	KeyFrame[2].trazFrameInc = 0.0;
	KeyFrame[2].rotLampXInc = 0.0;
	KeyFrame[2].rotLampYInc = 0.0;
	KeyFrame[2].rotLampZInc = 0.0;

	// FRAME 3
	KeyFrame[3].traxFrame = 0.0;
	KeyFrame[3].trayFrame = 0.0;
	KeyFrame[3].trazFrame = 1.0;
	KeyFrame[3].rotLampX = 0.0;
	KeyFrame[3].rotLampY = 0.0;
	KeyFrame[3].rotLampZ = 60.0;

	KeyFrame[3].traxFrameInc = 0.0;
	KeyFrame[3].trayFrameInc = 0.0;
	KeyFrame[3].trazFrameInc = 0.0;
	KeyFrame[3].rotLampXInc = 0.0;
	KeyFrame[3].rotLampYInc = 0.0;
	KeyFrame[3].rotLampZInc = 0.0;

	// FRAME 4
	KeyFrame[4].traxFrame = 0.0;
	KeyFrame[4].trayFrame = 0.0;
	KeyFrame[4].trazFrame = 4.0;
	KeyFrame[4].rotLampX = -30.0;
	KeyFrame[4].rotLampY = 0.0;
	KeyFrame[4].rotLampZ = 10.0;

	KeyFrame[4].traxFrameInc = 0.0;
	KeyFrame[4].trayFrameInc = 0.0;
	KeyFrame[4].trazFrameInc = 0.0;
	KeyFrame[4].rotLampXInc = 0.0;
	KeyFrame[4].rotLampYInc = 0.0;
	KeyFrame[4].rotLampZInc = 0.0;

	// FRAME 5
	KeyFrame[5].traxFrame = 0.0;
	KeyFrame[5].trayFrame = 0.0;
	KeyFrame[5].trazFrame = 4.0;
	KeyFrame[5].rotLampX = 10.0;
	KeyFrame[5].rotLampY = -60.0;
	KeyFrame[5].rotLampZ = 110.0;

	KeyFrame[5].traxFrameInc = 0.0;
	KeyFrame[5].trayFrameInc = 0.0;
	KeyFrame[5].trazFrameInc = 0.0;
	KeyFrame[5].rotLampXInc = 0.0;
	KeyFrame[5].rotLampYInc = 0.0;
	KeyFrame[5].rotLampZInc = 0.0;

	// FRAME 6
	KeyFrame[6].traxFrame = 0.0;
	KeyFrame[6].trayFrame = 0.0;
	KeyFrame[6].trazFrame = 0.0;
	KeyFrame[6].rotLampX = -0.000008;
	KeyFrame[6].rotLampY = -0.000042;
	KeyFrame[6].rotLampZ = 0.0;

	KeyFrame[6].traxFrameInc = 0.0;
	KeyFrame[6].trayFrameInc = 0.0;
	KeyFrame[6].trazFrameInc = 0.0;
	KeyFrame[6].rotLampXInc = 0.0;
	KeyFrame[6].rotLampYInc = 0.0;
	KeyFrame[6].rotLampZInc = 0.0;
	FrameIndex = 7;
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
/*********** LUCES	 ***********/
void foc1(int source) {

	//if (foco1)
//		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);			// Position The Light
	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.0);
		//glutSolidSphere(0.1, 10, 10);	//Representa la fuente de luz, solo para prueba

		if (foco1) {
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);			// Position The Light
			switch (source) {
				case 1:
					glEnable(GL_LIGHT1);
					glLightfv(GL_LIGHT1, GL_POSITION, LightPosition3);
					break;
				case 2:
					glEnable(GL_LIGHT2);
					glLightfv(GL_LIGHT2, GL_POSITION, LightPosition3);
					break;
				case 3:
					glEnable(GL_LIGHT3);
					glLightfv(GL_LIGHT3, GL_POSITION, LightPosition3);
					break;
				case 4:
					glEnable(GL_LIGHT4);
					glLightfv(GL_LIGHT4, GL_POSITION, LightPosition3);
					break;
				case 5:
					glEnable(GL_LIGHT5);
					glLightfv(GL_LIGHT5, GL_POSITION, LightPosition3);
					break;
			}	
		}
		else {
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHT3);
			glDisable(GL_LIGHT4);
			glDisable(GL_LIGHT5);
		}
	glPopMatrix();
}
void foc2() {
	//	LUZ SPOTLIGHT
	if (foco2)
		glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, LightAngle);			// Position The Light
	glPushMatrix();
	glRotatef(-90.0, 1.0f, 0.0f, 0.0f);


	if (foco2) {
		glEnable(GL_LIGHT6);
		glLightfv(GL_LIGHT6, GL_POSITION, LightPosition2);
		glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, LightDirection2);

	}
	else {
		glDisable(GL_LIGHT6);
	}
	glPopMatrix();
}
void foc3() {
	if (foco3)
		glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, LightAngle);			// Position The Light
	glPushMatrix();
	//glTranslatef(0.0 + trax, 0.0 + tray, 0.0 + traz);
	//glTranslatef(-5.0, 9.1, -0.1);
	glRotatef(-90.0, 1.0, 0.0, 0.0);


	if (foco3) {
		glEnable(GL_LIGHT7);
		glLightfv(GL_LIGHT7, GL_POSITION, LightPosition2);
		glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, LightDirection2);

	}
	else {
		glDisable(GL_LIGHT7);
	}
	glPopMatrix();
}
void sol() {
	/*if (sol0)
		glLightf(GL_LIGHT0, GL_POSITION, LightAngle);			// Position The Light*/
	glPushMatrix();
	glTranslatef(0.0 + trax, 0.0 + tray, 0.0 + traz);
	//glTranslatef(-5.0, 9.1, -0.1);
	glRotatef(rotSol, 1.0, 0.0, 1.0);


	if (sol0) {
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition2);

	}
	else {
		glDisable(GL_LIGHT0);
	}
	glPopMatrix();
}
void luz() {
	glPushMatrix();
		glPushMatrix();
			if (!light) {
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
				glDisable(GL_LIGHT1);
				glDisable(GL_LIGHT2);
				glDisable(GL_LIGHT3);
				glDisable(GL_LIGHT4);
				glDisable(GL_LIGHT5);
				glDisable(GL_LIGHT6);
				glDisable(GL_LIGHT7);
			}
			else {
				glEnable(GL_LIGHTING);
			}
			glPopMatrix();
			sol();
		//foc1();
		//foc2();
		//foc3();
	glPopMatrix();
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
		set_material(jade);
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
		set_material(madera);
		fenceA();
		fenceB();
		fenceC();
		fenceD();
	glPopMatrix();
};
/*********** BANCAS	 ***********/
void createBanca(GLuint text, GLuint text2, GLuint text3) {
	glPushMatrix();
		// Pata izquierda
		glPushMatrix();
			set_material(obsidiana);
			glTranslatef(-1.5, 0.5, 0.0);
			brick.prisma(1.0, 1.0, 1.0, text);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, 1.0, 1.0, text);
		glPopMatrix();
		// Pata Derecha
		glPushMatrix();
			set_material(obsidiana);
			glTranslatef(1.5, 0.5, 0.0);
			brick.prisma(1.0, 1.0, 1.0, text);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, 1.0, 1.0, text);
		glPopMatrix();
		glPushMatrix();
			set_material(brass);
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
/*********** MESAS	 ***********/
void createMesa(GLuint text, GLuint text2, GLuint text3, GLuint text4) {
	//text para el soporte, text2 para color mesa, text3 cilindros mesa, text4 cilindros sillas?
	glPushMatrix();
		glPushMatrix();
			set_material(obsidiana);
			brick.prisma(1.0, 1.0, 1.0, text);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, 1.0, 1.0, text);
			glTranslatef(0.0, 0.6, 0.0);
			set_material(brass);
			brick.flatV(6.0, 6.0, text2, text4);
		glPopMatrix();
		// Sillas
		glPushMatrix();
			set_material(obsidiana);
			glTranslatef(3.3 + 1.5, 0.0, 0.0);
			brick.prisma(1.0, 1.0, 3.0, text);
			glTranslatef(-0.5, 0.6, 0.0);
			set_material(ruby);
			brick.flatV(2.0, 3.0, text4, text2);
		glPopMatrix();
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glPushMatrix();
			set_material(obsidiana);
			glTranslatef(3.3 + 1.5, 0.0, 0.0);
			brick.prisma(1.0, 1.0, 3.0, text);
			glTranslatef(-0.5, 0.6, 0.0);
			set_material(ruby);
			brick.flatV(2.0, 3.0, text4, text2);
		glPopMatrix();
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glPushMatrix();
			set_material(obsidiana);
			glTranslatef(3.3 + 1.5, 0.0, 0.0);
			brick.prisma(1.0, 1.0, 3.0, text);
			glTranslatef(-0.5, 0.6, 0.0);
			set_material(ruby);
			brick.flatV(2.0, 3.0, text4, text2);
		glPopMatrix();
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glPushMatrix();
			set_material(obsidiana);
			glTranslatef(3.3 + 1.5, 0.0, 0.0);
			brick.prisma(1.0, 1.0, 3.0, text);
			glTranslatef(-0.5, 0.6, 0.0);
			set_material(ruby);
			brick.flatV(2.0, 3.0, text4, text2);
		glPopMatrix();
	glPopMatrix();
}
void createMesas() {
	glPushMatrix();
		glPushMatrix();
			glTranslatef(-8.0, 0.0, 25.0);
			createMesa(t_carbon.GLindex, t_naranja.GLindex, t_naranja2.GLindex, t_rojo.GLindex);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-8.0, 0.0, -25.0);
			createMesa(t_carbon.GLindex, t_naranja.GLindex, t_naranja2.GLindex, t_rojo.GLindex);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-68.0, 0.0, 33.0);
			createMesa(t_carbon.GLindex, t_naranja.GLindex, t_naranja2.GLindex, t_rojo.GLindex);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-68.0, 0.0, -33.0);
			createMesa(t_carbon.GLindex, t_naranja.GLindex, t_naranja2.GLindex, t_rojo.GLindex);
		glPopMatrix();
		//glTranslatef(0.0 + trax, 0.0 + tray, 0.0 + traz);
	glPopMatrix();
}
/*********** ARBOLES ***********/
void layerGeneral(int layer,int lado, GLuint text, GLuint text2) {
	float bloqueAz = 1.4, unionAB = 0.3, bloqueBz = 1.4, unionBC = 0.3, bloqueCz = 1.4, escalaX1 = 0.3, escalaX2 = 0.9;
	float distTronco = 0.6;
	set_material(emerald);
	switch (layer) {
		case 1:
			//BloqueA inicial
			escalaX1 = 0.7;
			break;
		case 2:
			//BloqueA exp unionAB inicial
			bloqueAz *= 2;
			break;
		case 3:
			//BloqueA exp, unionAB estable
			bloqueAz *= 3;
			unionAB *= 2;
			break;
		case 4:
			//BloqueA exp, unionAB estable, BloqueB inicial
			bloqueAz *= 4;
			unionAB *= 2;
			break;
		case 5:
			//BloqueA exp, unionAB estable, BloqueB exp
			bloqueAz *= 5;
			unionAB *= 2;
			bloqueBz *= 2;
			break;
		case 6:
			//BloqueA exp, unionAB estable, BloqueB exp, unionBC incial
			bloqueAz *= 6;
			unionAB *= 2;
			bloqueBz*= 3;
			break;
		case 7:
			//BloqueA exp, unionAB estable, BloqueB exp, unionBC
			bloqueAz *= 7;
			unionAB *= 2;
			bloqueBz *= 4;
			unionBC *= 2;
			break;
		case 8:
			//BloqueA exp, unionAB estable, BloqueB exp, unionBC, Bloque C ini
			bloqueAz *= 8;
			unionAB *= 2;
			bloqueBz *= 5;
			unionBC *= 2;
			break;
		case 9:
			bloqueAz *= 9;
			unionAB *= 2;
			bloqueBz *= 6;
			unionBC *= 2;
			bloqueCz *= 2;
			//BloqueA exp, unionAB estable, BloqueB exp, unionBC, Bloque C exp
			break;
		default:
			distTronco = 0.0;
			break;
	}
	if (distTronco > 0.0) {
		glPushMatrix();
			//Lado derecho
			glPushMatrix();
			// Primer bloque, pegado al tronco
			if (layer <= 1) {
				//BloqueA
				glTranslatef(lado * (distTronco + (escalaX1 / 2)), 0.0, 0.0);
				brick.prisma(0.7, escalaX1, bloqueAz, text);
			}
			else {
				//BloqueA
				glTranslatef(lado * (distTronco + (escalaX2 / 2)), 0.0, 0.0);
				brick.prisma(0.7, escalaX2, bloqueAz, text);
				glPushMatrix();
				if (layer <= 2) {
					// UnionAB 1a exp
					glTranslatef(lado * (unionAB / 2 + escalaX2 / 2), 0.0, 0.0);
					brick.prisma(0.7, unionAB,unionAB, text2);
				}
				else {
					// UnionAB estable
					glTranslatef(lado * (unionAB / 2 + escalaX2 / 2), 0.0, 0.0);
					brick.prisma(0.7, unionAB, unionAB, text2);
					glPushMatrix();
					if (layer <= 4) {
						//BloqueB ini
						glTranslatef(lado*(unionAB/2 + escalaX1 / 2), 0.0, 0.0);
						brick.prisma(0.7, escalaX1, bloqueBz, text);
					}
					else {
						//BloqueB 2a exp
						glTranslatef(lado * (unionAB / 2 + escalaX2 / 2), 0.0, 0.0);
						brick.prisma(0.7, escalaX2, bloqueBz, text);
						glPushMatrix();
						if (layer <= 6) {
							//UnionBC ini
							glTranslatef(lado * (unionBC / 2 + escalaX2 / 2), 0.0, 0.0);
							brick.prisma(0.7, unionBC, unionBC, text2);
						}
						else {
							//UnionBC exp
							glTranslatef(lado * (unionBC / 2 + escalaX2 / 2), 0.0, 0.0);
							brick.prisma(0.7, unionBC, unionBC, text2);
							glPushMatrix();
							if (layer <= 8) {
								//Tercer bloque 1a exp
								glTranslatef(lado*(unionBC/2 + escalaX1/2), 0.0, 0.0);
								brick.prisma(0.7, escalaX1, bloqueCz, text);
							}
							else {
								//Tercer bloque 1a exp
								glTranslatef(lado* (unionBC / 2 + escalaX2 / 2), 0.0, 0.0);
								brick.prisma(0.7, escalaX2, bloqueCz, text);
							}
							glPopMatrix();
						}
						glPopMatrix();
					}
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();
		glPopMatrix();
	}
}
void tronco() {
	glPushMatrix();
	set_material(madera);
	brick.cilindro(0.6, 20.0 , 5, t_madera.GLindex);
	glPopMatrix();
}
void createArbol(GLuint text, GLuint text2) {
	glPushMatrix();
		tronco();
		glTranslatef(0.0, 20.5, 0.0);
		brick.prisma(1.0, 1.0, 1.0, text);
		glPushMatrix();
			for (int i = 1; i < 10; i++) {
				glTranslatef(0.0, -1.5, 0.0);
				layerGeneral(i, 1, text, text2);
			}
		glPopMatrix();
		glPushMatrix();
			for (int i = 1; i < 10; i++) {
				glTranslatef(0.0, -1.5, 0.0);
				layerGeneral(i, -1, text, text2);
			}
		glPopMatrix();
	glPopMatrix();
}
void createArboles(GLuint text, GLuint text2) {
	glPushMatrix();
		//Jardines derechos
		//glTranslatef(0.0 + trax, 0.0 + tray, 0.0 + traz);
		glTranslatef(12.0, 0.0, -36.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(34.0, 0.0, -25.0);
		glRotatef(195.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(14.0, 0.0, -18.0);
		glRotatef(30.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(53.0, 0.0, -32.0);
		glRotatef(85.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(70.0, 0.0, -20.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(25.0, 0.0, -40.0);
		glRotatef(153.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	//Abajo
	glPushMatrix();
		glTranslatef(12.0, 0.0, 36.0);
		glRotatef(45.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(23.0, 0.0, 20.0);
		glRotatef(70.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(37.0, 0.0, 29.0);
		glRotatef(153.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(51.0, 0.0, 35.0);
		glRotatef(34.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(68.0, 0.0, 27.0);
		glRotatef(69.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	//Jardineraz IZquierdas
	//Abajo
	glPushMatrix();
		glTranslatef(-26.0, 0.0, 27.0);
		glRotatef(69.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-39.0, 0.0, 35.0);
		glRotatef(29.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-53.0, 0.0, 18.0);
		glRotatef(79.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	//Arriba
	glPushMatrix();
		glTranslatef(-26.0, 0.0, -20.0);
		glRotatef(17.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-39.0, 0.0, -35.0);
		glRotatef(135.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-53.0, 0.0, -30.0);
		glRotatef(59.0, 0.0, 1.0, 0.0);
		createArbol(text, text2);
	glPopMatrix();
}
/*********** TIENDAS ***********/
void createTecho(GLuint text, GLuint text2, GLuint text3, GLuint text4){
	glPushMatrix();
		//glTranslatef(-0.5 + trax, 12.6 + tray, 13.5 + traz);
		glTranslatef(-0.5, 12.6, 14.0);
		brick.flatV(21.0, 14.0, text, text3);
		glPushMatrix();
			//Tejado frontal
			glTranslatef(0.0, -1.6, 6.5);
			brick.slopeV(17.0, 3.0, 1.0, text, text2, false);
			glPushMatrix();
				//Corners
				glTranslatef(-9.5, 0.0, -0.5);
				brick.cornerV(2.0, 3.0, 2.0, text, text4, false);
				glTranslatef(19.0, 0.0, 0.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				brick.cornerV(2.0, 3.0, 2.0, text, text4, false);
			glPopMatrix();
			glPushMatrix();
				//Tejado izquierdo
				glTranslatef(-10.0, 0.0, -7.5);
				glRotatef(-90.0, 0.0, 1.0, 0.0);
				brick.slopeV(12.0, 3.0, 1.0, text, text3, false);
			glPopMatrix();
			glPushMatrix();
				//Tejado derecho
				glTranslatef(10.0, 0.0, -7.5);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				brick.slopeV(12.0, 3.0, 1.0, text, text3, false);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}
void createParedes(GLuint text, GLuint text2, GLuint text3, GLuint text4, materialStruct mat1, materialStruct mat2, materialStruct mat3, materialStruct mat4) {
	GLuint textG = 0.0;
	glPushMatrix();
		//Pared Tras
		glPushMatrix();
			//Tienda de 15 de ancho, 19 de largo +-
			glTranslatef(-9.5, 0.0, 7.5);
			for (int i = 0; i < 13; i++) {//Hacia arriba
				glPushMatrix();
				for (int j = 1; j < 20; j++) {//Hacia un lado
					if (j % 2 == 0) {
						textG = text;
						set_material(mat1);
					}
						
					else if (j % 3 == 0) {
						textG = text2;
						set_material(mat2);
					}
					else if (j % 7 == 0) {
						textG = text3;
						set_material(mat3);
					}
					else {
						textG = text4;
						set_material(mat4);
					}
						
					brick.prisma(1.0, 1.0, 1.0, textG);
					glTranslatef(1.0, 0.0, 0.0);
				}
				glPopMatrix();
				glTranslatef(0.0, 1.0, 0.0);
			}
		glPopMatrix();
		//Pared Izq
		glPushMatrix();
			glTranslatef(-9.5, 0.0, 7.5);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			glTranslatef(-1.0, 0.0, 0.0);
			for (int i = 0; i < 13; i++) {//Hacia arriba
				glPushMatrix();
				for (int j = 2; j < 13; j++) {//Hacia un lado
					if (j % 2 == 0) {
						textG = text;
						set_material(mat1);
					}
					else if (j % 3 == 0) {
						textG = text2;
						set_material(mat2);
					}
					else if (j % 7 == 0) {
						textG = text3;
						set_material(mat3);
					}
					else{
						textG = text4;
						set_material(mat4);
					}
						
					brick.prisma(1.0, 1.0, 1.0, textG);
					glTranslatef(-1.0, 0.0, 0.0);
				}
				glPopMatrix();
				glTranslatef(0.0, 1.0, 0.0);
			}
		glPopMatrix();

		//Pared Der
		glPushMatrix();
			glTranslatef(8.5, 0.0, 7.5);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			glTranslatef(-1.0, 0.0, 0.0);
			for (int i = 0; i < 13; i++) {//Hacia arriba
				glPushMatrix();
				for (int j = 2; j < 13; j++) {//Hacia un lado
					if (j % 2 == 0) {
						textG = text;
						set_material(mat1);
					}
					else if (j % 3 == 0) {
						textG = text2;
						set_material(mat2);
					}
					else if (j % 7 == 0) {
						textG = text3;
						set_material(mat3);
					}
					else {
						textG = text4;
						set_material(mat4);
					}
					brick.prisma(1.0, 1.0, 1.0, textG);
					glTranslatef(-1.0, 0.0, 0.0);
				}
				glPopMatrix();
				glTranslatef(0.0, 1.0, 0.0);
			}
		glPopMatrix();
		//Front
		glPushMatrix();
			glTranslatef(-8.5, 0.0, 19.5);		
			for (int i = 0; i < 3; i++) {//Hacia arriba
				glPushMatrix();
				for (int j = 2; j < 19; j++) {//Hacia un lado
					if (j % 2 == 0) {
						textG = text2;
						set_material(mat2);
					}
					else if (j % 3 == 0) {
						textG = text3;
						set_material(mat3);
					}
					else if (j % 7 == 0) {
						textG = text4;
						set_material(mat4);
					}
					else {
						textG = text;
						set_material(mat1);
					}
					brick.prisma(1.0, 1.0, 1.0, textG);
					glTranslatef(1.0, 0.0, 0.0);
				}
				glPopMatrix();
				glTranslatef(0.0, 1.0, 0.0);
			}
			glPushMatrix();
				glTranslatef(0.0, -1.0, -1.0);
				for (int i = 0; i < 2; i++) {
					glPushMatrix();
					for (int j = 2; j < 19; j++) {//Hacia un lado
						if (j % 2 == 0) {
							textG = text2;
							set_material(mat2);
						}
						else if (j % 3 == 0) {
							textG = text3;
							set_material(mat3);
						}
						else if (j % 7 == 0) {
							textG = text4;
							set_material(mat4);
						}
						else {
							textG = text;
							set_material(mat1);
						}
						brick.prisma(1.0, 1.0, 1.0, textG);
						glTranslatef(1.0, 0.0, 0.0);
					}
					glPopMatrix();
					glTranslatef(0.0, 0.0, -1.0);
				}
			glPopMatrix();
			glTranslatef(0.0, 6.0, 0.0);
			for (int i = 7; i < 11; i++) {//Hacia arriba
				glPushMatrix();
				for (int j = 2; j < 19; j++) {//Hacia un lado
					if (j % 2 == 0) {
						textG = text;
						set_material(mat1);
					}
					else if (j % 3 == 0) {
						textG = text2;
						set_material(mat2);
					}
					else if (j % 7 == 0) {
						textG = text3;
						set_material(mat3);
					}
					else {
						textG = text4;
						set_material(mat4);
					}
					brick.prisma(1.0, 1.0, 1.0, textG);
					glTranslatef(1.0, 0.0, 0.0);
				}
				glPopMatrix();
				glTranslatef(0.0, 1.0, 0.0);
			}
		glPopMatrix();
	glPopMatrix();
}
void createTienda(float dir1, float dir2, GLuint text, GLuint text2, GLuint text3, GLuint text4, materialStruct mat1, materialStruct mat2, materialStruct mat3, materialStruct mat4){
	glPushMatrix();
		glTranslatef(dir1*8.0, 0.0, dir2*52.0);
		createParedes(text, text2, text3, text4, mat1, mat2, mat3, mat4);
		createTecho(text, text2, text3, text4);
		//t_madera.GLindex, t_amarillo.GLindex, t_morado.GLindex, t_morado.GLindex
	glPopMatrix();
}
void createTiendas(){
	glPushMatrix();
		createTienda(-1.0,-1.0, t_maderaO.GLindex, t_amarillo.GLindex, t_rosa2.GLindex, t_rosa2.GLindex, madera, gold, rosa_palido, rosa_palido);
	glPopMatrix();
	glPushMatrix();
		glRotatef(180.0, 0.0, 1.0, 0.0);
		createTienda(1.125,-1.0, t_madera.GLindex, t_azul.GLindex, t_morado.GLindex, t_morado.GLindex, madera, azul, morado, morado);
	glPopMatrix();
}
/*********** BAÑOS	 ***********/
void createParedesB(GLuint text, bool sexo, GLuint sexBano) {
	float largoX = 0.0, largoY = 0.0;
	if (sexo == true) {
		largoX = 23.0;
		largoY = 20.0;
	}
	else {
		largoX = 20.0;
		largoY = 24.0;
	}
	glPushMatrix();
		//Pared Tras
		glTranslatef(0.0, 0.0, 81.5);
		glPushMatrix();
			//Bano de 13 de ancho, 21 de largo +-
			glTranslatef(1.5, 0.0, 0.0);
			for (int i = 0; i < 15; i++) {//Hacia arriba
				glPushMatrix();
				for (int j = 1; j < 13; j++) {//Hacia un lado
					brick.prisma(1.0, 1.0, 1.0, text);
					glTranslatef(1.0, 0.0, 0.0);
				}
				glPopMatrix();
				glTranslatef(0.0, 1.0, 0.0);
			}
		glPopMatrix();
		//Pared Der
		glPushMatrix();
			//glTranslatef(-9.5, 0.0, 7.5);
			glTranslatef(0.5, 0.0, 0.0);
			glRotatef(-90.0, 0.0, 1.0, 0.0);
			glTranslatef(-1.0, 0.0, 0.0);
			for (int i = 0; i < 15; i++) {//Hacia arriba
				glPushMatrix();
				for (int j = 0; j < largoX; j++) {//Hacia un lado
					brick.prisma(1.0, 1.0, 1.0, text);
					glTranslatef(-1.0, 0.0, 0.0);
				}
				glPopMatrix();
				glTranslatef(0.0, 1.0, 0.0);
			}
		glPopMatrix();
		//Pared Izq
		glPushMatrix();
			glTranslatef(13.5, 0.0, 0.0);
			glRotatef(-90.0, 0.0, 1.0, 0.0);
			glTranslatef(-1.0, 0.0, 0.0);
			for (int i = 0; i < 15; i++) {//Hacia arriba
				glPushMatrix();
				for (int j = 0; j < largoY; j++) {//Hacia un lado
					brick.prisma(1.0, 1.0, 1.0, text);
					glTranslatef(-1.0, 0.0, 0.0);
				}
				glPopMatrix();
				glTranslatef(0.0, 1.0, 0.0);
			}
		glPopMatrix();
			//Front
		glPushMatrix();
			if (sexo == true) {
				glPushMatrix();
					glTranslatef(0.5, 0.0, -24.0);
					for (int i = 0; i < 15; i++) {//Hacia arriba
						glPushMatrix();
						for (int j = 0; j < 5; j++) {//Hacia un lado
							brick.prisma(1.0, 1.0, 1.0, text);
							glTranslatef(1.0, 0.0, 0.0);
						}
						glPopMatrix();
						glTranslatef(0.0, 1.0, 0.0);
					}
				glPopMatrix();
				glTranslatef(4.5, 0.0, -20.0);
			}
			else {
				glPushMatrix();
					glTranslatef(8.5, 0.0, -24.0);
					for (int i = 0; i < 15; i++) {//Hacia arriba
						glPushMatrix();
						for (int j = 0; j < 5; j++) {//Hacia un lado
							brick.prisma(1.0, 1.0, 1.0, text);
							glTranslatef(1.0, 0.0, 0.0);
						}
						glPopMatrix();
						glTranslatef(0.0, 1.0, 0.0);
					}
				glPopMatrix();
				glTranslatef(0.5, 0.0, -20.0);
			}
			for (int i = 0; i < 15; i++) {//Hacia arriba
				glPushMatrix();
				for (int j = 0; j < 9; j++) {//Hacia un lado
					brick.prisma(1.0, 1.0, 1.0, text);
					glTranslatef(1.0, 0.0, 0.0);
				}
				glPopMatrix();
				glTranslatef(0.0, 1.0, 0.0);
			}
			glPushMatrix();
				glTranslatef(4.0, -7.0, -0.5);
				brick.prisma(5.1, 5.0, 0.101, sexBano);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}
void createBano() {
	glPushMatrix();
		glRotatef(90.0, 0.0, 1.0, 0.0);
		createParedesB(t_blanco.GLindex,true, t_banoHombre.GLindex);
		glTranslatef(-14.0 , 0.0, 0.0);
		createParedesB(t_blanco.GLindex, false, t_banoMujer.GLindex);
	glPopMatrix();
}
void createTechoB(GLuint text, GLuint text2) {
	glPushMatrix();
		//glTranslatef(69.5 + trax, 14.7 + tray, 0.0 + traz);
		glTranslatef(69.5, 14.7, 0.0);
		brick.flatV(25.0, 28.0, text, text2);
	glPopMatrix();
}
void createWCs(float scale) {
	//0.108
	glPushMatrix();
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glTranslatef(-80.0, -0.5, -10.0);
		glScalef(scale, scale, scale);
		wc.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();
	glPushMatrix();
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glTranslatef(-80.0, -0.5, -5.0);
		glScalef(scale, scale, scale);
		wc.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();
	glPushMatrix();
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glTranslatef(-80.0, -0.5, 10.0);
		glScalef(scale, scale, scale);
		wc.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();
	glPushMatrix();
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glTranslatef(-80.0, -0.5, 5.0);
		glScalef(scale, scale, scale);
		wc.GLrender(NULL, _SHADED, 1.0);
	glPopMatrix();
}
void createLavabos(float scale) {
	glPushMatrix();
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glPushMatrix();
			glTranslatef(-12.0, 2.0, 63.5);
			glScalef(scale, scale, scale);
			lavaManos.GLrender(NULL, _SHADED, 1.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-8.0, 2.0, 63.5);
			glScalef(scale, scale, scale);
			lavaManos.GLrender(NULL, _SHADED, 1.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(6.0, 2.0, 63.5);
			glScalef(scale, scale, scale);
			lavaManos.GLrender(NULL, _SHADED, 1.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(10.0, 2.0, 63.5);
			glScalef(scale, scale, scale);
			lavaManos.GLrender(NULL, _SHADED, 1.0);
		glPopMatrix();
	glPopMatrix();
}
void createPuertas(GLuint text,float pos) {
	glPushMatrix();
		set_material(madera);
		glTranslatef(73.0, 0.0, pos);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		for (int i = 0; i < 12; i++) {
			glPushMatrix();
			for (int j = 0; j < 7; j++) {
				brick.prisma(1.0, 1.0, 0.5, text);
				glTranslatef(-1.0, 0.0, 0.0);
			}
			glPopMatrix();
			glTranslatef(0.0, 1.0, 0.0);
		}
		glTranslatef(0.0, -1.0, 0.0);
		glPushMatrix();
			glTranslatef(0.5+1.25, -1.0, 0.0);
			brick.prisma(3.0, 2.5, 0.5,text);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-7.5 - 0.25, -1.0, 0.0);
			brick.prisma(3.0, 2.5, 0.5, text);
		glPopMatrix();
		glPushMatrix();
			set_material(perl);
			glTranslatef(-3.0, 0.6, 0.0);
			brick.flatV(12.0, 1.0, text, t_blanco.GLindex);
		glPopMatrix();
		glPushMatrix();
			set_material(madera);
			glTranslatef(-3.0, -11.0, 0.8);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			for (int i = 0; i < 12; i++) {
				glPushMatrix();
				for (int j = 0; j < 8; j++) {
					brick.prisma(1.0, 1.0, 0.5, text);
					glTranslatef(-1.0, 0.0, 0.0);
				}
				glPopMatrix();
				glTranslatef(0.0, 1.0, 0.0);
			}
			glTranslatef(0.0, -1.0, 0.0);
		glPopMatrix();
	glPopMatrix();
}
void createInteriorB() {
	createWCs(0.135);
	createLavabos(0.088);
}
void createBanos() {
	glPushMatrix();
		set_material(perl);
		createBano();
		createTechoB(t_blanco.GLindex, t_turquesa.GLindex);
		if(banderaModelos==true)
			createInteriorB();
		createPuertas(t_madera.GLindex, 4.0);
		createPuertas(t_madera.GLindex, -10.0);
	glPopMatrix();
}
/*********** FAROLAS ***********/
void createFarola(GLuint cuerpo, GLuint lampara, int source, float altoPalo) {
	/*float largoEx = 0.0, largoIn = 0.0;
	if (altoPalo == 16.0) {
		largoEx = 3.0;
		largoIn = 1.5;
	}
	else {
		largoEx = 
	}*/
	glPushMatrix();
		//glTranslatef(0.0 + trax, 0.0 + tray, 0.0 + traz);
		glPushMatrix();
			set_material(obsidiana);
			//Base
			glTranslatef(0.0, 0.25, 0.0);
			brick.prisma(0.5, 3.0, 3.0, cuerpo);
			glTranslatef(0.0, 0.25 + 0.15, 0.0);
			brick.prisma(0.3, 1.5, 1.5, cuerpo);
			//Palo
			glTranslatef(0.0, 0.15, 0.0);
			brick.cilindro(0.25, altoPalo, 8, cuerpo);
			glTranslatef(0.0, altoPalo, 0.0);
			glPushMatrix();
				glTranslatef(2.0, 0.25, 0.0);
				brick.prisma(0.5, 4.0, 2.0, cuerpo);
				glTranslatef(1.0, -0.5, 0.0);
				set_material(gold);
				brick.prisma(0.5, 2.0, 1.4, lampara);
				glTranslatef(0.0, -0.25, 0.0);
				glPushMatrix();
					glRotatef(180.0, 0.0, 0.0, 1.0);
					set_material(perl);
					brick.brick1_1_1(lampara, t_blanco.GLindex);
				glPopMatrix();
				glTranslatef(0.0, -0.25 - 0.15 - 0.9, 0.0);
				if (source == 6)
					foc2();
				else if (source == 7)
					foc3();
				else
					foc1(source);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}
void createFarolaKeyFrame(GLuint cuerpo, GLuint lampara, int source, float altoPalo) {
	glPushMatrix();
		glPushMatrix();
		/********	KEYFRAME	********/
			glTranslatef(0.0 + traxFrame, 0.0 + trayFrame, -0.5 + trazFrame);
			//glTranslatef(0.0 + trax, 0.0 + tray, -3.0 + traz);
		/********	KEYFRAME	********/
			set_material(obsidiana);
			//Base
			glTranslatef(0.0, 0.25, 0.0);
			brick.prisma(0.5, 3.0, 3.0, cuerpo);
			glTranslatef(0.0, 0.25 + 0.15, 0.0);
			brick.prisma(0.3, 1.5, 1.5, cuerpo);
			//Palo
			glTranslatef(0.0, 0.15, 0.0);
			brick.cilindro(0.25, altoPalo, 8, cuerpo);
			glTranslatef(0.0, altoPalo, 0.0);
			/********	KEYFRAME	********/
			glRotatef(rotLampX, 1.0, 0.0, 0.0);
			glRotatef(rotLampY, 0.0, 1.0, 0.0);
			glRotatef(rotLampZ, 0.0, 0.0, 1.0);
			/*glRotatef(scaleX, 1.0, 0.0, 0.0);
			glRotatef(scaleY, 0.0, 1.0, 0.0);
			glRotatef(scaleZ, 0.0, 0.0, 1.0);*/
			/********	KEYFRAME	********/
			glPushMatrix();
				glTranslatef(2.0, 0.25, 0.0);
				brick.prisma(0.5, 4.0, 2.0, cuerpo);
				glTranslatef(1.0, -0.5, 0.0);
				set_material(gold);
				brick.prisma(0.5, 2.0, 1.4, lampara);
				glTranslatef(0.0, -0.25, 0.0);
				glPushMatrix();
					glRotatef(180.0, 0.0, 0.0, 1.0);
					set_material(perl);
					brick.brick1_1_1(lampara, t_blanco.GLindex);
				glPopMatrix();
				glTranslatef(0.0, -0.25 - 0.15 - 0.9, 0.0);
				if (source == 6)
					foc2();
				else if (source == 7)
					foc3();
				else
					foc1(source);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}
void createFarolas() {
	glPushMatrix();

	//glTranslatef(0.0 + trax, 0.0 + tray, 0.0 + traz);
	//Farola 1
	glPushMatrix();
	glTranslatef(-76.0, 0.0, -31.0);
	//glRotatef(angleY1, 0.0, 1.0, 0.0);
	createFarola(t_carbon.GLindex, t_amarillo2.GLindex, 1, 16.0);
	glPopMatrix();
	//Farola 2
	glPushMatrix();
	glTranslatef(-76.0, 0.0, 31.0);
	//glRotatef(angleY1, 0.0, 1.0, 0.0);
	createFarola(t_carbon.GLindex, t_amarillo2.GLindex, 0, 16.0);
	glPopMatrix();
	//Farola 3
	//A
	glPushMatrix();
	glTranslatef(-38.5, 0.0, -12.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	createFarola(t_carbon.GLindex, t_amarillo2.GLindex, 3, 16.0);
	glPopMatrix();
	//B
	glPushMatrix();
	glTranslatef(-38.5, 0.0, 12.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	createFarola(t_carbon.GLindex, t_amarillo2.GLindex, 0, 16.0);
	glPopMatrix();
	//Farola 4
	glPushMatrix();
	glTranslatef(-16.0, 0.0, -22.0);
	//glRotatef(0.0, 0.0, 1.0, 0.0);
	createFarola(t_carbon.GLindex, t_amarillo2.GLindex, 4, 16.0);
	glPopMatrix();

	//Farola 5
	glPushMatrix();
	glTranslatef(-0.5, 0.0, 25.0);
	glRotatef(180.0, 0.0, 1.0, 0.0);
	createFarola(t_carbon.GLindex, t_amarillo2.GLindex, 5, 16.0);
	glPopMatrix();
	//Farola 6

	//A
	glPushMatrix();
	glTranslatef(36.0, 0.0, 12.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	createFarola(t_carbon.GLindex, t_amarillo2.GLindex, 2, 16.0);
	glPopMatrix();
	//B
	glPushMatrix();
	glTranslatef(36.0, 0.0, -12.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	createFarola(t_carbon.GLindex, t_amarillo2.GLindex, 0, 16.0);
	glPopMatrix();
	glPopMatrix();
}
/*********** KIOSKO	 ***********/
void baseLado(GLuint text) {
	glPushMatrix();
		//set_material(mat1);
		
		//1*4 left
		glPushMatrix();
			glTranslatef(-4.5, 0.5, 0.0);
			glPushMatrix();
				glTranslatef(0.0, 0.0, -1.0);
				brick.prisma(1.0, 1.0, 2.0, text);
				glTranslatef(0.0, 0.0, 2.0);
				brick.prisma(1.0, 1.0, 2.0, text);
			glPopMatrix();
			glTranslatef(0.0, 1.0, 0.0);
			glPushMatrix();
				glTranslatef(0.0, 0.0, -1.0);
				brick.prisma(1.0, 1.0, 2.0, text);
				glTranslatef(0.0, 0.0, 2.0);
				brick.prisma(1.0, 1.0, 2.0, text);
			glPopMatrix();
		glPopMatrix();
		//1*2 mid
		glPushMatrix();
			glTranslatef(-3.5, 0.5, 0.0);
			glPushMatrix();
				glTranslatef(0.0, 0.0, -2.0);
				brick.prisma(1.0, 1.0, 2.0, text);
				glTranslatef(0.0, 0.0, 4.0);
				brick.prisma(1.0, 1.0, 2.0, text);
			glPopMatrix();
			glTranslatef(0.0, 1.0, 0.0);
			glPushMatrix();
				glTranslatef(0.0, 0.0, -2.0);
				brick.prisma(1.0, 1.0, 2.0, text);
				glTranslatef(0.0, 0.0, 4.0);
				brick.prisma(1.0, 1.0, 2.0, text);
			glPopMatrix();
		glPopMatrix();
		//1*2 Right
		glPushMatrix();
			glTranslatef(-2.5, 0.5, 0.0);
			glPushMatrix();
				glTranslatef(0.0, 0.0, -3.0);
				brick.prisma(1.0, 1.0, 2.0, text);
				glTranslatef(0.0, 0.0, 6.0);
				brick.prisma(1.0, 1.0, 2.0, text);
			glPopMatrix();
			glTranslatef(0.0, 1.0, 0.0);
			glPushMatrix();
				glTranslatef(0.0, 0.0, -3.0);
				brick.prisma(1.0, 1.0, 2.0, text);
				glTranslatef(0.0, 0.0, 6.0);
				brick.prisma(1.0, 1.0, 2.0, text);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}
void createBase(GLuint text, materialStruct mat1) {
	glPushMatrix();
		set_material(mat1);
		glPushMatrix();
			baseLado(text);
			glRotatef(180.0, 0.0, 1.0, 0.0);
			baseLado(text);
		glPopMatrix();
		//Front-Back
		glPushMatrix();
			glTranslatef(0.0, 0.5, -4.5);
			glPushMatrix();
				glTranslatef(-1.0, 0.0, 0.0);
				brick.prisma(1.0, 2.0, 1.0, text);
				glTranslatef(2.0, 0.0, 0.0);
				brick.prisma(1.0, 2.0, 1.0, text);
			glPopMatrix();
			glTranslatef(0.0, 1.0, 0.0);
			glPushMatrix();
				glTranslatef(-1.0, 0.0, 0.0);
				brick.prisma(1.0, 2.0, 1.0, text);
				glTranslatef(2.0, 0.0, 0.0);
				brick.prisma(1.0, 2.0, 1.0, text);
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			glRotatef(180.0, 0.0, 1.0, 0.0);
			glTranslatef(0.0, 0.5, -4.5);
			glPushMatrix();
				glTranslatef(-1.0, 0.0, 0.0);
				brick.prisma(1.0, 2.0, 1.0, text);
				glTranslatef(2.0, 0.0, 0.0);
				brick.prisma(1.0, 2.0, 1.0, text);
			glPopMatrix();
			glTranslatef(0.0, 1.0, 0.0);
			glPushMatrix();
				glTranslatef(-1.0, 0.0, 0.0);
				brick.prisma(1.0, 2.0, 1.0, text);
				glTranslatef(2.0, 0.0, 0.0);
				brick.prisma(1.0, 2.0, 1.0, text);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}
void pilaresIzqDer(GLuint text) {
	float base = 1.2;
	float radio = 0.3;
	glPushMatrix();
		glPushMatrix();
			glTranslatef(-4.5, 0.0, -1.5);
			glTranslatef(0.0, 0.25, 0.0);
			brick.prisma(0.5, base, base, text);
			glTranslatef(0.0, 0.25 + 0.15, 0.0);
			brick.prisma(0.3, base / 2, base / 2, text);
			brick.cilindro(radio, 16.0, 8, text);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-4.5, 0.0, 1.5);
			glTranslatef(0.0, 0.25, 0.0);
			brick.prisma(0.5, base, base, text);
			glTranslatef(0.0, 0.25 + 0.15, 0.0);
			brick.prisma(0.3, base / 2, base / 2, text);
			brick.cilindro(radio, 16.0, 8, text);
		glPopMatrix();
	glPopMatrix();
}
void pilaresEntrada(GLuint text) {
	float base = 1.2;
	float radio = 0.3;
	glPushMatrix();
		glPushMatrix();
			glTranslatef(-3.5, 0.0, -2.5);
			glTranslatef(0.0, 0.25, 0.0);
			brick.prisma(0.5, base, base, text);
			glTranslatef(0.0, 0.25 + 0.15, 0.0);
			brick.prisma(0.3, base / 2, base / 2, text);
			brick.cilindro(radio, 16.0, 8, text);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-3.5, 0.0, 2.5);
			glTranslatef(0.0, 0.25, 0.0);
			brick.prisma(0.5, base, base, text);
			glTranslatef(0.0, 0.25 + 0.15, 0.0);
			brick.prisma(0.3, base / 2, base / 2, text);
			brick.cilindro(radio, 16.0, 8, text);
		glPopMatrix();
	glPopMatrix();
}
void createPilares(GLuint text, materialStruct mat1) {
	glPushMatrix();
		//glTranslatef(0.0, 3.2, 0.0);
		//Izq-Der
		set_material(mat1);
		glTranslatef(0.0, 0.0, 0.0);
		glPushMatrix();
			pilaresEntrada(text);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			pilaresIzqDer(text);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			pilaresIzqDer(text);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			pilaresIzqDer(text);
		glPopMatrix();
		//glTranslatef(-1.0 + trax, 0.0, -1.0 + traz);
		//glRotatef(180.0, 0.0, 1.0, 0.0);
	glPopMatrix();
}
void createEscaleras(GLuint text, materialStruct mat1, int level) {
	glPushMatrix();
		//glRotatef(90.0, 0.0, 0.0, 1.0);
		for (int i = 0; i < 4; i++) {
			glPushMatrix();
				for (int j = 0; j < level; j++) {
					brick.prisma(1.0, 1.0, 1.0, text);
					glTranslatef(1.0, 0.0, 0.0);
				}
			glPopMatrix();
			glTranslatef(0.0, 0.0, 1.0);
		}
	glPopMatrix();
}
void bardaDoble(GLuint text, GLuint text2, materialStruct mat1, materialStruct mat2) {
	glPushMatrix();
		glTranslatef(-0.5, 0.0, -4.5);
		glPushMatrix();
			set_material(mat1);
			brick.prisma(1.0, 0.8, 0.8, text);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, 0.8, 0.8, text);
			glTranslatef(0.0, 1.0, 0.0);
			set_material(mat2);
			brick.brick1_1_1(text, text2);
		glPopMatrix();
		glTranslatef(1.0, 0.0, 0.0);
		glPushMatrix();
			set_material(mat1);
			brick.prisma(1.0, 0.8, 0.8, text);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, 0.8, 0.8, text);
			glTranslatef(0.0, 1.0, 0.0);
			set_material(mat2);
			brick.brick1_1_1(text, text2);
		glPopMatrix();
	glPopMatrix();
}
void createBarda(GLuint text, GLuint text2, materialStruct mat1, materialStruct mat2) {
	float largoX = 0.8;
	glPushMatrix();
		glTranslatef(2.5, 0.5, 4.5);
		glPushMatrix();
			set_material(mat1);
			//brick.prisma(altura, largo, profundidad)
			brick.prisma(1.0, largoX, largoX, text);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, largoX, largoX, text);
			glTranslatef(0.0, 1.0, 0.0);
			set_material(mat2);
			brick.brick1_1_1(text, text2);
		glPopMatrix();
		glTranslatef(1.0, 1.0, 0.0);
		set_material(mat1);
		brick.brick1V(text, text2, 1.2, 1.0, 1.0);
		glTranslatef(1.0, -1.0, 0.0);
		glPushMatrix();
			brick.prisma(1.0, largoX, largoX, text);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, largoX, largoX, text);
			glTranslatef(0.0, 1.0, 0.0);
			set_material(mat2);
			brick.brick1_1_1(text, text2);
		glPopMatrix();
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glTranslatef(1.0, 1.0, 0.0);
		brick.brick1V(text, text2, 1.2, 1.0, 1.0);
		glTranslatef(1.0, -1.0, 0.0);
		glPushMatrix();
			set_material(mat1);
			brick.prisma(1.0, largoX, largoX, text);
			glTranslatef(0.0, 1.0, 0.0);
			brick.prisma(1.0, largoX, largoX, text);
			glTranslatef(0.0, 1.0, 0.0);
			set_material(mat2);
			brick.brick1_1_1(text, text2);
		glPopMatrix();
	glPopMatrix();
}
void createBardas(GLuint text, GLuint text2, materialStruct mat1, materialStruct mat2) {
	glPushMatrix();
		//Huecos
		glPushMatrix();
			//glTranslatef(-0.5 + trax, 0.0 + tray, -4.5 + traz);
			bardaDoble(text, text2, mat1, mat2);
			glRotatef(-90.0, 0.0, 1.0, 0.0);
			bardaDoble(text, text2, mat1, mat2);
			glRotatef(-90.0, 0.0, 1.0, 0.0);
			bardaDoble(text, text2, mat1, mat2);
		glPopMatrix();
		//Rodeo
		createBarda(text, text2, mat1, mat2);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		createBarda(text, text2, mat1, mat2);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		createBarda(text, text2, mat1, mat2);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		createBarda(text, text2, mat1, mat2);
	glPopMatrix();
}
void escalerasFull() {
	glPushMatrix();
	set_material(perl);
		glTranslatef(-8.0, 0.5, 0.0);
		glTranslatef(-5.5, 0.0, 1.5);
		glRotatef(180.0, 0.0, 1.0, 0.0);
		createEscaleras(t_blanco.GLindex, perl, 6);
		glTranslatef(0.0, 1.0, 0.0);
		createEscaleras(t_blanco.GLindex, perl, 5);
		glTranslatef(0.0, 1.0, 0.0);
		createEscaleras(t_madera.GLindex, polishedCopper, 4);
		glTranslatef(0.0, 1.0, 0.0);
		createEscaleras(t_madera.GLindex, polishedCopper, 3);
		glTranslatef(0.0, 1.0, 0.0);
		createEscaleras(t_maderaO.GLindex, madera, 2);
		glTranslatef(0.0, 1.0, 0.0);
		createEscaleras(t_maderaO.GLindex, madera, 1);
	glPopMatrix();
}
void slopesTecho(GLuint text, GLuint text2, materialStruct mat1, float distCentro, float largo) {
	bool cil = true;
	if (distCentro == 3.5)
		cil = false;
	else
		cil = true;
	glPushMatrix();
		set_material(mat1);
		glTranslatef(0.0, 0.5, distCentro);
		set_material(mat1);
		brick.slopeV(largo, 1.0, 1.0, text, text2, cil);
		glTranslatef(largo/2+0.5, 0.0, 0.0);
		set_material(polishedCopper);
		brick.brick2_2_1(t_madera.GLindex, t_naranja.GLindex);
	glPopMatrix();
}
void techito(GLuint text, GLuint text2, materialStruct mat1, float x, float y) {
	glPushMatrix();
		glTranslatef(0.0, 0.1, 0.0);
		set_material(mat1);
		brick.flatV(x, y, text, text2);
	glPopMatrix();
}
void salidaT(GLuint text, GLuint text2, materialStruct mat1) {
	glPushMatrix();
		set_material(mat1);
		glTranslatef(0.0, -0.25, 5.0);
		brick.flatV(6.0, 2.0, text, text2);
	glPopMatrix();
}
void createFoco() {
	glPushMatrix();
		glPushMatrix();
			glTranslatef(5.1, 0.4, 0.0);
			glScalef(0.6, 0.6, 0.6);
			createFarolaKeyFrame(t_carbon.GLindex, t_amarillo.GLindex, 6, 5.0);
		glPopMatrix();
		glPushMatrix();
			glRotatef(180.0, 0.0, 1.0, 0.0);
			glTranslatef(5.1, 0.4, 0.0);
			glScalef(0.6, 0.6, 0.6);
			createFarolaKeyFrame(t_carbon.GLindex, t_amarillo.GLindex, 7, 5.0);
		glPopMatrix();
	glPopMatrix();
}
void createTecho() {
	glPushMatrix();
		/*
		techitos 4x4
		slope 3.5 6
		techo
		slope 2.5 4
		techo
		*/
		glTranslatef(0.0, 0.5, 0.0);
		salidaT(t_maderaO.GLindex, t_maderaO2.GLindex, madera);
		glPushMatrix();
			for (int i = 0; i < 4; i++) {
				salidaT(t_maderaO.GLindex, t_maderaO2.GLindex, madera);
				slopesTecho(t_maderaO.GLindex, t_maderaO2.GLindex, madera, 3.5, 6.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
			}
		glPopMatrix();
		glTranslatef(0.0, 1.0, 0.0);
		techito(t_madera.GLindex, t_naranja.GLindex, polishedCopper, 8.0, 8.0);
		glTranslatef(0.0, 0.2, 0.0);
		glPushMatrix();
			for (int i = 0; i < 4; i++) {
				slopesTecho(t_maderaO.GLindex, t_maderaO2.GLindex, madera, 2.5, 4.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
			}
		glPopMatrix();
		glTranslatef(0.0, 1.0, 0.0);
		techito(t_madera.GLindex, t_naranja.GLindex, polishedCopper, 4.0, 4.0);
	glPopMatrix();
	glPushMatrix();
		//glutSolidSphere(5.0, 10.0, 10.0);
		createFoco();
	glPopMatrix();
}
void createKiosko() {
	glPushMatrix();
		//Bases
	//glScalef(scaleX, scaleX, scaleX);
		glPushMatrix();
			set_material(perl);
			glTranslatef(-8.0, 0.0, 0.0);
			createBase(t_blanco.GLindex, polishedSilver);
			glTranslatef(0.0, 2.0, 0.0);
			createBase(t_madera.GLindex, polishedCopper);
			glTranslatef(0.0, 2.0, 0.0);
			createBase(t_maderaO.GLindex, madera);
			glTranslatef(0.0, 2.1, 0.0);
			set_material(perl);
			brick.flatV(10.0, 10.0, t_piso2.GLindex, t_piso.GLindex);
			glTranslatef(0.0, 0.1, 0.0);
			createBardas(t_madera.GLindex, t_maderaO.GLindex, polishedCopper, madera);
			createPilares(t_carbon.GLindex, perl);
			glTranslatef(0.0, 16.0, 0.0);
			createTecho();
		glPopMatrix();
		glPushMatrix();
			escalerasFull();
		glPopMatrix();
	glPopMatrix();
}
/*********** FUENTE	 ***********/
/*********** FACHADA ***********/
/*********** AVATAR1 ***********/



/*********** PRINCIPAL ***********/
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

		// Construcción Primaria
		glPushMatrix(); //Piso primario
			glTranslatef(0.0, 0.0, 0.0);
			glScalef(0.5, 0.5, 0.5);
			createPlancha();
			glTranslatef(0.0, 0.6, 0.0);
			createFences();
			createMesas();
			createBancas(t_carbon.GLindex, t_madera.GLindex, t_maderaO.GLindex);
			createArboles(t_pasto.GLindex, t_pasto2.GLindex);
			set_material(perl);
			brick.brick1_1_1(0.0, t_carbon.GLindex);
			createTiendas();
			createBanos();
			createFarolas();
			createKiosko();
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
		if (light == true) {
			if (dia == true) {
				rotSol += 1.0;
				printf("%f\n", rotSol);
				if (rotSol >= 500.0) {
					sol0 = false;
					foco1 = true;
					foco2 = true;
					foco3 = true;
					rotSol = 0.0;
					dia = false;
				}
				else {
					sol0 = true;
				}
			}
			else {
				rotSol += 1.0;
				printf("%f\n", rotSol);
				if (rotSol <= 500.0) {
					sol0 = false;
				}
				else {
					rotSol = 0.0;
					sol0 = true;
					dia = true;
					foco1 = false;
					foco2 = false;
					foco3 = false;
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
			traxFrame += KeyFrame[playIndex].traxFrameInc;
			trayFrame += KeyFrame[playIndex].trayFrameInc;
			trazFrame += KeyFrame[playIndex].trazFrameInc;
			rotLampX += KeyFrame[playIndex].rotLampXInc;
			rotLampY += KeyFrame[playIndex].rotLampYInc;
			rotLampZ += KeyFrame[playIndex].rotLampZInc;

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
		banderaModelos = !banderaModelos;
		break;
	case '4':	//
		banderaVentana = !banderaVentana;
		/*if (banderaVentana == false) {
			banderaVentana = true;
			if (rotVentana == 0)
				banderaUpDown = true;	//Va de 0 a 90
			else
				banderaUpDown = false;
		}*/
		//banderaVentana = !banderaVentana;
		break;
	case '5':
		
		break;
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
	case 'l':   //Activamos/desactivamos luz
	case 'L':
		light = !light;
		//dia = !dia;
		if (light == true) {
			sol0 = true;
			dia = true;
			rotSol = 0.0;
		}
		break;
	case '7':   //Activamos/desactivamos luz
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

	case 'V':
		LightAngle += 2.0;
		printf("Ang = %f\n", LightAngle);
		break;
	case 'v':
		LightAngle -= 2.0;
		printf("Ang = %f\n", LightAngle);
		break;

	case 'j':
		if (banderaVentana == true)
			//trax += 0.1;
			traxFrame += 0.1;
		else
			//trax += 1.0;
			traxFrame += 1.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'J':
		if (banderaVentana == true)
			//trax -= 0.1;
			traxFrame -= 1.0;
		else
			//trax -= 1.0;
			traxFrame -= 1.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'k':
		if (banderaVentana == true)
			//tray += 0.1;
			trayFrame += 0.1;
		else
			tray += 1.0;
			trayFrame += 1.0;
			printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'K':
		if (banderaVentana == true)
			//tray -= 0.1;
			trayFrame -= 0.1;
		else
			//tray -= 1.0;
			trayFrame -= 1.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'i':
		if (banderaVentana == true)
			//traz += 0.1;
			trazFrame += 0.1;
		else
			//traz += 1.0;
			trazFrame += 1.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'I':
		if (banderaVentana == true)
			//traz -= 0.1;
			trazFrame -= 0.1;
		else
			//traz -= 1.0;
			trazFrame -= 1.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'z':
		rotLampX += 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'Z':
		rotLampX -= 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'x':
		rotLampY += 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'X':
		rotLampY -= 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'c':
		rotLampZ += 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'C':
		rotLampZ -= 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'b':
		scaleZ += 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'B':
		scaleZ -= 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nrotLampX = %f\nrotLampY = %f\nrotLampZ = %f\n\n", traxFrame, trayFrame, trazFrame, rotLampX, rotLampY, rotLampZ);
		break;
	case 'n':
		scaleX += 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nscaleX = %f\nscaleY = %f\nscaleZ = %f\n\n", trax, tray, traz, scaleX, scaleY, scaleZ);
		break;
	case 'N':
		scaleX -= 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nscaleX = %f\nscaleY = %f\nscaleZ = %f\n\n", trax, tray, traz, scaleX, scaleY, scaleZ);
		break;
	case 'm':
		scaleY += 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nscaleX = %f\nscaleY = %f\nscaleZ = %f\n\n", trax, tray, traz, scaleX, scaleY, scaleZ);
		break;
	case 'M':
		scaleY -= 10.0;
		printf("traX = %f\ntraY = %f\ntraZ = %f\nscaleX = %f\nscaleY = %f\nscaleZ = %f\n\n", trax, tray, traz, scaleX, scaleY, scaleZ);
		break;
	/**********	KEYFRAMES	*********/
	case ' ':	//Salvar Frame
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}

		break;
	case '.':
		for (int i = 0; i < FrameIndex; i++) {
			printf("Frame [%d]\n", i);
			printf("traxFrame = %f  trayFrame = %f  trazFrame = %f  rotLampX = %f  rotLampY = %f  rotLampZ = %f\n", KeyFrame[i].traxFrame, KeyFrame[i].trayFrame, KeyFrame[i].trazFrame, KeyFrame[i].rotLampX, KeyFrame[i].rotLampY, KeyFrame[i].rotLampZ);

			printf("\nIncrementos:\n\n");
			printf("traxFrameInc = %f  trayFrameInc = %f  trazFrameInc = %f  rotLampXInc = %f  rotLampYInc = %f  rotLampZInc = %f\n", KeyFrame[i].traxFrameInc, KeyFrame[i].trayFrameInc, KeyFrame[i].trazFrameInc, KeyFrame[i].rotLampXInc, KeyFrame[i].rotLampYInc, KeyFrame[i].rotLampZInc);
			printf("******************************************\n\n");
		}
		break;
	/**********	KEYFRAMES	*********/
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

