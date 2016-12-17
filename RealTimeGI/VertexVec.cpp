#include "VertexVec.h"

using namespace glm;
using namespace std;

void VertexVec::push_3vecs(const vec3 &a, const vec3 &b, const vec3 &c) {
	vertices.push_back(a.x);
	vertices.push_back(a.y);
	vertices.push_back(a.z);
	vertices.push_back(b.x);
	vertices.push_back(b.y);
	vertices.push_back(b.z);
	vertices.push_back(c.x);
	vertices.push_back(c.y);
	vertices.push_back(c.z);
}