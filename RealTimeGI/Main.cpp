#include <iostream>

#include <gl/glew.h>
#include <gl/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MatReader.h"
#include "shader.hpp"
#include "Sphere.h"

using namespace glm;
using namespace std;

int main() {
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_SAMPLES, 4);
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

	static const GLfloat boxVertices[] = {
		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
	};
	GLuint boxVBO, boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &boxVBO);

	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

	glBindVertexArray(boxVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) (6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);


	static const GLfloat lightVertices[] = {
		-0.12f, 0.499f, 0.12f,
		-0.12f, 0.499f, -0.12f,
		0.12f, 0.499f, -0.12f,

		-0.12f, 0.499f, 0.12f,
		0.12f, 0.499f, 0.12f,
		0.12f, 0.499f, -0.12f,
	};
	GLuint lightVBO, lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);

	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

	glBindVertexArray(lightVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	Sphere sphere[2] = { 
		Sphere(vec3(0.2f, -0.3f, 0.0f), 0.2, vec3(1.0f, 0.0f, 1.0f), 60, 60),
		Sphere(vec3(-0.3f, -0.3f, -0.3f), 0.2, vec3(0.0f, 1.0f, 1.0f), 60, 60)
	};
	sphere[0].init();
	sphere[1].init();

	MatReader matReader("../nn/indirect_1.mat");
	auto directNN = matReader.readNN();
	matReader = MatReader("../nn/indirect_1.mat");
	auto indirectNN = matReader.readNN();

	//directNN.insert(directNN.end(), indirectNN.begin(), indirectNN.end());

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_1D, texture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, directNN.size(), 0, GL_RED, GL_FLOAT, directNN.data());

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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, texture);
		glUniform1i(glGetUniformLocation(boxProgram, "texture"), 0);

		/*glBindVertexArray(boxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 10 * 3);
		glBindVertexArray(0);*/

		sphere[0].draw();
		//sphere[1].draw();

		glUseProgram(lightProgram);
		modelLoc = glGetUniformLocation(lightProgram, "model");
		viewLoc = glGetUniformLocation(lightProgram, "view");
		projLoc = glGetUniformLocation(lightProgram, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(View));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(Projection));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 2 * 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	return 0;
}