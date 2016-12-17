#include "Quad.h"
#include "VertexVec.h"

#include <glm/gtx/string_cast.hpp>

using namespace glm;
using namespace std;

Quad::Quad(const vec3 &_a, const vec3 &_b, const vec3 &_c, const vec3 &_d, const vec3 &_normal, const vec3 &_color) :
	Object(), a(_a), b(_b), c(_c), d(_d), normal(_normal), color(_color) {}

Quad::~Quad() {}

void Quad::init() {
	vertexVec.push_3vecs(a, normal, color);
	vertexVec.push_3vecs(b, normal, color);
	vertexVec.push_3vecs(c, normal, color);
	vertexVec.push_3vecs(c, normal, color);
	vertexVec.push_3vecs(d, normal, color);
	vertexVec.push_3vecs(a, normal, color);

	Object::init();
}

void Quad::draw() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 2 * 3);
	glBindVertexArray(0);
}