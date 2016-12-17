#include "ObjReader.h"
#include "Utils.h"

#include <fstream>
#include <sstream>

using namespace glm;
using namespace std;

ObjReader::ObjReader(const string &objFileName, const vec3 &_center, real_t _radius) :
	center(_center), radius(_radius) {
	ifstream fin(objFileName);
	if (!fin) error_exit("Obj file not found!\n");

	while (!fin.eof()) {
		string line;
		getline(fin, line);

		if (!line.size() || line[0] == '#') continue;

		if (line.find("vn") == 0) vertexNormals.push_back(readVec3(line));
		else if (line.find("v") == 0) vertices.push_back(readVec3(line));
		else if (line.find("f") == 0) triIndices.push_back(readTri(line));
	}

	fin.close();

	fitSize();
	if (!vertexNormals.size()) computeNormal();
	else for (auto &normal : vertexNormals) normal /= length(normal);
}

void ObjReader::computeNormal() {
	vector<int> vertexTris(vertices.size(), 0);		// vertex has how many adjacent triangles
	vertexNormals = vector<vec3>(vertices.size(), vec3{ 0.0, 0.0, 0.0 });

	rep(i, triIndices.size()) {
		array<int, 3> idx = triIndices[i];
		const vec3 &a = vertices[idx[0]], &b = vertices[idx[1]], &c = vertices[idx[2]];
		vec3 normal = normalize(cross(b - a, c - b));
		rep(k, 3) {
			++vertexTris[idx[k]];
			vertexNormals[idx[k]] += normal;
		}
	}

	rep(i, vertices.size()) vertexNormals[i] /= vertexTris[i];
}

vec3 ObjReader::readVec3(const string &str) {
	istringstream iss(str);
	string tmp;
	real_t x, y, z;
	iss >> tmp >> x >> y >> z;
	return{ x, y, z };
}

array<int, 3> ObjReader::readTri(const string &str) {
	istringstream iss(str);
	string tmp;
	iss >> tmp;
	
	int cnt = 0;
	for (const auto &ch : str) {
		if (ch == '/') ++cnt;
	}

	if (cnt == 0) {
		int idx[3];
		iss >> idx[0] >> idx[1] >> idx[2];
		array<int, 3> res = { idx[0] - 1, idx[1] - 1, idx[2] - 1 };
		return res;
	} else if (cnt == 3) {
		error_exit("Not supported type!\n");
	} else if (cnt == 6) {
		int idx[3], tmpInt;
		char tmpCh;
		rep(k, 3) {
			iss >> idx[k] >> tmpCh;
			int nowPos = iss.tellg();
			if (str[nowPos] == tmpCh) iss >> tmpCh;
			else iss >> tmpInt >> tmpCh;
			iss >> tmpInt;
		}
		array<int, 3> res = { idx[0] - 1, idx[1] - 1, idx[2] - 1 };
		return res;
	} else {
		error_exit("Not supported type!\n");
	}
}

void ObjReader::fitSize() {
	moveToCenter();
	scaleToRadius();
}

void ObjReader::moveToCenter() {
	vec3 meshCenter{ 0.0, 0.0, 0.0 };
	for (const auto &v : vertices) meshCenter += v;
	meshCenter /= vertices.size();
	for (auto &v : vertices) v = v - meshCenter + center;
}

void ObjReader::scaleToRadius() {
	float maxDist = 0.0f;
	for (const auto &v : vertices) updateMax(maxDist, length(v - center));
	for (auto &v : vertices) v = float(radius / maxDist) * (v - center) + center;
}