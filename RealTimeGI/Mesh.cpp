#include "Mesh.h"
#include "VertexVec.h"

using namespace glm;

Mesh::Mesh(const ObjReader &objReader, const vec3 &_color) :
	Object(), vertexNormals(objReader.vertexNormals), vertices(objReader.vertices), triIndices(objReader.triIndices), color(_color) {}

void Mesh::init() {
	for (const auto &triIdx : triIndices) {
		const vec3 &a = vertices[triIdx[0]], &b = vertices[triIdx[1]], &c = vertices[triIdx[2]];
		rep(k, 3) vertexVec.push_3vecs(vertices[triIdx[k]], vertexNormals[triIdx[k]], color);
	}

	Object::init();
}

void Mesh::draw() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexVec.size());
	glBindVertexArray(0);
}