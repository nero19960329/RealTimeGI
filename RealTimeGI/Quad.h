#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <vector>

class Quad {
private:
	GLuint vao, vbo;

	glm::vec3 a, b, c, d, normal, color;

public:
	Quad(const glm::vec3 &_a, const glm::vec3 &_b, const glm::vec3 &_c, const glm::vec3 &_d, const glm::vec3 &_normal, const glm::vec3 &_color);
	~Quad();

	void init();
	void draw();

private:
	void pushVec(std::vector<float> &vertices, const glm::vec3 &v, const glm::vec3 &n, const glm::vec3 &color);
};