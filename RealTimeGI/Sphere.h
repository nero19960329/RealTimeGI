#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

class Sphere {
private:
	int lats, longs;
	GLuint vao, vbo;

	glm::vec3 center, color;
	double radius;
	
public:
	Sphere(const glm::vec3 &_center, double _radius, const glm::vec3 &_color, int _lats = 40, int _longs = 40);
	~Sphere();

	void init();
	void draw();
};