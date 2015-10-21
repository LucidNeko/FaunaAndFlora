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
extern vec3 lightPos;
extern vec3 volLightCol;
// volLightCol.x,volLightCol.y,volLightCol.z
Lights::Lights(){
	directionLight();
	ambientLight();
	pointLight();
	spotLight();
}

void Lights::updateLights()
{
	glPushMatrix();
		
		glRotatef(g_xRotation, 1, 0, 0);
		glRotatef(g_yRotation, 0, 1, 0);

		glPushMatrix();
			glTranslatef(lightPos.x, lightPos.y, lightPos.z);
			pointLight();
		glPopMatrix();

		directionLight();
		ambientLight();
		spotLight();

	glPopMatrix();
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
	GLfloat diffintensity[] = {0.465, 0.464, 0.45, 1.0f};

	glLightfv(GL_LIGHT2, GL_POSITION, direction);
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT1, GL_SPECULAR, diffintensity);

	glEnable(GL_LIGHT2);
}

void Lights::pointLight()
{
	// POINT LIGHT
	//
	GLfloat lig_diff[] = {volLightCol.x,volLightCol.y,volLightCol.z, 1.0};
	GLfloat lig_spec[] = {0.9445098039, 0.944901960784, 0.949411764705, 1.0};

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lig_diff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lig_spec);

	GLfloat light_pos[] = {-lightPos.x, lightPos.y, lightPos.z, 1.0};
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
	GLfloat lig_diff[] = {0.050, 0.050, 0.10, 1.0};
	GLfloat lig_spec[] = {0.050, 0.050, 0.10, 1.0};
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lig_diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lig_spec);

	GLfloat light_pos[] = {0.0, 50.0, -50.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	GLfloat spot_dir[] = {0.0, -1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_dir);

	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 5.0); 

	glEnable(GL_LIGHT0);
}
