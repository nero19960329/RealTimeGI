#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <vector>

class VertexVec {
private:
	std::vector<GLfloat> vertices;

public:
	VertexVec() {}
	~VertexVec() {}

	void push_3vecs(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c);
	std::vector<GLfloat>::size_type size() { return vertices.size(); }
	GLfloat *beginAddr() { return &vertices[0]; }
};