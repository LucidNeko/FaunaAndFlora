#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "comp308.hpp"
#include "geometries.hpp"

using namespace std;
using namespace comp308;
extern GLuint g_texture;
Geometries::Geometries()
{
	initGeometries();
}

Geometries::~Geometries()
{
	delete m_box;
	delete m_bunny;
	delete m_sphere;
	delete m_table;
	delete m_teapot;
	delete m_torus;
}

void Geometries::initGeometries()
{
	m_box = new Geometry("work/res/assets/box.obj");
	m_bunny = new Geometry("work/res/assets/bunny.obj");
	m_sphere = new Geometry("work/res/assets/sphere.obj");
	m_table = new Geometry("work/res/assets/table.obj");
	m_teapot = new Geometry("work/res/assets/teapot.obj");
	m_torus = new Geometry("work/res/assets/torus.obj");

	// Initialise Textures
	initTextureWood("work/res/textures/wood.jpg");
	initTextureBrick("work/res/textures/brick.jpg");
	
}

void Geometries::renderGeometries()
{


	//
	// >>>>>>>>>>>>> BUNNY <<<<<<<<<<<<
	//
	glPushMatrix();

		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 
		glColor3f(1, 1, 1);
		glEnable(GL_COLOR_MATERIAL);
		GLfloat ambientb[] = { 0.19225, 0.19225, 0.19225, 1.0 };
		GLfloat diffuseb[] = { 0.50754, 0.50754, 0.50754};
		GLfloat specularb[] = {0.508273, 0.508273, 0.508273};
		GLfloat shininessb = 128.0f * 0.4f; // 20%  /* [0..128] */
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseb);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specularb);
		glMaterialf(GL_FRONT, GL_SHININESS, shininessb);

		glTranslatef(0.0f, -m_bunny->getLowestModelY(), 0.0f);
		m_bunny->renderGeometry();

	glPopMatrix();

	//
	// >>>>>>>>>>>>> SPHERE <<<<<<<<<<<<
	//
	glPushMatrix();

		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 
		glColor3f(0.80392156862f, 0.49803921568f, 0.19607843137f);
		glEnable(GL_COLOR_MATERIAL);
		GLfloat ambients[] = { 0.2125, 0.1275, 0.054, 1.0 };
		GLfloat diffuses[] = { 0.714, 0.4284, 0.18144};
		GLfloat speculars[] = {0.393548, 0.271906, 0.166721};
		GLfloat shininesss = 128.0f * 0.2f; // 20%  /* [0..128] */
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambients);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuses);
		glMaterialfv(GL_FRONT, GL_SPECULAR, speculars);
		glMaterialf(GL_FRONT, GL_SHININESS, shininesss);

		glTranslatef(-5.0f, -m_sphere->getLowestModelY(), 5.0f);
		m_sphere->renderGeometry();

	glPopMatrix();

	//
	// >>>>>>>>>>>>> TEAPOT <<<<<<<<<<<<
	//
	glPushMatrix();

		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 
		glColor3f(0.20392156862f, 0.49803921568f, 0.89607843137f);
		glEnable(GL_COLOR_MATERIAL);
		GLfloat ambienttp[] = { 0.19225, 0.19225, 0.19225, 1.0 };
		GLfloat diffusetp[] = { 0.50754, 0.50754, 0.50754};
		GLfloat speculartp[] = {0.508273, 0.508273, 0.508273};
		GLfloat shininesstp = 128.0f * 0.4f; // 20%  /* [0..128] */
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambienttp);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffusetp);
		glMaterialfv(GL_FRONT, GL_SPECULAR, speculartp);
		glMaterialf(GL_FRONT, GL_SHININESS, shininesstp);

		glTranslatef(-5.0f, -m_teapot->getLowestModelY(), -5.0f);
		m_teapot->renderGeometry();

	glPopMatrix();

	//
	// >>>>>>>>>>>>> TORUS <<<<<<<<<<<<
	//
	glPushMatrix();

		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 
		glColor3f(0.951f, 0.392f, 0.349f);
		glEnable(GL_COLOR_MATERIAL);
		GLfloat ambientt[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat diffuset[] = { 0.5, 0.0, 0.0};
		GLfloat speculart[] = {0.7, 0.6, 0.6};
		GLfloat shininesst = 128.0f * 0.25f; // 25%  /* [0..128] */
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambientt);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuset);
		glMaterialfv(GL_FRONT, GL_SPECULAR, speculart);
		glMaterialf(GL_FRONT, GL_SHININESS, shininesst);

		glTranslatef(5.0f, -m_torus->getLowestModelY(), 5.0f);
		m_torus->renderGeometry();

	glPopMatrix();

	//
	// >>>>>>>>>>>>> TABLE <<<<<<<<<<<<
	//
	glPushMatrix();
		// Set color for reflectivity
		glColor3f(1, 1, 1);
				// Enable Drawing texures
		glEnable(GL_TEXTURE_2D);
		// Use Texture as the color
		// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		// Set the location for binding the texture
		glActiveTexture(GL_TEXTURE0);
		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, m_woodTexture);

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTranslatef(0.0f, m_table->getLowestModelY(), 0.0f);
		m_table->renderGeometryUV();

	glPopMatrix();

	//
	// >>>>>>>>>>>>> BOX <<<<<<<<<<<<
	//
	glPushMatrix();
		// Set color for reflectivity
		glColor3f(1, 1, 1);

		// Enable Drawing texures
		glEnable(GL_TEXTURE_2D);
		// Use Texture as the color
		// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		// Set the location for binding the texture
		glActiveTexture(GL_TEXTURE0);
		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, m_brickTexture);

		glTranslatef(5.0f, -m_box->getLowestModelY(), -5.0f);
		m_box->renderGeometryUV();
	
	glPopMatrix();

}

void Geometries::initTextureBrick(string filename) {
	// image tex("work/res/textures/brick.jpg");
	image tex(filename);

	// cout << "brick: " << cout image._w << endl;

	glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glGenTextures(1, &m_brickTexture); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, m_brickTexture); // Bind it as a 2D texture
	
	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finnaly, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(), GL_UNSIGNED_BYTE, tex.dataPointer());

	// cout << tex.w << endl;
}

void Geometries::initTextureWood(string filename) {
	// image tex("work/res/textures/brick.jpg");
	image tex(filename);

	glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glGenTextures(1, &m_woodTexture); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, m_woodTexture); // Bind it as a 2D texture
	
	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finnaly, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(), GL_UNSIGNED_BYTE, tex.dataPointer());

	// cout << tex.w << endl;
}