#pragma once

#include "Object.h"
#include "ObjReader.h"

#include <vector>

class Mesh : public Object {
private:
	const std::vector<glm::vec3> &vertexNormals;
	const std::vector<glm::vec3> &vertices;
	const std::vector<std::array<int, 3>> &triIndices;

	glm::vec3 color;

public:
	Mesh(const ObjReader &objReader, const glm::vec3 &_color);
	~Mesh() {}

	void init() override;
	void draw() override;
};