//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

//Shader de colores para la casa
static const char* fShaderRojo = "shaders/shaderRojo.frag";
static const char* fShaderVerde = "shaders/shaderVerde.frag";
static const char* fShaderCafe = "shaders/shaderCafe.frag";
static const char* fShaderVerdeOscuro = "shaders/shaderVerdeOscuro.frag";
static const char* fShaderAzul = "shaders/shaderAzul.frag";

float angulo = 135.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,4,
		1,2,4,
		2,3,4,
		3,1,4,
		0,1,2,3
		
	};
	GLfloat vertices[] = {
		-0.5f, 0.0f,-0.5f,	//0
		0.5f,0.0f,-0.5f,	//1
		0.5f,0.0f,0.5f,	//2
		-0.5f,0.0f,0.5f,	//3
		0.0f,0.5f,0.0f//4
	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 15, 15);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B

	// Letra R
	// Barra vertical izquierda
	-0.9f,-0.9f,0.0f,                         0.58f,    0.15f,   0.82f,
	-0.8f,-0.9f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.9f,-0.5f,0.0f,                        0.58f,    0.15f,   0.82f,

	-0.8f,-0.9f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.8f,-0.5f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.9f,-0.5f,0.0f,                        0.58f,    0.15f,   0.82f,

	// Barra superior horizontal
	-0.8f,-0.5f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.6f,-0.5f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.8f,-0.6f,0.0f,                        0.58f,    0.15f,   0.82f,

	-0.6f,-0.5f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.6f,-0.6f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.8f,-0.6f,0.0f,                        0.58f,    0.15f,   0.82f,

	// Barra media horizontal
	-0.8f,-0.65f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.6f,-0.65f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.8f,-0.75f,0.0f,                        0.58f,    0.15f,   0.82f,

	-0.6f,-0.65f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.6f,-0.75f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.8f,-0.75f,0.0f,                        0.58f,    0.15f,   0.82f,

	// Lado derecho superior
	-0.6f,-0.5f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.5f,-0.5f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.6f,-0.75f,0.0f,                        0.58f,    0.15f,   0.82f,

	-0.5f,-0.5f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.5f,-0.75f,0.0f,                        0.58f,    0.15f,   0.82f,
	-0.6f,-0.75f,0.0f,                        0.58f,    0.15f,   0.82f,

	// Pierna diagonal inferior
	-0.7f, -0.75f, 0.0f,                     0.58f, 0.15f, 0.82f,
	-0.6f, -0.75f, 0.0f,                     0.58f, 0.15f, 0.82f,
	-0.6f, -0.9f,  0.0f,                     0.58f, 0.15f, 0.82f,
	// Triángulo 2
	-0.6f, -0.75f, 0.0f,                     0.58f, 0.15f, 0.82f,
	-0.5f, -0.9f,  0.0f,                     0.58f, 0.15f, 0.82f,
	-0.6f, -0.9f,  0.0f,                     0.58f, 0.15f, 0.82f,

		//Letra V
	// Lado izquierdo (barra inclinada)
	-0.2f,  0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,
	-0.1f,  0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,
	0.0f, -0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,

	-0.1f,  0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,
	0.1f, -0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,
	0.0f, -0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,

	// Lado derecho (barra inclinada)
	0.1f,  0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,
	0.2f,  0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,
	0.0f, -0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,

	0.2f,  0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,
	0.1f, -0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,
	0.0f, -0.2f, 0.0f,                       0.91f,     0.45f,    0.08f,

	//Letra T
	// Barra superior
	0.6f,0.9f,0.0f,                         0.10f,      0.75f,     0.68f,
	0.9f,0.9f,0.0f,                         0.10f,      0.75f,     0.68f,
	0.6f,0.8f,0.0f,                         0.10f,      0.75f,     0.68f,

	0.9f,0.9f,0.0f,                         0.10f,      0.75f,     0.68f,
	0.9f,0.8f,0.0f,                         0.10f,      0.75f,     0.68f,
	0.6f,0.8f,0.0f,                         0.10f,      0.75f,     0.68f,

	// Barra vertical
	0.70f,0.8f,0.0f,                         0.10f,      0.75f,     0.68f,
	0.8f,0.8f,0.0f,                         0.10f,      0.75f,     0.68f,
	0.70f,0.5f,0.0f,                         0.10f,      0.75f,     0.68f,

	0.8f,0.8f,0.0f,                         0.10f,      0.75f,     0.68f,
	0.8f,0.5f,0.0f,                         0.10f,      0.75f,     0.68f,
	0.7f,0.5f,0.0f,                         0.10f,      0.75f,     0.68f
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,324);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};
	GLfloat vertices_trianguloVerde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.13f,	0.37f,	0.13f,
		1.0f,	-1.0f,		0.5f,			0.13f,	0.37f,	0.13f,
		0.0f,	1.65f,		0.5f,			0.13f,	0.37f,	0.13f,

	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);
	MeshColor* trianguloVerde = new MeshColor();
	trianguloVerde->CreateMeshColor(vertices_trianguloVerde, 18);
	meshColorList.push_back(trianguloVerde);


	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};
	GLfloat vertices_cuadradoCafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067

	};
	GLfloat vertices_cuadradoRojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);
	MeshColor* cuadradoCafe = new MeshColor();
	cuadradoCafe->CreateMeshColor(vertices_cuadradoCafe, 36);
	meshColorList.push_back(cuadradoCafe);
	MeshColor* cuadradoRojo = new MeshColor();
	cuadradoRojo->CreateMeshColor(vertices_cuadradoRojo, 36);
	meshColorList.push_back(cuadradoRojo);


}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//shader de colores casa en 3d 
	Shader *shaderRojo = new Shader(); //2
	shaderRojo->CreateFromFiles(vShader, fShaderRojo);
	shaderList.push_back(*shaderRojo);

	Shader *shaderVerde = new Shader();//3
	shaderVerde->CreateFromFiles(vShader, fShaderVerde);
	shaderList.push_back(*shaderVerde);

	Shader *shaderCafe = new Shader();//4
	shaderCafe->CreateFromFiles(vShader, fShaderCafe);
	shaderList.push_back(*shaderCafe);

	Shader *shaderVerdeOscuro = new Shader();//5
	shaderVerdeOscuro->CreateFromFiles(vShader, fShaderVerdeOscuro);
	shaderList.push_back(*shaderVerdeOscuro);

	Shader *shaderAzul = new Shader();//6
	shaderAzul->CreateFromFiles(vShader, fShaderAzul);
	shaderList.push_back(*shaderAzul);
}


int main()
{
	mainWindow = Window(1000, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//
//	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		
		//Practica02 Parte1
		//LETRAS DE COLORES
/*
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();
*/
		//Parte 2 Practica02
			//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
	//	shaderList[0].useShader();
	//  uniformModel = shaderList[0].getModelLocation();
	//	uniformProjection = shaderList[0].getProjectLocation();
	//	angulo += 0.05;

		//techo
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.8f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//hojas arboles
		//izq
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.4f, 1.2f, 0.4f));
		model = glm::translate(model, glm::vec3(-1.95f, -0.5675f, -6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		//der
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.4f, 1.2f, 0.4f));
		model = glm::translate(model, glm::vec3(1.95f, -0.5675f, -6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		
		//cubo Rojo paredes_casa
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		model = glm::translate(model, glm::vec3(0.0f, -0.5, -5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		
		//ventanas
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		//izq
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, glm::vec3(-1.0f, -0.85, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//der
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, glm::vec3(1.0f, -0.85, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//puerta
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.3f));
		model = glm::translate(model, glm::vec3(0.0f, -2.2, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//troncos de arbol
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		//izq
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.3f));
		model = glm::translate(model, glm::vec3(-2.6f, -2.2, -6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		//der
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.3f));
		model = glm::translate(model, glm::vec3(2.6f, -2.2, -6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		/* //EJERCICIO2

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//triangulo azul techo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.55f, -4.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();
		//triangulo verde oscuro izq
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.55f, -1.65f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, -5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();
		//triangulo verde oscuro der
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.55f, -1.65f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, -5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();

		// Cuadrado verde (índice 3)  //Puerta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.8f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();
		//ventana izq
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.95f, 0.95f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();
		//ventana der
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.75f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.95f, 0.95f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();

		// Cuadrado café (índice 4) tronco izq
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.2f, -2.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[4]->RenderMeshColor();

		// Cuadrado café (índice 4) tronco der
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.2f, -2.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[4]->RenderMeshColor();

		// Cuadrado rojo (índice 5)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.8f, -4.0f));
		model = glm::scale(model, glm::vec3(3.f, 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[5]->RenderMeshColor();

*/
		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/