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

#include <sstream>

using namespace glm;
using namespace std;

int main() {
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_SAMPLES, 10);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(1024, 768, "RealTime GI", nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) return -1;

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint boxProgram = LoadShaders("BoxVertexShader.vertexshader", "BoxFragmentShader.fragmentshader");
	GLuint lightProgram = LoadShaders("LightVertexShader.vertexshader", "LightFragmentShader.fragmentshader");

	mat4 Projection = perspective(radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	mat4 View = lookAt(vec3(0.0, 0.0, 1.4), vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 Model = mat4(1.0f);

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
		Sphere(vec3(0.2f, -0.3f, 0.0f), 0.2, vec3(1.0f, 0.0f, 1.0f), 60, 60),
		Sphere(vec3(-0.3f, -0.3f, -0.3f), 0.2, vec3(0.0f, 1.0f, 1.0f), 60, 60)
	};
	sphere[0].init();
	sphere[1].init();

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

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(boxProgram);
		GLint lightColorLoc = glGetUniformLocation(boxProgram, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(boxProgram, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(boxProgram, "viewPos");
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, 0.0f, 0.499f, 0.0f);
		glUniform3f(viewPosLoc, 0.0f, 0.0f, 1.4f);

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
		sphere[0].draw();
		glUniform1i(objIDLoc, 1);
		sphere[1].draw();

		glUseProgram(lightProgram);
		modelLoc = glGetUniformLocation(lightProgram, "model");
		viewLoc = glGetUniformLocation(lightProgram, "view");
		projLoc = glGetUniformLocation(lightProgram, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(View));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(Projection));

		light.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	return 0;
}