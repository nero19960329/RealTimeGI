#include <iostream>

#include <gl/glew.h>
#include <gl/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MatReader.h"
#include "shader.hpp"
#include "Sphere.h"
#include "Quad.h"
#include "Mesh.h"
#include "ObjReader.h"

#include <sstream>

using namespace glm;
using namespace std;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void do_movement();

bool keys[1024];

const GLuint WIDTH = 1024, HEIGHT = 768;

vec3 cameraPos{ 0.0f, 0.0f, 1.4f };
vec3 cameraDir{ 0.0f, 0.0f, -1.0f };
vec3 cameraUp{ 0.0f, 1.0f, 0.0f };

GLfloat fov = 45.0f;
GLfloat ratio = 4.0 / 3.0;
GLfloat _yaw = -90.0f;
GLfloat _pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

GLboolean mouseLeftButtonPressed = GL_FALSE;
GLboolean mouseMiddleButtonPressed = GL_FALSE;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main() {
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_SAMPLES, 10);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "RealTime GI", nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) return -1;

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint boxProgram = LoadShaders("BoxVertexShader.vertexshader", "BoxFragmentShader.fragmentshader");
	GLuint lightProgram = LoadShaders("LightVertexShader.vertexshader", "LightFragmentShader.fragmentshader");

	vec3 vts[8] = {
		{ -0.5f, 0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ 0.5f, 0.5f, 0.5f },
		{ -0.5f, 0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, 0.5f, -0.5f }
	};

	Quad quads[5] = {
		Quad(vts[0], vts[1], vts[5], vts[4], vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)),
		Quad(vts[3], vts[2], vts[6], vts[7], vec3(-1.0f, 0.0, 0.0f), vec3(0.0f, 1.0f, 0.0f)),
		Quad(vts[4], vts[5], vts[6], vts[7], vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f)),
		Quad(vts[0], vts[4], vts[7], vts[3], vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f)),
		Quad(vts[1], vts[2], vts[6], vts[5], vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f))
	};
	rep(i, 5) quads[i].init();

	Quad light{ vec3(-0.12f, 0.499f, 0.12f), vec3(-0.12f, 0.499f, -0.12f), vec3(0.12f, 0.499f, -0.12f), vec3(0.12f, 0.499f, 0.12f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f) };
	light.init();

	Sphere sphere[2] = { 
		Sphere(vec3(0.2f, -0.3f, 0.0f), 0.2, vec3(1.0f, 0.0f, 1.0f), 100, 100),
		Sphere(vec3(-0.3f, -0.3f, -0.3f), 0.2, vec3(0.0f, 1.0f, 1.0f), 100, 100)
	};
	sphere[0].init();
	sphere[1].init();

	ObjReader objReader[2] = {
		ObjReader("../objs/teapot.obj", vec3{ -0.13, -0.3, -0.2 }, 0.4),
		ObjReader("../objs/bunny.obj", vec3{ 0.25, -0.35, 0.1 }, 0.3)
	};
	Mesh meshes[2] = {
		Mesh(objReader[0], vec3{ 1.0, 1.0, 1.0 }),
		Mesh(objReader[1], vec3{ 0.0, 0.6, 1.0 })
	};
	meshes[0].init();
	meshes[1].init();

	vector<float> nns;
	rep(i, 7) {
		ostringstream oss;
		oss << "../nn/direct_" << (i + 1) << ".mat";
		MatReader matReader(oss.str());
		auto directNN = matReader.readNN();
		nns.insert(nns.end(), directNN.begin(), directNN.end());
		oss.str("");
		oss << "../nn/indirect_" << (i + 1) << ".mat";
		matReader = MatReader(oss.str());
		auto indirectNN = matReader.readNN();
		nns.insert(nns.end(), indirectNN.begin(), indirectNN.end());
	}

	GLuint texID = 0;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_1D, texID);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, nns.size(), 0, GL_RED, GL_FLOAT, nns.data());

	glUniform1i(glGetUniformLocation(boxProgram, "texture"), 0);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLint lightColorLoc = glGetUniformLocation(boxProgram, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(boxProgram, "lightPos");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLoc, 0.0f, 0.499f, 0.0f);

	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		mouseLeftButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
		mouseMiddleButtonPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3);
		do_movement();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(boxProgram);
		GLint viewPosLoc = glGetUniformLocation(boxProgram, "viewPos");
		glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

		mat4 Projection = perspective(radians(fov), ratio, 0.1f, 100.0f);
		mat4 View = lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
		mat4 Model = mat4(1.0f);
		GLint modelLoc = glGetUniformLocation(boxProgram, "model");
		GLint viewLoc = glGetUniformLocation(boxProgram, "view");
		GLint projLoc = glGetUniformLocation(boxProgram, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(View));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(Projection));

		GLint objIDLoc = glGetUniformLocation(boxProgram, "ObjID");
		rep(i, 5) {
			glUniform1i(objIDLoc, i + 2);
			quads[i].draw();
		}

		glUniform1i(objIDLoc, 0);
		meshes[0].draw();
		//sphere[0].draw();
		glUniform1i(objIDLoc, 1);
		meshes[1].draw();
		//sphere[1].draw();

		glUseProgram(lightProgram);
		modelLoc = glGetUniformLocation(lightProgram, "model");
		viewLoc = glGetUniformLocation(lightProgram, "view");
		projLoc = glGetUniformLocation(lightProgram, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(View));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(Projection));

		light.draw();

		glFlush();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key <= 1024) {
		if (action == GLFW_PRESS) keys[key] = true;
		else if (action == GLFW_RELEASE) keys[key] = false;
	}
}

void do_movement() {
	GLfloat cameraSpeed = 0.6f * deltaTime;
	if (keys[GLFW_KEY_W]) cameraPos += cameraDir * cameraSpeed;
	if (keys[GLFW_KEY_S]) cameraPos -= cameraDir * cameraSpeed;
	if (keys[GLFW_KEY_A]) cameraPos -= normalize(cross(cameraDir, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D]) cameraPos += normalize(cross(cameraDir, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	if (mouseLeftButtonPressed == GL_TRUE) {
		GLfloat sensitivity = 0.05;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		_yaw += xoffset;
		_pitch += yoffset;

		if (_pitch > 89.0f) _pitch = 89.0f;
		if (_pitch < -89.0f) _pitch = -89.0f;

		vec3 front;
		front.x = cos(radians(_yaw)) * cos(radians(_pitch));
		front.y = sin(radians(_pitch));
		front.z = sin(radians(_yaw)) * cos(radians(_pitch));
		cameraDir = normalize(front);
	}

	if (mouseMiddleButtonPressed == GL_TRUE) {
		float deltaRatioW = 1.4 * xoffset / WIDTH;
		float deltaRatioH = 1.4 * yoffset / HEIGHT;
		cameraPos -= normalize(cross(cameraDir, cameraUp)) * deltaRatioW;
		cameraPos -= cameraUp * deltaRatioH;
	}
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
	if (mouseMiddleButtonPressed == GL_TRUE) return;
	if (fov >= 1.0f && fov <= 45.0f) fov -= yoffset;
	if (fov <= 1.0f) fov = 1.0f;
	if (fov >= 45.0f) fov = 45.0f;
}