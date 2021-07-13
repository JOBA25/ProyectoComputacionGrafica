#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 3.0f, 0.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// variables usadas para las animaciones de Keyframes
float rotPiernaIzqBmo = 0, rotPiernaDerBmo = 0, rotBrazoIzqBmo = 0, rotBrazoDerBmo = 0;

glm::vec3 PosIni(0.0f, 0.0f, 0.0f);
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z;
float rotPiernaIzqFinn = 0, rotPiernaDerFinn = 0;
float rot = 0, posXPiDe = -0.7, posZPiDe = 0;
float posXPiIz = 0.7, posZPiIz = 0;
bool animFinn = true;

//máximo de posiciones
#define MAX_FRAMES_BMO 4 
#define MAX_FRAMES_FINN 13


int i_max_steps = 190;
int i_curr_steps = 0;
int i_curr_steps_f = 0;


typedef struct _frame
{
	
	float rotPiernaIzqBmo;
	float rotIncPiernaIzqBmo;

	float rotPiernaDerBmo;
	float rotIncPiernaDerBmo;
	
	float rotBrazoIzqBmo;
	float rotIncBrazoIzqBmo;
	
	float rotBrazoDerBmo;
	float rotIncBrazoDerBmo;

}FRAME;

FRAME KeyFrameBMO[MAX_FRAMES_BMO];
//introducir datos
int FrameIndexBMO = 0;
bool playB = false;
int playIndex = 0;

typedef struct
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incZ;		//Variable para IncrementoZ

	float posXPiDe;      //al momento de dar vuelta deberemos afectar la posicion
	float posZPiDe;
	float posXIncPiDe;   //de las piernas
	float posZIncPiDe;

	float posZPiIz;
	float posXPiIz;
	float posZIncPiIz;
	float posXIncPiIz;

	float rot;
	float rotInc;
	float rotPiernaIzqFinn;
	float rotIncPiernaIzqFinn;
	float rotPiernaDerFinn;
	float rotIncPiernaDerFinn;
}FRAME_F;

FRAME_F KeyFrameFinn[MAX_FRAMES_FINN];
//introducir datos
int FrameIndexFinn = 0;
bool playF = false;
int playIndexF = 0;


// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(27.0f,15.0f,0.0f),
	glm::vec3(-30.0f,15.0f,54.0f),
	glm::vec3(-30.0f,15.0f,-25.0f),
	glm::vec3(27.0f,15.0f,-29.0f)
};

glm::vec3 LightP1;


//animación pato en bañera
float patoX = 45.0;
float patoZ = -34.0;
float patoRot = 0.0;

bool circuitoPato = false;
bool recorrido1Pato = true;
bool recorrido2Pato = false;

//animación Caracol
float caracolX = -29.0;
float caracolZ = 51.0;
float caracolRot = 0.0;

bool circuitoCaracol = false;
bool recorrido1Caracol = true;
bool recorrido2Caracol = false;
bool recorrido3Caracol = false;
bool recorrido4Caracol = false;

//animación hotcake
float hotcakeY = 6.85;
float hotcakeRot = 0;
bool circuitoHotcake = false;
bool recorrido1Hotcake = true;
bool recorrido2Hotcake = false;
bool recorrido3Hotcake = false;



//se salvan las posiciones de los personajes BMO y Finn
//se salva el tope de animaciones por personaje con sus respectivos
//FrameIndex
void saveFrame(void)
{
	KeyFrameBMO[0].rotPiernaIzqBmo = 0;
	KeyFrameBMO[1].rotPiernaIzqBmo = 55;
	KeyFrameBMO[2].rotPiernaIzqBmo = 26;

	KeyFrameBMO[0].rotPiernaDerBmo = 55;
	KeyFrameBMO[1].rotPiernaDerBmo = 0;
	KeyFrameBMO[2].rotPiernaDerBmo = 26;

	KeyFrameBMO[0].rotBrazoIzqBmo = 5;
	KeyFrameBMO[1].rotBrazoIzqBmo = 40;
	KeyFrameBMO[2].rotBrazoIzqBmo = 5;

	KeyFrameBMO[0].rotBrazoDerBmo = 30;
	KeyFrameBMO[1].rotBrazoDerBmo = -10;
	KeyFrameBMO[2].rotBrazoDerBmo = 30;

	FrameIndexBMO = 3;

	KeyFrameFinn[0].posX = -20;
	KeyFrameFinn[0].posZ = -27;
	KeyFrameFinn[0].rot = 0;
	KeyFrameFinn[0].rotPiernaDerFinn = 0;
	KeyFrameFinn[0].rotPiernaIzqFinn = 0;
	KeyFrameFinn[0].posXPiDe = -0.7;
	KeyFrameFinn[0].posZPiDe = 0;
	KeyFrameFinn[0].posXPiIz = 0.7;
	KeyFrameFinn[0].posZPiIz = 0;

	KeyFrameFinn[1].posX = -20;
	KeyFrameFinn[1].posZ = -18;
	KeyFrameFinn[1].rot = 0;
	KeyFrameFinn[1].rotPiernaDerFinn = 30;
	KeyFrameFinn[1].rotPiernaIzqFinn = -30;
	KeyFrameFinn[1].posXPiDe = -0.7;
	KeyFrameFinn[1].posZPiDe = 0;
	KeyFrameFinn[1].posXPiIz = 0.7;
	KeyFrameFinn[1].posZPiIz = 0;

	KeyFrameFinn[2].posX = -20;
	KeyFrameFinn[2].posZ = -9;
	KeyFrameFinn[2].rot = 0;
	KeyFrameFinn[2].rotPiernaDerFinn = 0;
	KeyFrameFinn[2].rotPiernaIzqFinn = 0;
	KeyFrameFinn[2].posXPiDe = -0.7;
	KeyFrameFinn[2].posZPiDe = 0;
	KeyFrameFinn[2].posXPiIz = 0.7;
	KeyFrameFinn[2].posZPiIz = 0;

	KeyFrameFinn[3].posX = -20;
	KeyFrameFinn[3].posZ = -9;
	KeyFrameFinn[3].rot = 90;
	KeyFrameFinn[3].rotPiernaDerFinn = 0;
	KeyFrameFinn[3].rotPiernaIzqFinn = 0;
	KeyFrameFinn[3].posXPiDe = 0.0;
	KeyFrameFinn[3].posZPiDe = 0.7;
	KeyFrameFinn[3].posXPiIz = 0.0;
	KeyFrameFinn[3].posZPiIz = -0.7;

	KeyFrameFinn[4].posX = -6.0;
	KeyFrameFinn[4].posZ = -9.0;
	KeyFrameFinn[4].rot = 90.0;
	KeyFrameFinn[4].rotPiernaDerFinn = -30.0;
	KeyFrameFinn[4].rotPiernaIzqFinn = 30.0;
	KeyFrameFinn[4].posXPiDe = 0.0;
	KeyFrameFinn[4].posZPiDe = 0.7;
	KeyFrameFinn[4].posXPiIz = 0.0;
	KeyFrameFinn[4].posZPiIz = -0.7;
	
	KeyFrameFinn[5].posX = 8.0;
	KeyFrameFinn[5].posZ = -9.0;
	KeyFrameFinn[5].rot = 90.0;
	KeyFrameFinn[5].rotPiernaDerFinn = 30.0;
	KeyFrameFinn[5].rotPiernaIzqFinn = -30.0;
	KeyFrameFinn[5].posXPiDe = 0.0;
	KeyFrameFinn[5].posZPiDe = 0.7;
	KeyFrameFinn[5].posXPiIz = 0.0;
	KeyFrameFinn[5].posZPiIz = -0.7;

	KeyFrameFinn[6].posX = 23.0;
	KeyFrameFinn[6].posZ = -9.0;
	KeyFrameFinn[6].rot = 90.0;
	KeyFrameFinn[6].rotPiernaDerFinn = 0.0;
	KeyFrameFinn[6].rotPiernaIzqFinn = 0.0;
	KeyFrameFinn[6].posXPiDe = 0.0;
	KeyFrameFinn[6].posZPiDe = 0.7;
	KeyFrameFinn[6].posXPiIz = 0.0;
	KeyFrameFinn[6].posZPiIz = -0.7;

	KeyFrameFinn[7].posX = 23.0;
	KeyFrameFinn[7].posZ = -9.0;
	KeyFrameFinn[7].rot = 180.0;
	KeyFrameFinn[7].rotPiernaDerFinn = 15.0;
	KeyFrameFinn[7].rotPiernaIzqFinn = -15.0;
	KeyFrameFinn[7].posXPiDe = 0.7;
	KeyFrameFinn[7].posZPiDe = 0.0;
	KeyFrameFinn[7].posXPiIz = -0.7;
	KeyFrameFinn[7].posZPiIz = 0.0;

	KeyFrameFinn[8].posX = 23.0;
	KeyFrameFinn[8].posZ = -25.0;
	KeyFrameFinn[8].rot = 180.0;
	KeyFrameFinn[8].rotPiernaDerFinn = 30.0;
	KeyFrameFinn[8].rotPiernaIzqFinn = -30.0;
	KeyFrameFinn[8].posXPiDe = 0.7;
	KeyFrameFinn[8].posZPiDe = 0.0;
	KeyFrameFinn[8].posXPiIz = -0.7;
	KeyFrameFinn[8].posZPiIz = 0.0;

	KeyFrameFinn[9].posX = 23.0;
	KeyFrameFinn[9].posZ = -25.0;
	KeyFrameFinn[9].rot = 180.0;
	KeyFrameFinn[9].rotPiernaDerFinn = 0.0;
	KeyFrameFinn[9].rotPiernaIzqFinn = 0.0;
	KeyFrameFinn[9].posXPiDe = 0.7;
	KeyFrameFinn[9].posZPiDe = 0.0;
	KeyFrameFinn[9].posXPiIz = -0.7;
	KeyFrameFinn[9].posZPiIz = 0.0;

	KeyFrameFinn[10].posX = 23.0;
	KeyFrameFinn[10].posZ = -25.0;
	KeyFrameFinn[10].rot = 90.0;
	KeyFrameFinn[10].rotPiernaDerFinn = -15.0;
	KeyFrameFinn[10].rotPiernaIzqFinn = 15.0;
	KeyFrameFinn[10].posXPiDe = 0.0;
	KeyFrameFinn[10].posZPiDe = 0.7;
	KeyFrameFinn[10].posXPiIz = 0.0;
	KeyFrameFinn[10].posZPiIz = -0.7;

	KeyFrameFinn[11].posX = 37.0;
	KeyFrameFinn[11].posZ = -25.0;
	KeyFrameFinn[11].rot = 90.0;
	KeyFrameFinn[11].rotPiernaDerFinn = 30.0;
	KeyFrameFinn[11].rotPiernaIzqFinn = -30.0;
	KeyFrameFinn[11].posXPiDe = 0.0;
	KeyFrameFinn[11].posZPiDe = 0.7;
	KeyFrameFinn[11].posXPiIz = 0.0;
	KeyFrameFinn[11].posZPiIz = -0.7;

	KeyFrameFinn[12].posX = 37.0;
	KeyFrameFinn[12].posZ = -25.0;
	KeyFrameFinn[12].rot = 90.0;
	KeyFrameFinn[12].rotPiernaDerFinn = 0.0;
	KeyFrameFinn[12].rotPiernaIzqFinn = 0.0;
	KeyFrameFinn[12].posXPiDe = 0.0;
	KeyFrameFinn[12].posZPiDe = 0.7;
	KeyFrameFinn[12].posXPiIz = 0.0;
	KeyFrameFinn[12].posZPiIz = -0.7;

	FrameIndexFinn = 13;
	
}

void resetElements(void)
{
	rotPiernaIzqBmo = KeyFrameBMO[0].rotPiernaIzqBmo;
	rotPiernaDerBmo = KeyFrameBMO[0].rotPiernaDerBmo;
	rotBrazoIzqBmo = KeyFrameBMO[0].rotBrazoIzqBmo;
	rotBrazoDerBmo = KeyFrameBMO[0].rotBrazoDerBmo;


}

void interpolation(void)
{

	
	KeyFrameBMO[playIndex].rotIncPiernaIzqBmo = (KeyFrameBMO[playIndex + 1].rotPiernaIzqBmo - KeyFrameBMO[playIndex].rotPiernaIzqBmo) / i_max_steps;
	KeyFrameBMO[playIndex].rotIncPiernaDerBmo = (KeyFrameBMO[playIndex + 1].rotPiernaDerBmo - KeyFrameBMO[playIndex].rotPiernaDerBmo) / i_max_steps; 
	KeyFrameBMO[playIndex].rotIncBrazoIzqBmo = (KeyFrameBMO[playIndex + 1].rotBrazoIzqBmo - KeyFrameBMO[playIndex].rotBrazoIzqBmo) / i_max_steps;
	KeyFrameBMO[playIndex].rotIncBrazoDerBmo = (KeyFrameBMO[playIndex + 1].rotBrazoDerBmo - KeyFrameBMO[playIndex].rotBrazoDerBmo) / i_max_steps;
}



void interpolacionF(void) {
	KeyFrameFinn[playIndexF].incX                = (KeyFrameFinn[playIndexF + 1].posX             - KeyFrameFinn[playIndexF].posX)             / i_max_steps;    
	KeyFrameFinn[playIndexF].incZ                = (KeyFrameFinn[playIndexF + 1].posZ             - KeyFrameFinn[playIndexF].posZ)             / i_max_steps;
	KeyFrameFinn[playIndexF].rotInc              = (KeyFrameFinn[playIndexF + 1].rot              - KeyFrameFinn[playIndexF].rot)              / i_max_steps;
	KeyFrameFinn[playIndexF].rotIncPiernaDerFinn = (KeyFrameFinn[playIndexF + 1].rotPiernaDerFinn - KeyFrameFinn[playIndexF].rotPiernaDerFinn) / i_max_steps;
	KeyFrameFinn[playIndexF].rotIncPiernaIzqFinn = (KeyFrameFinn[playIndexF + 1].rotPiernaIzqFinn - KeyFrameFinn[playIndexF].rotPiernaIzqFinn) / i_max_steps;
	KeyFrameFinn[playIndexF].posXIncPiDe         = (KeyFrameFinn[playIndexF + 1].posXPiDe         - KeyFrameFinn[playIndexF].posXPiDe)         / i_max_steps;
	KeyFrameFinn[playIndexF].posZIncPiDe         = (KeyFrameFinn[playIndexF + 1].posZPiDe         - KeyFrameFinn[playIndexF].posZPiDe)         / i_max_steps;
	KeyFrameFinn[playIndexF].posXIncPiIz         = (KeyFrameFinn[playIndexF + 1].posXPiIz         - KeyFrameFinn[playIndexF].posXPiIz)        / i_max_steps;
	KeyFrameFinn[playIndexF].posZIncPiIz         = (KeyFrameFinn[playIndexF + 1].posZPiIz         - KeyFrameFinn[playIndexF].posZPiIz)        / i_max_steps;
}


int main()
{
	// Init GLFW
	glfwInit(); //inciciacion de la ventana
	

	// Create a GLFWwindow object that we can use for GLFW's functions
	//Ininiciación de la ventana con ancho, alto, nombre de la ventana c
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final Barcenas Jorge", nullptr, nullptr);

	//se revisa si la existe algun error y en caso de que lo haya se manda un mensaje de error a la consola
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	//se manda la información al buffer para que pinte la ventana
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;


	// Initialize GLEW to setup the OpenGL Function pointers
	//se verifica si existe error con GLEW
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	//muebles
	Model Fachada((char*)"Models/fachada/fachada.obj");
	Model Hielera((char*)"Models/hielera/hielera.obj");
	Model LavaManos((char*)"Models/lavamanos/lavamanos.obj");
	Model Estufa((char*)"Models/estufa/estufa.obj");
	Model Sarten((char*)"Models/sarten/sarten.obj");
	Model Hotcake((char*)"Models/sarten/hotcake.obj");
	Model Escritorio((char*)"Models/escritorio/escritorio.obj");
	Model LavaPlatos((char*)"Models/lavaPlatos/lavaplatos.obj");
	Model SillonCafe((char*)"Models/sillonCafe/sillonCafe.obj");
	Model SillonRojo((char*)"Models/sillonRojo/sillonrojo.obj");
	Model Wc((char*)"Models/wc/wc.obj");
	Model TinaBaño((char*)"Models/tinaCortina/tina.obj");
	Model CortinaBaño((char*)"Models/tinaCortina/cortina.obj");
	//personajes
	Model Pato((char*)"Models/pato/pato.obj");

	Model Caracol((char*)"Models/caracol/caracol.obj");
	
	Model BmoCuerpo((char*)"Models/bmo/cuerpo.obj");
	Model BmoBrazoDerecho((char*)"Models/bmo/brazoDerecho.obj");
	Model BmoBrazoIzquierdo((char*)"Models/bmo/brazoIzquierdo.obj");
				//usaremos el mismo modelo ya que son iguales
	Model BmoPierna((char*)"Models/bmo/pierna.obj");

	Model FinnTorso((char*)"Models/finn/torsofinn.obj");
	Model FinnPierIzq((char*)"Models/finn/piernaizquierda.obj");
	Model FinnPierDer((char*)"Models/finn/piernaderecha.obj");


	// Build and compile our shader program

	//Inicialización de KeyFrames
	
	for(int i=0; i<MAX_FRAMES_BMO; i++)
	{
		
		KeyFrameBMO[i].rotPiernaIzqBmo = 0;
		KeyFrameBMO[i].rotIncPiernaIzqBmo = 0;
		KeyFrameBMO[i].rotPiernaDerBmo = 0;
		KeyFrameBMO[i].rotIncPiernaDerBmo = 0;
		KeyFrameBMO[i].rotBrazoIzqBmo = 0;
		KeyFrameBMO[i].rotIncBrazoIzqBmo = 0;
		KeyFrameBMO[i].rotBrazoDerBmo = 0;
		KeyFrameBMO[i].rotIncBrazoDerBmo = 0;
	}

	for (int i = 0; i < MAX_FRAMES_FINN; i++)
	{
		KeyFrameFinn[i].posX = 0;
		KeyFrameFinn[i].incX = 0;
		KeyFrameFinn[i].incZ = 0;
		KeyFrameFinn[i].rotPiernaIzqFinn = 0;
		KeyFrameFinn[i].rotIncPiernaIzqFinn = 0;
		KeyFrameFinn[i].rotPiernaDerFinn = 0;
		KeyFrameFinn[i].rotIncPiernaDerFinn = 0;
		KeyFrameFinn[i].rot = 0;
		KeyFrameFinn[i].rotInc = 0;
	}



	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	//vertices usados para la creación del cubo con el cual 
	//texturizaremos el ambiente por medio de skybox
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// arreglo de caras del cubo
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");
	
	//se manda a la biblioteca a texturizar y se carge en memoria
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.05f, 0.05f, 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.05f, 0.05f, 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.05f, 0.05f, 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.05f, 0.05f, 0.05f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp




		view = camera.GetViewMatrix();
		glm::mat4 model(1);
		tmp = model = glm::translate(model, glm::vec3(0, 0, 0));
		//model = glm::translate(model,glm::vec3(posX,posY,posZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Fachada.Draw(lightingShader);


		//Carga de modelo 
		//hielera

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(-39.0f, 0.0f, 18.0f));
		model = glm::rotate(model, glm::radians(-54.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Hielera.Draw(lightingShader);
		
		////escritorio
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(-32.0f, 0.0f, 45.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Escritorio.Draw(lightingShader);


		//Sillón rojo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-24.0f, 1.5f, -30.0f));
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(27.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillonRojo.Draw(lightingShader);


		//////Estufa
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(0.0f, 1.0f, 53.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Estufa.Draw(lightingShader);

		//sarten
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(2.22f, 6.8f, 50.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sarten.Draw(lightingShader);

		//hotcake
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(2.2f, hotcakeY, 50.5f));
		model = glm::rotate(model, glm::radians(hotcakeRot), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Hotcake.Draw(lightingShader);

		////Sillón café
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(26.0f, 1.8f, 20.0f));
		model = glm::rotate(model, glm::radians(190.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillonCafe.Draw(lightingShader);

		//////Lava Platos (tina con banco y barril con regadera)
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(19.0f, 1.0f, 47.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LavaPlatos.Draw(lightingShader);

		//muebles del segundo cuarto (baño)

		//Lava Manos
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(26.0f, -2.0f, -37.9f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LavaManos.Draw(lightingShader);

		//////WC
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(33.0f, 0.0f, -37.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Wc.Draw(lightingShader);

		//////Tina baño
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(45.0f, 0.5f, -29.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TinaBaño.Draw(lightingShader);

		////Cortina baño
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(45.0f, 0.5f, -29.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CortinaBaño.Draw(lightingShader);

		////
		////Pato de hule
		view = camera.GetViewMatrix();
		model = glm::mat4(1);								//-26
		model = glm::translate(tmp, glm::vec3(patoX, 2.6f, patoZ));
		model = glm::rotate(model, glm::radians(patoRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pato.Draw(lightingShader);

		//caracol
		view = camera.GetViewMatrix();
		model = glm::mat4(1);								
		model = glm::translate(tmp, glm::vec3(caracolX, 0.0f, caracolZ));
		model = glm::rotate(model, glm::radians(caracolRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Caracol.Draw(lightingShader);
		

		//////BMO 

		////cuerpo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		tmp = model = glm::translate(tmp, glm::vec3(-13.0f, 3.1f, -31.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BmoCuerpo.Draw(lightingShader);

		////brazo derecho
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-0.6f, 0.33f, -0.55f));
		model = glm::rotate(model, glm::radians(rotBrazoDerBmo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BmoBrazoDerecho.Draw(lightingShader);

		////brazo izquierdo
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(0.7f, 0.33f, -0.55f));
		model = glm::rotate(model, glm::radians(rotBrazoIzqBmo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BmoBrazoIzquierdo.Draw(lightingShader);

		////pierna derecha
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(-0.31f, 0.09f, -0.04f));
		model = glm::rotate(model, glm::radians(rotPiernaDerBmo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BmoPierna.Draw(lightingShader);

		////pierna izquierda
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(0.39f, 0.09f, -0.04f));
		model = glm::rotate(model, glm::radians(rotPiernaIzqBmo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BmoPierna.Draw(lightingShader);

		//finn
		tmp = glm::mat4(1.0f);
		//torso
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		tmp = model = glm::translate(model, glm::vec3(-20.0, 3.0f, -27.0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		FinnTorso.Draw(lightingShader);


		//pierna derecha
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(0.0f, 0.2f, 0.0f));
		model = glm::translate(model, glm::vec3(posX + posXPiDe, posY, posZ + posZPiDe));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPiernaDerFinn), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		FinnPierDer.Draw(lightingShader);

		////pierna izquierda
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(tmp, glm::vec3(0.0f, 0.2f, 0.0f));
		model = glm::translate(model, glm::vec3(posX + posXPiIz, posY, posZ + posZPiIz));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPiernaIzqFinn), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		FinnPierIzq.Draw(lightingShader);



		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// Swap the screen buffers
		glfwSwapBuffers(window);


		saveFrame();
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}


void animacion()
{
	//movimiento del pato
	if (circuitoPato) {
		if (recorrido1Pato) {
			patoZ += 0.05f;
			

			if (patoZ > -26 ) {
				recorrido1Pato = false;
				recorrido2Pato = true;
				patoRot = 180.0f;
			}
		}

		if (recorrido2Pato) {
			patoZ -= 0.05f;

			if (patoZ < -34) {
				recorrido2Pato = false;
				recorrido1Pato = true;
				patoRot = 0.0f;
			}
		}


	}

	//movimiento de caracol
	if (circuitoCaracol) {
		if (recorrido1Caracol) {
			caracolX += 0.05f;


			if (caracolX > 10) {
				recorrido1Caracol = false;
				recorrido2Caracol = true;
				caracolRot = 90.0f;
			}
		}

		if (recorrido2Caracol) {
			caracolZ -= 0.05f;

			if (caracolZ < -30) {
				recorrido2Caracol = false;
				recorrido3Caracol = true;
				caracolRot = 180.0f;
			}
		}

		if (recorrido3Caracol) {
			caracolX -= 0.05f;


			if (caracolX < -29) {
				recorrido3Caracol = false;
				recorrido4Caracol = true;
				caracolRot = 270.0f;
			}
		}

		if (recorrido4Caracol) {
			caracolZ += 0.05f;

			if (caracolZ > 51) {
				recorrido4Caracol = false;
				recorrido1Caracol = true;
				caracolRot = 0.0f;
			}
		}

	}

	//movimiento hotcake
	if (circuitoHotcake) {
		if (recorrido1Hotcake) {
			hotcakeY += 0.05f;


			if (hotcakeY > 9.0) {
				recorrido1Hotcake = false;
				recorrido2Hotcake = true;
			}
		}

		if (recorrido2Hotcake) {
			hotcakeRot += 1.0 ;

			if (hotcakeRot > 360) {
				recorrido2Hotcake = false;
				recorrido3Hotcake = true;
			}
		}

		if (recorrido3Hotcake) {
			hotcakeY -= 0.05f;

			if (hotcakeY < 6.85) {
				recorrido3Hotcake = false;
				recorrido1Hotcake = true;
				hotcakeRot = 0;
				circuitoHotcake = false;
			}
		}


	}

		//Movimiento del personaje BMO
	if (playB)
	{
		if (i_curr_steps >= i_max_steps) 
		{
			playIndex++;
			if (playIndex > MAX_FRAMES_BMO - 2)	
			{
				printf("termina anim\n");
				playIndex = 0;
				playB = false;
			}
			else 
			{
				i_curr_steps = 0; //Reset counter
									//Interpolation
				interpolation();
			}
		}
		else
		{
			rotPiernaIzqBmo += KeyFrameBMO[playIndex].rotIncPiernaIzqBmo;
			rotPiernaDerBmo += KeyFrameBMO[playIndex].rotIncPiernaDerBmo;
			rotBrazoDerBmo += KeyFrameBMO[playIndex].rotIncBrazoDerBmo;
			rotBrazoIzqBmo += KeyFrameBMO[playIndex].rotIncBrazoIzqBmo;

			i_curr_steps++;
		}

	}



	//Movimiento del personaje Finn
	if (playF and animFinn)
	{
		if (i_curr_steps_f >= i_max_steps)
		{
			playIndexF++;
			if (playIndexF > FrameIndexFinn - 2)
			{
				printf("termina anim\n");
				playIndexF = 0;
				playF = false;
				animFinn = false;
			}
			else
			{
				i_curr_steps_f = 0; //Reset counter
									//Interpolation
				interpolacionF();
			}
		}
		else
		{
			posX             += KeyFrameFinn[playIndexF].incX;
			posZ             += KeyFrameFinn[playIndexF].incZ;
			rot              += KeyFrameFinn[playIndexF].rotInc;

			rotPiernaIzqFinn += KeyFrameFinn[playIndexF].rotIncPiernaIzqFinn;
			rotPiernaDerFinn += KeyFrameFinn[playIndexF].rotIncPiernaDerFinn;
			
			posXPiDe         += KeyFrameFinn[playIndexF].posXIncPiDe;
			posZPiDe         += KeyFrameFinn[playIndexF].posZIncPiDe;

			posXPiIz         += KeyFrameFinn[playIndexF].posXIncPiIz;
			posZPiIz         += KeyFrameFinn[playIndexF].posZIncPiIz;


			i_curr_steps_f++;
		}

	}

}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_B])
	{
		
		
		if (playB == false && (FrameIndexBMO > 1))
		{
			
			resetElements();
			//First Interpolation				
			interpolation();

			playB = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			
			playB = false;
		}

	}

	if (keys[GLFW_KEY_F])
	{


		if (playF == false && (FrameIndexFinn > 1))
		{

			//resetElementsF();
			//First Interpolation				
			interpolacionF();

			playF = true;
			playIndexF = 0;
			i_curr_steps_f = 0;
		}
		else
		{
			
			playF = false;
		}

	}

	
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_P]) {
		circuitoPato = true;
	}

	if (keys[GLFW_KEY_C]) {
		circuitoCaracol = true;
	}

	
	if (keys[GLFW_KEY_H])
	{
		circuitoHotcake = true;
	}

	




	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}






}