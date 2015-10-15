#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "comp308.hpp"
#include "lights.hpp"

using namespace std;
using namespace comp308;

extern GLfloat g_zoomFactor;
extern GLfloat g_xRotation;
extern GLfloat g_yRotation;

Lights::Lights(){

	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();
	directionLight();
	ambientLight();
	// Set up the view part of the model view matrix
	// glPushMatrix();
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();

	// glTranslatef(0, 0, -50 * g_zoomFactor);
	// glRotatef(g_xRotation, 1, 0, 0);
	// glRotatef(g_yRotation, 0, 1, 0);
	pointLight();
	spotLight();
	// glPopMatrix();
}

void Lights::ambientLight()
{
	GLfloat ambient[] = {0.62f, 0.611f, 0.607f, 1.0f};
	glLightfv(GL_LIGHT3, GL_AMBIENT,  ambient);	
	glEnable(GL_LIGHT3);
}

void Lights::directionLight()
{
	GLfloat direction[]	    = {1.0f, 0.50f, 0.0f, 0.0f};
	GLfloat diffintensity[] = {0.85098039215, 0.539215686274, 0.534901960784, 1.0f};

	glLightfv(GL_LIGHT2, GL_POSITION, direction);
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT1, GL_SPECULAR, diffintensity);

	glEnable(GL_LIGHT2);
}

void Lights::pointLight()
{
	// POINT LIGHT
	//
	GLfloat lig_diff[] = {0.545098039, 0.94901960784, 0.69411764705, 1.0};
	GLfloat lig_spec[] = {0.345098039, 0.94901960784, 0.49411764705, 1.0};

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lig_diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lig_spec);

	GLfloat light_pos[] = {-6.0, 2.0, -50.0, 1.0};
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos);

	GLfloat spot_dir[] = {0.0, -1.0, 0.0};
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0); 

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.4);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.2);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

	glEnable(GL_LIGHT1);
}

void Lights::spotLight()
{
	// SPOT LIGHT
	//
	GLfloat lig_diff[] = {0.50, 0.50, 1.0, 1.0};
	GLfloat lig_spec[] = {0.50, 0.50, 1.0, 1.0};
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lig_diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lig_spec);

	GLfloat light_pos[] = {0.0, 50.0, -50.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	GLfloat spot_dir[] = {0.0, -1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_dir);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 5.0); 

	glEnable(GL_LIGHT0);
}