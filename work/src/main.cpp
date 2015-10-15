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
#include <cstdlib>
#include <iostream>
#include <string>

#include "comp308.hpp"
#include "imageLoader.hpp"
#include "shaderLoader.hpp"
#include "geometries.hpp"
#include "lights.hpp"
#include "particleSystem.hpp"
#include "particleSystemBasic.hpp"
#include "particleSystemSwarm.hpp"
#include "particleSystemOBJ.hpp"
#include "OBJLoader.hpp"

using namespace std;
using namespace comp308;


// Global variables
// 
GLuint g_winWidth  = 640;
GLuint g_winHeight = 480;
GLuint g_mainWindow = 0;


// Projection values
// 
float g_fovy = 20.0;
float g_znear = 0.1;
float g_zfar = 1000.0;


// Mouse controlled Camera values
//
bool g_mouseDown = false;
vec2 g_mousePos;
float g_yRotation = 0;
float g_xRotation = 0;
float g_zoomFactor = 1.0;


// Scene information
//
GLuint g_texture = 0;
GLuint g_shader = 0;
bool g_useShader = false;

// Object to hold the geometries
Geometries *g_geometries = nullptr;

// Object to hold the lights
Lights *g_lights = nullptr;

// Particles
//
ParticleSystem *g_particleSystem = nullptr;
ParticleSystem *g_particleSystemOBJ = nullptr;
GLuint g_mrtShader = 0;
OBJLoader *g_objLoader = nullptr;


void initTexture() {
	image tex("work/res/textures/brick.jpg");

	glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glGenTextures(1, &g_texture); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, g_texture); // Bind it as a 2D texture
	
	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finnaly, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(), GL_UNSIGNED_BYTE, tex.dataPointer());

	cout << tex.w << endl;
}

void initShader() {
	g_shader = makeShaderProgram("work/res/shaders/shaderDemo.vert", "work/res/shaders/shaderDemo.frag");
	g_mrtShader = makeShaderProgram("work/res/shaders/mrtDemo.vert", "work/res/shaders/mrtDemo.frag");
}

// Sets up where the camera is in the scene
// Called every frame
// 
void setUpCamera() {
	// Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fovy, float(g_winWidth) / float(g_winHeight), g_znear, g_zfar);

	// Set up the view part of the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -50 * g_zoomFactor);
	glRotatef(g_xRotation, 1, 0, 0);
	glRotatef(g_yRotation, 0, 1, 0);
}

// Draw function
//
void draw() {

	//log errors
	GLenum err = GL_NO_ERROR;
	while((err = glGetError()) != GL_NO_ERROR)
	{
	  cout << "GL_ERROR: " << err << endl;
	}

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Enable flags for normal rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	setUpCamera();


	// g_objLoader->render();
	g_particleSystemOBJ->tick(1.f/60.f);
	g_particleSystemOBJ->render();

	// g_geometries->renderGeometries();

	// g_particleSystem->tick(1.f/60.f);
	// g_particleSystem->render();

	// Disable flags for cleanup (optional)
	glDisable(GL_TEXTURE_2D);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);

	

	g_particleSystem->tick(1.f/60.f);
	g_particleSystem->render();

	glDisable(GL_DEPTH_TEST);

	// Move the buffer we just drew to the front
	glutSwapBuffers();

	// Queue the next frame to be drawn straight away
	glutPostRedisplay();
}

// Reshape function
// 
void reshape(int w, int h) {
    if (h == 0) h = 1;

	g_winWidth = w;
	g_winHeight = h;
    
    // Sets the openGL rendering window to match the window size
    glViewport(0, 0, g_winWidth, g_winHeight);  
}

// Keyboard callback
// Called once per button state change
//
void keyboardCallback(unsigned char key, int x, int y) {
	cout << "Keyboard Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
}

// Special Keyboard callback
// Called once per button state change
//
void specialCallback(int key, int x, int y) {
	cout << "Special Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
}

// Mouse Button Callback function
// Called once per button state change
// 
void mouseCallback(int button, int state, int x, int y) {
	cout << "Mouse Callback :: button=" << button << ", state=" << state << ", x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
	switch(button){

		case 0: // left mouse button
			g_mouseDown = (state==0);
			g_mousePos = vec2(x, y);
			break;

		case 2: // right mouse button
			if (state==0)
				g_useShader = !g_useShader;
			break;

		case 3: // scroll foward/up
			g_zoomFactor /= 1.1;
			break;

		case 4: // scroll back/down
			g_zoomFactor *= 1.1;
			break;
	}
}

// Mouse Motion Callback function
// Called once per frame if the mouse has moved and
// at least one mouse button has an active state
// 
void mouseMotionCallback(int x, int y) {
	cout << "Mouse Motion Callback :: x,y=(" << x << "," << y << ")" << endl;
	// YOUR CODE GOES HERE
	// ...
	if (g_mouseDown) {
		vec2 dif = vec2(x,y) - g_mousePos;
		g_mousePos = vec2(x,y);
		g_yRotation += 0.3 * dif.x;
		g_xRotation += 0.3 * dif.y;
	}
}

//Main program
// 
int main(int argc, char **argv) {

	if(argc != 1){
		cout << "No arguments expected" << endl;
		exit(EXIT_FAILURE);
	}

	// Initialise GL, GLU and GLUT
	glutInit(&argc, argv);

	// Setting up the display
	// - RGB color model + Alpha Channel = GLUT_RGBA
	// - Double buffered = GLUT_DOUBLE
	// - Depth buffer = GLUT_DEPTH
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

	// Initialise window size and create window
	glutInitWindowSize(g_winWidth, g_winHeight);
	g_mainWindow = glutCreateWindow("COMP308 Assignment 3");


	// Initilise GLEW
	// must be done after creating GL context (glutCreateWindow in this case)
	GLenum err = glewInit();
	if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
		abort(); // Unrecoverable error
	}

	cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;



	// Register functions for callback
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);

	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotionCallback);

	// initLight();
	initShader();
	initTexture();

	glEnable(GL_NORMALIZE);

	g_geometries = new Geometries();
	g_lights = new Lights();
	g_particleSystem = new SwarmParticleSystem(200, 6);
	g_particleSystem->create();

	g_objLoader = new OBJLoader("work/res/assets/teapot.obj");
	g_particleSystemOBJ = new OBJParticleSystem(g_objLoader, 3500, 6);
	g_particleSystemOBJ->create();
	// Loop required by GLUT
	// This will not return until we tell GLUT to finish
	glutMainLoop();

	// Don't forget to delete all pointers that we made
	delete g_geometries;
	delete g_lights;
	return 0;
}