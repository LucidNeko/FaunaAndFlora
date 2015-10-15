#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "comp308.hpp"
#include "IRenderable.hpp"

struct obj_vertex {
	int p = 0;
	int t = 0;
	int n = 0;
};

struct obj_triangle {
	obj_vertex v[3];
};

class OBJLoader : public IRenderable {
private:
	std::string m_fname;
	
	float m_scale = 1.f;

	void readOBJ(std::string fname);
	void createNormals();

public:

	std::vector<comp308::vec3> m_points;
	std::vector<comp308::vec2> m_uvs;
	std::vector<comp308::vec3> m_normals;
	std::vector<obj_triangle> m_triangles;

	OBJLoader(std::string fname);

	void setScale(float scale) {
		m_scale = scale;
	};

	virtual void render();
};