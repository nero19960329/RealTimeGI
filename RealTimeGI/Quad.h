#pragma once

#include "Object.h"

#include <vector>

class Quad : public Object {
private:
	glm::vec3 a, b, c, d, normal, color;

public:
	Quad(const glm::vec3 &_a, const glm::vec3 &_b, const glm::vec3 &_c, const glm::vec3 &_d, const glm::vec3 &_normal, const glm::vec3 &_color);
	~Quad();

	void init() override;
	void draw() override;
};