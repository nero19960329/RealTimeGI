#pragma once

#include <array>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Utils.h"

class Mesh;

class ObjReader {
	friend class Mesh;

protected:
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::vec3> vertices;
	std::vector<std::array<int, 3>> triIndices;

	glm::vec3 center;
	real_t radius;

public:
	explicit ObjReader(const std::string &objFileName, const glm::vec3 &_center, real_t _radius);
	~ObjReader() {}

private:
	glm::vec3 readVec3(const std::string &str);
	std::array<int, 3> readTri(const std::string &str);

	void computeNormal();

	void fitSize();
	void moveToCenter();
	void scaleToRadius();
};