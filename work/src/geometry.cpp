//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "comp308.hpp"
#include "geometry.hpp"

using namespace std;
using namespace comp308;

Geometry::Geometry(string filename) {
	cout << "filename: " << filename << endl;
	m_filename = filename;
	readOBJ(filename);
}


void Geometry::readOBJ(string filename) {

	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_faceNormals.clear();
	m_triangles.clear();
	// m_faceCount.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0,0,0));
	m_uvs.push_back(vec2(0,0));
	m_normals.push_back(vec3(0,0,1));
	m_faceNormals.push_back(vec3(0,0,1));
	// m_faceCount.push_back(0);

	ifstream objFile(filename);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file " << filename << endl;

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
				if (vt.x > m_maxUVX) {m_maxUVX = vt.x;}
				if (vt.y > m_maxUVY) {m_maxUVY = vt.y;}
				m_uvs.push_back(vt);

			} else if(mode == "f") {
				
				vector<vertex> verts;
				while (objLine.good()){
					vertex v;

					if (m_uvs.size() <= 1 && !(m_normals.size() <= 1)) //If there is no uv data, ignore it
					{
					objLine >> v.p;		// Scan in position index
					objLine.ignore(1);	// Ignore the '/' character
					objLine.ignore(1);	// Ignore the '/' character
					objLine >> v.n;		// Scan in normal index
					}
					else if (m_uvs.size() <= 1 && m_normals.size() <= 1) 
					{
					objLine >> v.p;		// Scan in position index
					}


					else
					{
					// Assumes you have all of v/vt/vn for each vertex
					objLine >> v.p;		// Scan in position index
					objLine.ignore(1);	// Ignore the '/' character
					objLine >> v.t;		// Scan in uv (texture coord) index
					objLine.ignore(1);	// Ignore the '/' character
					objLine >> v.n;		// Scan in normal index
					}

					// check and set lowestModelY
					// cout << m_lowestModelY << endl;
					if(m_lowestModelY > m_points[v.p].y){
						m_lowestModelY = m_points[v.p].y;
					}
					verts.push_back(v);	
				}
				if(verts.size() == 3){
					
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);
				}
			}
		}
	}

	// Calculate normals if they are not calculated already
	if (m_normals.size() <= 1)
	{
		createNormals();
	}

	cout << "Reading OBJ file is DONE." << endl;
	cout << m_points.size()-1 << " points" << endl;
	cout << m_uvs.size()-1 << " uv coords" << endl;
	cout << m_normals.size()-1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;
	cout << m_lowestModelY << " is the lowest point" << endl;
	cout << m_maxUVY << " is maxuvy" << endl;
	cout << m_maxUVX << " is maxuvx" << endl;
}

//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to populate the normals for 
// the model currently loaded. Compute per face normals
// first and get that working before moving onto calculating
// per vertex normals.
//-------------------------------------------------------------
void Geometry::createNormals() {
	//"syntax" : "Packages/User/c++output.tmLanguage"
	cout << "create face normals reached" << endl;
	// YOUR CODE GOES HERE
	// ... 
	vec3 p1, p2, p3, U, V, faceNormal, normalizedFaceNormal;
	for(uint i = 0; i < m_triangles.size(); i++)
	{
		
		// The current triangle/face
		triangle tri = m_triangles[i];

		// Position and normal data.
		vertex v0 = tri.v[0];
		vertex v1 = tri.v[1];
		vertex v2 = tri.v[2];

		// Create the 3 vec3's
		p1 = vec3(m_points[v0.p].x, m_points[v0.p].y, m_points[v0.p].z);
		p2 = vec3(m_points[v1.p].x, m_points[v1.p].y, m_points[v1.p].z);
		p3 = vec3(m_points[v2.p].x, m_points[v2.p].y, m_points[v2.p].z);	

		// Use right hand rule to make U and V and use these vectors to make 
		// the surface normal
		U = p2 - p1;
		V = p3 - p1;

		// Calculate the face normal
		faceNormal = cross(U, V);

		//	Normalize face normal
		normalizedFaceNormal = normalize(faceNormal);

		// put normal vector into normal list
		m_faceNormals.push_back(normalizedFaceNormal);
		
		// Add the normals position to triange in the triangles array
		m_triangles[i].v[0].n = m_faceNormals.size()-1;
		m_triangles[i].v[1].n = m_faceNormals.size()-1;
		m_triangles[i].v[2].n = m_faceNormals.size()-1;
	}
	// cout << "face normals done" << endl;

	//initialise normals count and normals vertex list
	for(uint i = 1; i < m_points.size(); i++){
		
		m_normals.push_back(vec3(0.0f, 0.0f, 0.0f));
	}	
	//sum together the verticies and add a count
	for(uint i = 0; i < m_triangles.size(); i++)
	{
		
		// The current triangle/face
		triangle tri = m_triangles[i];

		//point the triangle to the correct position for the normal
		m_triangles[i].v[0].n = m_triangles[i].v[0].p;
		m_triangles[i].v[1].n = m_triangles[i].v[1].p;
		m_triangles[i].v[2].n = m_triangles[i].v[2].p;

		// Position and normal data.
		vertex v0 = tri.v[0];
		vertex v1 = tri.v[1];
		vertex v2 = tri.v[2]; 

		m_normals[v0.p]+=(m_faceNormals[v0.n]);
		m_normals[v1.p]+=(m_faceNormals[v1.n]);
		m_normals[v2.p]+=(m_faceNormals[v2.n]);
	}
	// normalise the vector normals
	for(uint i = 1; i < m_normals.size(); i++){
		m_normals[i] = normalize(m_normals[i]);
	}
}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as wireframe model
//-------------------------------------------------------------
void Geometry::createDisplayListPolyUV() {
	// cout << "in createDisplayListPoly()" << endl;
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);
	// cout << "in createDisplayListPoly() displaylist deleted" << endl;
	// Create a new list
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);
	// cout << "in createDisplayListPoly() displaylist created" << endl;
	
	glBegin(GL_TRIANGLES);
	// cout << "in createDisplayListPoly() beginning" << endl;
	// Loop through all triangles/faces and add them to the list to be drawn
	for(uint i = 0; i < m_triangles.size(); i++)
	{
		// cout << "in createDisplayListPoly() in loop iteration: " << i << endl;
		// The current triangle/face we are going to draw
		triangle tri = m_triangles[i];

		// Position and normal data.
		vertex v0 = tri.v[0];
		vertex v1 = tri.v[1];
		vertex v2 = tri.v[2];	

		// Using Gourad shading, add a normal for each vertex.
		glNormal3f(m_normals[v0.n].x, m_normals[v0.n].y, m_normals[v0.n].z);
		glTexCoord2f(m_uvs[v0.t].x * 5, m_uvs[v0.t].y * 5);
		glVertex3f(m_points[v0.p].x, m_points[v0.p].y, m_points[v0.p].z);

		glNormal3f(m_normals[v1.n].x, m_normals[v1.n].y, m_normals[v1.n].z);
		glTexCoord2f(m_uvs[v1.t].x * 5, m_uvs[v1.t].y * 5);
		glVertex3f(m_points[v1.p].x, m_points[v1.p].y, m_points[v1.p].z);

		glNormal3f(m_normals[v2.n].x, m_normals[v2.n].y, m_normals[v2.n].z);
		glTexCoord2f(m_uvs[v2.t].x * 5, m_uvs[v2.t].y * 5);
		glVertex3f(m_points[v2.p].x, m_points[v2.p].y, m_points[v2.p].z);
	}
	glEnd();
	glEndList();
}

void Geometry::createDisplayListPoly() {
	// cout << "in createDisplayListPoly()" << endl;
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);
	// cout << "in createDisplayListPoly() displaylist deleted" << endl;
	// Create a new list
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);
	// cout << "in createDisplayListPoly() displaylist created" << endl;
	
	glBegin(GL_TRIANGLES);
	// cout << "in createDisplayListPoly() beginning" << endl;
	// Loop through all triangles/faces and add them to the list to be drawn
	for(uint i = 0; i < m_triangles.size(); i++)
	{
		// cout << "in createDisplayListPoly() in loop iteration: " << i << endl;
		// The current triangle/face we are going to draw
		triangle tri = m_triangles[i];

		// Position and normal data.
		vertex v0 = tri.v[0];
		vertex v1 = tri.v[1];
		vertex v2 = tri.v[2];	

		// Using Gourad shading, add a normal for each vertex.
		glNormal3f(m_normals[v0.n].x, m_normals[v0.n].y, m_normals[v0.n].z);
		glVertex3f(m_points[v0.p].x, m_points[v0.p].y, m_points[v0.p].z);

		glNormal3f(m_normals[v1.n].x, m_normals[v1.n].y, m_normals[v1.n].z);
		glVertex3f(m_points[v1.p].x, m_points[v1.p].y, m_points[v1.p].z);

		glNormal3f(m_normals[v2.n].x, m_normals[v2.n].y, m_normals[v2.n].z);
		glVertex3f(m_points[v2.p].x, m_points[v2.p].y, m_points[v2.p].z);
	}
	glEnd();
	glEndList();
}


void Geometry::createDisplayListWire() {

	// Delete old list if there is one
	if (m_displayListWire) glDeleteLists(m_displayListWire, 1);

	// Create a new list
	m_displayListWire = glGenLists(1);
	glNewList(m_displayListWire, GL_COMPILE);

	glBegin(GL_LINES);

	// Loop through all triangles/faces and add them to the list to be drawn
	for(uint i = 1; i < m_triangles.size(); i++){

		// The current triangle/face we are going to draw
		triangle tri = m_triangles[i];

		// Position and normal data.
		vertex v0 = tri.v[0];
		vertex v1 = tri.v[1];
		vertex v2 = tri.v[2];

		// Add each vertex. 
		// First and last points are the same to close the triangle.
		glVertex3f(m_points[v0.p].x, m_points[v0.p].y, m_points[v0.p].z);
		glVertex3f(m_points[v1.p].x, m_points[v1.p].y, m_points[v1.p].z);
		glVertex3f(m_points[v1.p].x, m_points[v1.p].y, m_points[v1.p].z);
		glVertex3f(m_points[v2.p].x, m_points[v2.p].y, m_points[v2.p].z);
		glVertex3f(m_points[v2.p].x, m_points[v2.p].y, m_points[v2.p].z);
		glVertex3f(m_points[v0.p].x, m_points[v0.p].y, m_points[v0.p].z);
	}
	glEnd();
	glEndList();
	// cout << "Finished creating Wire Geometry" << endl;
}

void Geometry::renderGeometry() {
	// cout << "in renderGeometry()" << endl;
	if (m_wireFrameOn) {
		// cout << "in renderGeometry() m_wireFrameOn" << endl;
		// glShadeModel(GL_SMOOTH);
		createDisplayListWire();
		glCallList(m_displayListWire);

	} else {
		// cout << "in renderGeometry() !m_wireFrameOn" << endl;
		// wireframe off
		
		createDisplayListPoly();
		glCallList(m_displayListPoly);
	}
	// cout << "out renderGeometry()" << endl;	
}

void Geometry::renderGeometryUV() {
		// cout << "in renderGeometry() !m_wireFrameOn" << endl;
		// wireframe off
		// glShadeModel(GL_SMOOTH);
		createDisplayListPolyUV();
		glCallList(m_displayListPoly);
	
	
}

void Geometry::toggleWireFrame() {
	m_wireFrameOn = !m_wireFrameOn;
}

GLfloat Geometry::getLowestModelY() 
{
	return m_lowestModelY;
}