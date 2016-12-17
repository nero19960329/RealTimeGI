#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "VertexVec.h"

class Object {
protected:
	GLuint vao, vbo;
	VertexVec vertexVec;

public:
	Object(): vao(0), vbo(0) {}
	virtual ~Object() {}

	virtual void init() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexVec.size() * sizeof(GLfloat), vertexVec.beginAddr(), GL_STATIC_DRAW);

		glBindVertexArray(vao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) (6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	virtual void draw() = 0;
};