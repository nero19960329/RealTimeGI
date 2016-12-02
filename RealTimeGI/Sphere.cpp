#include "Sphere.h"

#include <assert.h>
#include <vector>

#include <glm/gtx/string_cast.hpp>

using namespace glm;
using namespace std;

Sphere::Sphere(const vec3 &_center, double _radius, const vec3 &_color, int _lats, int _longs) :
	vao(0), vbo(0), center(_center), radius(_radius), color(_color), lats(_lats), longs(_longs) {}

Sphere::~Sphere() {}

void Sphere::init() {
	vector<GLfloat> vertices;

	int cnt = 0;
	for (int i = 0; i <= lats; ++i) {
		double lat0 = pi<double>() * (-0.5 + (i - 1) * 1.0 / lats);
		double z0 = sin(lat0), zr0 = cos(lat0);

		double lat1 = pi<double>() * (-0.5 + i * 1.0 / lats);
		double z1 = sin(lat1), zr1 = cos(lat1);

		for (int j = 0; j <= longs; ++j) {
			double lng = 2 * pi<double>() * (j - 1) * 1.0 / longs;
			double x = cos(lng), y = sin(lng);

			vertices.push_back(x * zr0 * radius + center[0]);
			vertices.push_back(y * zr0 * radius + center[1]);
			vertices.push_back(z0 * radius + center[2]);

			vertices.push_back(x * zr0);
			vertices.push_back(y * zr0);
			vertices.push_back(z0);

			vertices.push_back(color[0]);
			vertices.push_back(color[1]);
			vertices.push_back(color[2]);


			vertices.push_back(x * zr1 * radius + center[0]);
			vertices.push_back(y * zr1 * radius + center[1]);
			vertices.push_back(z1 * radius + center[2]);

			vertices.push_back(x * zr1);
			vertices.push_back(y * zr1);
			vertices.push_back(z1);

			vertices.push_back(color[0]);
			vertices.push_back(color[1]);
			vertices.push_back(color[2]);
		}
	}

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

void Sphere::draw() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (lats + 1) * (longs + 1) * 2);
	glBindVertexArray(0);
}