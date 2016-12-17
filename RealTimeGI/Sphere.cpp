#include "Sphere.h"
#include "VertexVec.h"

#include <assert.h>
#include <vector>

#include <glm/gtx/string_cast.hpp>

using namespace glm;
using namespace std;

Sphere::Sphere(const vec3 &_center, double _radius, const vec3 &_color, int _lats, int _longs) :
	Object(), center(_center), radius(_radius), color(_color), lats(_lats), longs(_longs) {}

Sphere::~Sphere() {}

void Sphere::init() {
	for (int i = 0; i <= lats; ++i) {
		double lat0 = pi<double>() * (-0.5 + (i - 1) * 1.0 / lats);
		double z0 = sin(lat0), zr0 = cos(lat0);

		double lat1 = pi<double>() * (-0.5 + i * 1.0 / lats);
		double z1 = sin(lat1), zr1 = cos(lat1);

		for (int j = 0; j <= longs; ++j) {
			double lng = 2 * pi<double>() * (j - 1) * 1.0 / longs;
			double x = cos(lng), y = sin(lng);

			vertexVec.push_3vecs(
				vec3{ x * zr0 * radius + center[0], y * zr0 * radius + center[1], z0 * radius + center[2] },
				vec3{ x * zr0, y * zr0, z0 },
				color
			);

			vertexVec.push_3vecs(
				vec3{ x * zr1 * radius + center[0], y * zr1 * radius + center[1], z1 * radius + center[2] },
				vec3{ x * zr1, y * zr1, z1 },
				color
			);
		}
	}

	Object::init();
}

void Sphere::draw() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (lats + 1) * (longs + 1) * 2);
	glBindVertexArray(0);
}