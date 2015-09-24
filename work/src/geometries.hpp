#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "comp308.hpp"
#include "geometry.hpp"
#include "imageLoader.hpp"

class Geometries{

	Geometry *m_box 	= nullptr;
	Geometry *m_bunny	= nullptr;	
	Geometry *m_sphere 	= nullptr;
	Geometry *m_table 	= nullptr;
	Geometry *m_teapot 	= nullptr;
	Geometry *m_torus 	= nullptr;

private:
	// Scene information
	//
	GLuint m_brickTexture = 0;
	GLuint m_woodTexture = 0;

	void initGeometries();		// This will load up all the objects with data, ready for the drawing
	void initTextureBrick(std::string);
	void initTextureWood(std::string);
	
public:

	Geometries();
	~Geometries();
	void renderGeometries();

};