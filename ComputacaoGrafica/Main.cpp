#include<GLFW/glfw3.h>
#include<Windows.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<vector>
#include <GL/freeglut.h>
#include <iostream>
using namespace std;

#include"ObjLoader.h"
#include "materiais.h"

// Inicializar 
void init(GLFWwindow* window);
void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void reSize(int w, int h);
void esfera(unsigned& id);
// Fun��es


// Vari�veis
static float angulo = 0;
static unsigned blenderModelId, esferaId, esfera2Id, esfera3Id;
static bool mostraLuz = true;
static bool mostraLuz2 = true;
static bool mostraLuz3 = true;

vec3 objPos(0.f, 0.f, -25.f);
vec3 lightPos(5.f, 0.f, 10.f);
vec3 lightPos1(-5.f, 0.f, -15.f);
vec3 lightPos2(0.f, 10.f, 10.f);


void Desenha(float dt) {

	glLoadIdentity();

	vec3 position(0.f, 0.f, 0.f);
	vec3 direction(0.f, 0.f, -3.f);
	vec3 up(0.f, 1.f, 0.f);
	vec3 look = position + direction;// direction;

	gluLookAt(
		position.x, position.y, position.z,
		look.x, look.y, look.z,
		up.x, up.y, up.z);


	float veloc_ang = 25.f * dt;
	angulo += veloc_ang;

	copper.ativar();
	glPushMatrix();
	glTranslatef(objPos.x, objPos.y, objPos.z);
	glRotatef(angulo, 1.f, 0.f, 0.f);
	glRotatef(-angulo, 0.f, 1.f, 0.f);
	glRotatef(-angulo, 0.f, 0.f, 1.f);
	glCallList(blenderModelId);
	glPopMatrix();

	luz.ativar();
	glPushMatrix();
	if (mostraLuz) {
		glTranslatef(lightPos.x, lightPos.y, lightPos.z);
		glCallList(esferaId);
	}
	if (mostraLuz) {
		glEnable(GL_LIGHT0);
		float lightposition[] = { lightPos.x, lightPos.y, lightPos.z,1.f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
	}
	else {
		glDisable(GL_LIGHT0);
	}
	glPopMatrix();

	glPushMatrix();
	if (mostraLuz2) {
		glTranslatef(lightPos1.x, lightPos1.y, lightPos1.z);
		glCallList(esfera2Id);
	}

	if (mostraLuz2) {
		glEnable(GL_LIGHT1);
		float lightposition1[] = { lightPos1.x, lightPos1.y, lightPos1.z,1.f };
		glLightfv(GL_LIGHT1, GL_POSITION, lightposition1);
	}
	else {
		glDisable(GL_LIGHT1);
	}
	glPopMatrix();

	glPushMatrix();
	if (mostraLuz3) {
		glTranslatef(lightPos2.x, lightPos2.y, lightPos2.z);
		glCallList(esfera3Id);
	}

	if (mostraLuz3) {
		glEnable(GL_LIGHT2);
		float lightposition2[] = { lightPos2.x, lightPos2.y, lightPos2.z,1.f };
		glLightfv(GL_LIGHT2, GL_POSITION, lightposition2);
	}
	else {
		glDisable(GL_LIGHT2);
	}
	glPopMatrix();
	


	if (angulo >= 360.0)
		angulo = 0.0;
}

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Tutorial Light - Blender", NULL, NULL);

	init(window);

	float lastTime = 0.0;

	bool running = true;
	while (running)
	{
		float currentTime = (float)glfwGetTime();
		float dt = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		reSize(width, height);

		// -- Draw Objects --
		Desenha(dt);

		glfwSwapBuffers(window);
		running = !glfwWindowShouldClose(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}


void init(GLFWwindow* window) {
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, teclado_callback);

	glClearColor(0.f, 0.f, 0.f, 1.f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);


	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	float globalAmb[] = { 0.1f, 0.1f, 0.1f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	float light0[3][4] = {
				{0.1f, 0.1f, 0.1f, 1.f}, // ambient
				{0.8f, 0.8f, 0.8f, 1.f}, // diffuse
				{ 1.f,  1.f,  1.f, 1.f } // specular
	};

	glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);

	glLightfv(GL_LIGHT1, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT1, GL_SPECULAR, &light0[2][0]);

	glLightfv(GL_LIGHT2, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT2, GL_SPECULAR, &light0[2][0]);

	ObjLoader::loadOBJ(blenderModelId, "Blender/head_fixed.obj");
	esfera(esferaId);
	esfera(esfera2Id);
	esfera(esfera3Id);
}


void esfera(unsigned& id)
{
	vector<vector<vec3> > pontos;
	vector<vector<vec3> > normals;

	float raio = 0.2f;
	unsigned nStacks = 10;
	unsigned nSectors = 10;
	const float PI = 3.14159265359;
	const float TWO_PI = 2 * PI;
	const float HALF_PI = PI / 2.0;

	float deltaPhi = PI / nStacks;
	float deltaTheta = TWO_PI / nSectors;


	for (unsigned i = 0; i <= nStacks; i++)
	{
		float phi = -HALF_PI + i * deltaPhi;
		float temp = raio * cos(phi);
		float y = raio * sin(phi);

		vector<vec3> pts;

		for (unsigned j = 0; j <= nSectors; j++)
		{
			float theta = j * deltaTheta;
			float x = temp * sin(theta);
			float z = temp * cos(theta);

			float s = theta / TWO_PI;
			GLfloat t = phi / PI + 0.5;

			pts.push_back(vec3(x, y, z));
		}// next j

		pontos.push_back(pts);
		normals.push_back(pts);
	}// next i

	id = glGenLists(1);
	glNewList(id, GL_COMPILE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	for (unsigned i = 0; i < nStacks; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);

		for (unsigned j = 0; j <= nSectors; j++)
		{
			glNormal3fv(&normals[i][j].x); glVertex3fv(&pontos[i][j].x);
			glNormal3fv(&normals[i + 1][j].x); glVertex3fv(&pontos[i + 1][j].x);
		}
		glEnd();
	}
	glDisable(GL_CULL_FACE);
	glEndList();
}



void reSize(int w, int h)
{
	glViewport(0, 0, w, h);

	float aspect = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
}


void teclado_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		mostraLuz = !mostraLuz;
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		mostraLuz2 = !mostraLuz2;
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		mostraLuz3 = !mostraLuz3;
	}
}
