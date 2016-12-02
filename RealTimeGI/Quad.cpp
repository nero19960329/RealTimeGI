#include "Quad.h"

#include <glm/gtx/string_cast.hpp>

using namespace glm;
using namespace std;

Quad::Quad(const vec3 &_a, const vec3 &_b, const vec3 &_c, const vec3 &_d, const vec3 &_normal, const vec3 &_color) :
	a(_a), b(_b), c(_c), d(_d), normal(_normal), color(_color) {}

Quad::~Quad() {}

void Quad::init() {
	vector<GLfloat> vertices;

	pushVec(vertices, a, normal, color);
	pushVec(vertices, b, normal, color);
	pushVec(vertices, c, normal, color);
	pushVec(vertices, c, normal, color);
	pushVec(vertices, d, normal, color);
	pushVec(vertices, a, normal, color);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) (6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void Quad::draw() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 2 * 3);
	glBindVertexArray(0);
}

void Quad::pushVec(vector<GLfloat> &vertices, const vec3 &v, const vec3 &n, const vec3 &color) {
	vertices.push_back(v.x);
	vertices.push_back(v.y);
	vertices.push_back(v.z);

	vertices.push_back(n.x);
	vertices.push_back(n.y);
	vertices.push_back(n.z);

	vertices.push_back(color.x);
	vertices.push_back(color.y);
	vertices.push_back(color.z);
}