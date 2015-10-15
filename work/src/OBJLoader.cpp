#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "comp308.hpp"
#include "OBJLoader.hpp"

using namespace std;
using namespace comp308;

OBJLoader::OBJLoader(std::string fname) {
	m_fname = fname;
	readOBJ(fname);
}

void OBJLoader::readOBJ(std::string fname) {
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0,0,0));
	m_uvs.push_back(vec2(0,0));
	m_normals.push_back(vec3(0,0,1));

	ifstream objFile(fname);

	if(!objFile.is_open()) {
		cerr << "Error reading " << fname << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file " << fname << endl;
	
	// good() means that failbit, badbit and eofbit are all not set
	while(objFile.good()) {

		// Pull out line from file
		string line;
		std::getline(objFile, line);
		istringstream objLine(line);

		// Pull out mode from line
		string mode;
		objLine >> mode;

		// Reading like this means whitespace at the start of the line is fine
		// attempting to read from an empty string/line will set the failbit
		if (!objLine.fail()) {

			if (mode == "v") {
				vec3 v;
				objLine >> v.x >> v.y >> v.z;
				m_points.push_back(v);

			} else if(mode == "vn") {
				vec3 vn;
				objLine >> vn.x >> vn.y >> vn.z;
				m_normals.push_back(vn);

			} else if(mode == "vt") {
				vec2 vt;
				objLine >> vt.x >> vt.y;
				m_uvs.push_back(vt);

			} else if(mode == "f") {

				vector<obj_vertex> verts;
				while (objLine.good()){
					obj_vertex v;

					objLine >> v.p;
					if(objLine.peek() == int('/')) {
						objLine.ignore(1);
						if(objLine.peek() == int('/')) {
							objLine.ignore(1);
						} else {
							objLine >> v.t;
							objLine.ignore(1);
						}
						objLine >> v.n;
					}

					verts.push_back(v);
				}

				// IFF we have 3 verticies, construct a triangle
				if(verts.size() == 3){
					obj_triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);
				}
			}
		}
	}

	//clear normals so we can calculate
	// m_normals.clear();
	// m_normals.push_back(vec3(0,0,1));

	// If we didn't have any normals, create them
	if (m_normals.size() <= 1) createNormals();

	cout << "Reading OBJ file is DONE." << endl;
	cout << m_points.size()-1 << " points" << endl;
	cout << m_uvs.size()-1 << " uv coords" << endl;
	cout << m_normals.size()-1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;
}

void OBJLoader::createNormals() {
	cout << "building face normals";

	std::vector<vec3> faceNormals;

	for(uint i = 0; i < m_triangles.size(); i++) {
		obj_triangle t = m_triangles[i];

		vec3 a = m_points[t.v[0].p];
		vec3 b = m_points[t.v[1].p];
		vec3 c = m_points[t.v[2].p];

		vec3 n = cross(b-a, c-a);

		if(n.x != 0 || n.y != 0 || n.z != 0) {
			n = normalize(n);
		}

		uint n_idx = faceNormals.size();
		faceNormals.push_back(n);

		m_triangles[i].v[0].n = n_idx;
		m_triangles[i].v[1].n = n_idx;
		m_triangles[i].v[2].n = n_idx;

		//flat shading
		// m_triangles[i].v[0].n = t.v[0].p;
		// m_triangles[i].v[1].n = t.v[1].p;
		// m_triangles[i].v[2].n = t.v[2].p;

		// m_normals[t.v[0].p] = n;
		// m_normals[t.v[1].p] = n;
		// m_normals[t.v[2].p] = n;
	}

	cout << "filling temp normals" << endl; 

	while(m_normals.size() < m_points.size()) {
		m_normals.push_back(vec3(0,0,0));
	}

	cout << "calculating per vertex normals" << endl;

	for(uint i = 0; i < m_triangles.size(); i++) {
		obj_triangle t = m_triangles[i];

		m_normals[t.v[0].p] += faceNormals[t.v[0].n];
		m_normals[t.v[1].p] += faceNormals[t.v[1].n];
		m_normals[t.v[2].p] += faceNormals[t.v[2].n];

		m_triangles[i].v[0].n = t.v[0].p;
		m_triangles[i].v[1].n = t.v[1].p;
		m_triangles[i].v[2].n = t.v[2].p;
	}

	cout << "normalizing normals" << endl;

	for(uint i = 0; i < m_normals.size(); i++) {
		m_normals[i] = normalize(m_normals[i]);
	}
}

void OBJLoader::render() {
	glBegin(GL_TRIANGLES);
	for(uint i = 0; i < m_triangles.size(); i++) {
		obj_triangle t = m_triangles[i];

		//Get the 3 points of this triangle
		vec3 a = m_points[t.v[0].p];
		vec3 b = m_points[t.v[1].p];
		vec3 c = m_points[t.v[2].p];

		//normals
		vec3 an = m_normals[t.v[0].n];
		vec3 bn = m_normals[t.v[1].n];
		vec3 cn = m_normals[t.v[2].n];

		//uv
		vec2 auv = m_uvs[t.v[0].t]*5;
		vec2 buv = m_uvs[t.v[1].t]*5;
		vec2 cuv = m_uvs[t.v[2].t]*5;

		glNormal3f(an.x, an.y, an.z);
		glTexCoord2f(auv.x, auv.y);
		glVertex3f(a.x, a.y, a.z);

		glNormal3f(bn.x, bn.y, bn.z);
		glTexCoord2f(buv.x, buv.y);
		glVertex3f(b.x, b.y, b.z);
		

		glNormal3f(cn.x, cn.y, cn.z);
		glTexCoord2f(cuv.x, cuv.y);
		glVertex3f(c.x, c.y, c.z);
		
	}
	glEnd();
}