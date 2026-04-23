/*
Animación:
Sesión 1: Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Sesión 2: Compleja: Por medio de funciones y algoritmos. Textura Animada
*/

//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movCocheY;
float movCocheZ;
float movOffset;
float rotllanta;
float rotllantaOffset;
float rotcarro;
float rotcarroZ;
//nave
float movNaveX;
float movOffsetNaveX;
float movNaveY;
float movOffsetNaveY;
float movNaveZ;
float rotala;
float rotalaOffset;
float rotaspa;
float rotNave;

bool avanza;
bool avanzaNave;
bool subiendo;
bool aleteando;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;



Model Kitt_M;
Model Cofre_carro;
Model Llanta_M;
Model Pista_M;
Model Nave_M;
Model Ala_M;
Model Ala_Der;
Model Aspa_Der;
Model Aspa_Izq;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/carro.obj");
	Cofre_carro = Model();
	Cofre_carro.LoadModel("Models/cofre.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta.obj");
	Pista_M = Model();
	Pista_M.LoadModel("Models/pista.obj");
	Nave_M = Model();
	Nave_M.LoadModel("Models/nave.obj");
	Ala_M = Model();
	Ala_M.LoadModel("Models/ala.obj");
	Ala_Der = Model();
	Ala_Der.LoadModel("Models/alaDer.obj");
	Aspa_Der = Model();
	Aspa_Der.LoadModel("Models/aspaDer.obj");
	Aspa_Izq = Model();
	Aspa_Izq.LoadModel("Models/aspaIzq.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, -1.0f, 0.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz nave
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.001f, 0.001f,
		20.0f);
	spotLightCount++;
	//faro carro
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,  // color azul
		0.5f, 0.5f,
		-3.65f, 0.5f, 0.0f,// posicion inicial frente del carro
		-1.0f, 0.0f, 0.0f,  // direccion: hacia X negativo
		0.3f, 0.0f, 0.0f,  // atenuacion
		20.0f               // angulo del cono en grados
	);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.05f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	rotcarro = 0.0f;
	glm::vec3 direccionFaro = glm::vec3(-1.0f, 0.0f, 0.0f);
	avanza = 1;

	movNaveX= -180.0f;
	movOffsetNaveX = 0.1f;
	movNaveY = 0.0f;
	movOffsetNaveY = 0.05f;
	movNaveZ = 0.0f;
	rotala = 0.0f;
	rotalaOffset = 10.0f;
	rotNave = 0.0f;
	subiendo = 1.0f;
	avanzaNave = 1.0;
	aleteando = 1.0;

	glm::vec3 direcLuzNave = glm::vec3(0.707f, -0.707f, 0.0);
	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f*deltaTime;
		if (mainWindow.getReiniciar())
		{
			movCoche = 2.0f;
			movCocheZ = 0.0f;
			movCocheY = 0.0f;
			rotcarro = 0.0f;
			rotcarroZ = 0.0f;
			rotllanta = 0.0f;
			direccionFaro = glm::vec3(-1.0f, 0.0f, 0.0f);
			avanza = true;

			mainWindow.setReiniciar(false); // resetear la bandera
		}

		if (avanza)
		{
			if (movCoche > -180.0f)
			{
				movCoche -= movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;

				if (movCoche < -37.0f && movCoche > -120.0f)
				{
					movCocheY += 0.0025f * deltaTime; // sube poco a poco      
				}
				else
				{
					rotcarroZ = 0.0f; // regresa a horizontal
				}
				// ============ CURVA 1 ============

				if (movCoche < -25.0f && movCoche > -50.0f)
				{
					rotcarro = 23.0f;              // rota el coche en Y
					direccionFaro = glm::vec3(-1.0f, 0.0f, 0.23f);
					movCocheZ += 0.02f * deltaTime; // se mueve en Z
				}
				// ============ CURVA 2 ============
				if (movCoche < -50.0f && movCoche > -70.0f)
				{
					rotcarroZ = 9.0f;
					movCocheY += 0.008f * deltaTime;
					rotcarro = -37.0f;              // rota al lado contrario
					direccionFaro = glm::vec3(-1.0f, 0.09f, -0.37f);
					movCocheZ -= 0.055f * deltaTime; // regresa en Z
				}
				// ============ CURVA 3 ============
				if (movCoche < -70.0f && movCoche > -110.0f)
				{
					rotcarroZ = 25.0f;
					movCocheY += 0.01f * deltaTime;
					rotcarro = 17.0f;              // rota al lado contrario
					movCocheZ += 0.01f * deltaTime; // regresa en Z
					direccionFaro = glm::vec3(-1.0f, 0.25f, 0.17f);
				}
				if (movCoche < -80.0f && movCoche > -115.0f)
				{
					movCocheY += 0.013f * deltaTime;
				}
				if (movCoche < -115.0f && movCoche > -130.0f)
				{
					movCocheY += 0.015f * deltaTime;
				}
				// ============ CURVA 4 ============
				if (movCoche < -115.0f && movCoche > -180.0f)
				{
					rotcarro = -18.0f;              // rota al lado contrario
					direccionFaro = glm::vec3(-1.0f, 0.0f, -0.18f);
					movCocheZ -= 0.015f * deltaTime; // regresa en Z
				}
				//Bajada Final
				if (movCoche < -160.0f && movCoche > -180.0f)
				{
					movCocheY -= 0.007f * deltaTime;
					rotcarroZ = -10.0f;
					direccionFaro = glm::vec3(-1.0f, 0.0f, -0.10f);
				}
			}
			else
			{
				avanza = false; // detener al llegar al final
			}
		}
		if (avanzaNave)
		{
			if (aleteando)
			{
				rotaspa += rotalaOffset * deltaTime;//movimiento de las aspas
				rotala += rotalaOffset * deltaTime;
				if (rotala > 55.0f)
				{
					rotala = 55.0f; // Aseguramos que no pase del límite
					aleteando = false;
				}
			}
			else
			{
				rotala -= rotalaOffset * deltaTime;
				if (rotala <= -55.0f)
				{
					rotala = -55.0f; // Aseguramos que no pase del límite
					aleteando = true;
				}
			}
			// La nave comienza en -180.0f y termina en 0.0f
			if (movNaveX < 0.0f)
			{
				movNaveX += movOffsetNaveX * deltaTime; // Avanza sumando hacia el cero

				// Rotación base para mirar hacia el frente (+X)
				float baseRot = -180.0f;
				rotNave = baseRot;

				// ============ TRAMO INICIAL (-180 a -160) ============
				// Lo que para el coche era bajada final, para la nave es subida inicial
				if (movNaveX > -180.0f && movNaveX < -160.0f)
				{
					movNaveY += 0.008f * deltaTime;
				}

				// ============ CURVA 4 (-160 a -115) ============
				else if (movNaveX >= -160.0f && movNaveX < -115.0f)
				{
					rotNave = baseRot - 18.0f;
					movNaveZ += 0.015f * deltaTime;
				}

				// ============ CURVA 3 Y ALTURA (-115 a -70) ============
				if (movNaveX >= -115.0f && movNaveX < -70.0f)
				{

					movNaveY -= 0.02f * deltaTime; // Descenso inversamente proporcional
					rotNave = baseRot + 17.0f;
					movNaveZ -= 0.01f * deltaTime;
				}

				// ============ CURVA 2 (-70 a -50) ============
				else if (movNaveX >= -70.0f && movNaveX < -50.0f)
				{

					movNaveY -= 0.01f * deltaTime;
					rotNave = baseRot - 37.0f;
					movNaveZ += 0.055f * deltaTime;
				}

				// ============ CURVA 1 (-50 a -25) ============
				else if (movNaveX >= -50.0f && movNaveX < -25.0f)
				{
					rotNave = baseRot + 23.0f;
					movNaveZ -= 0.02f * deltaTime;
				}

				// ============ AJUSTE DE ALTURA FINAL (-37 a 0) ============
				if (movNaveX > -37.0f)
				{
					movNaveY -= 0.073f * deltaTime;

				}
			}
			else
			{
				// Al llegar a 0.0f, activamos el aterrizaje
				avanzaNave = false;
			}
		
			
		}
		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model= glm::mat4(1.0);
		modelaux= glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Pista
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.1f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pista_M.RenderModel();
		
		//Cuerpo prin
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche-2.0f, 0.60f+movCocheY, 4.0+movCocheZ));
		//faro
		glm::vec3 posicionFaro = glm::vec3(
			model[3][0] + (-3.0f),  // X: posicion del carro
			model[3][1] + (1.0f), // Y: altura del faro
			model[3][2] + (-1.0)          // Z: centro
		);
		
		glm::vec3 coloresFaro[] = {
		glm::vec3(1.0f, 0.0f, 0.0f),  // 0 - rojo
		glm::vec3(1.0f, 1.0f, 0.0f),  // 1 - amarillo
		glm::vec3(0.0f, 1.0f, 0.0f),  // 2 - verde
		glm::vec3(0.0f, 1.0f, 1.0f),  // 3 - cian
		glm::vec3(0.0f, 0.0f, 1.0f),  // 4 - azul
		glm::vec3(1.0f, 0.0f, 1.0f),  // 5 - magenta
		};

		glm::vec3 colorActual = coloresFaro[mainWindow.getColorFaro()];
		spotLights[2] = SpotLight(
			colorActual.r, colorActual.g, colorActual.b,
			0.5f, 0.5f,
			posicionFaro.x, posicionFaro.y, posicionFaro.z,
			direccionFaro.x, direccionFaro.y, direccionFaro.z,
			0.3f, 0.001f, 0.001f,
			20.0f
		);
		spotLights[2].SetFlash(posicionFaro, direccionFaro);

		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotcarro * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotcarroZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		modelaux2 = model;
		
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		// Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.70f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_carro.RenderModel();

		// llanta delantera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(3.65f, -1.6f, 1.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		// llanta delantera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(3.65f, -1.6f, -1.5f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		// llanta trasera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.5f, -1.6f, 1.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		// llanta trasera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.5f, -1.6f, -1.5f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();


		//Aquí va la nave con jerarquía de modelos, completar
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movNaveX-0.0f, movNaveY+37.0f, movNaveZ-10.0f));
		glm::vec3 luzNave = glm::vec3(
			model[3][0],  // X
			model[3][1],  // Y
			model[3][2]   // Z
		);
		luzNave.y -= 0.05f;// se coloca la salida de la luz debajo
		spotLights[1].SetFlash(luzNave, direcLuzNave);
		model = glm::rotate(model, rotNave * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nave_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.4f));
		model = glm::rotate(model, -rotala * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.4f));
		model = glm::rotate(model, rotala * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_Der.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.75f, -0.6f, -0.68f));
		model = glm::rotate(model, -rotaspa * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aspa_Der.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.75f, -0.6f, 0.68f));
		model = glm::rotate(model, -rotaspa * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aspa_Izq.RenderModel();
	

		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
