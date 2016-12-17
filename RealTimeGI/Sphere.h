#pragma once

#include "Object.h"

class Sphere : public Object {
private:
	int lats, longs;

	glm::vec3 center, color;
	double radius;
	
public:
	Sphere(const glm::vec3 &_center, double _radius, const glm::vec3 &_color, int _lats = 40, int _longs = 40);
	~Sphere();

	void init() override;
	void draw() override;
};