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
#include "particleSystemRope.hpp"
#include "OBJLoader.hpp"
#include "lsystemtree.hpp"

using namespace std;
using namespace comp308;

// Global variables
GLuint g_winWidth  = 1280;
GLuint g_winHeight = 720;
GLuint g_mainWindow = 0;

// Projection values
float g_fovy = 20.0;
float g_znear = 0.1;
float g_zfar = 1000.0;

// Mouse controlled Camera values
bool g_mouseDown = false;
vec2 g_mousePos;
float g_yRotation = 0;
float g_xRotation = 0;
float g_zoomFactor = 1.0;

// Scene information
GLuint g_texture = 0;
GLuint g_beeTexture = 0;
bool g_useShader = false;
GLuint g_volumetricShader = 0;
GLuint g_materialShader = 0;
GLuint g_occlusionShader = 0;
GLuint g_texShader = 0;
GLuint g_twoTexShader = 0;
GLuint g_blurShader = 0;
GLuint g_fongShader = 0;
GLuint g_texFongShader = 0;
float *g_lightParticle = nullptr;
vec3 lightPos = vec3(10.0,05.5,-30);
vec3 lightDir = vec3(0.0f, -1.0f, 0.0f);
vec3 cameraPosition = vec3(0.0f,0.0f,0.0f);

vec3 volLightCol = vec3(1.0f,1.0f,1.0f);
// Object to hold the geometries
Geometries *g_geometries = nullptr;

// Object to hold the lights
Lights *g_lights = nullptr;

unsigned int FBO0;
unsigned int renderTexture0,depthTexture0, shadowMap0;
unsigned int FBO1;
unsigned int renderTexture1,depthTexture1, shadowMap1;
unsigned int FBO2;
unsigned int renderTexture2,depthTexture2, shadowMap2, renderTexture2_1;
unsigned int FBO3;
unsigned int renderTexture3,depthTexture3;

// Particles
ParticleSystem *g_particleSystem = nullptr;
ParticleSystem *g_particleSystemOBJ = nullptr;
ParticleSystem *g_particleSystemRope = nullptr;
GLuint g_mrtShader = 0;
OBJLoader *g_objLoader = nullptr;
LSystem *tree = new LSystem("P:I+[P+F]--//[--L]I[++L]-[PF]++PF "
                           "I:FS[//&&L][//^^L]FS "
                           "S:SFS "
                           "L:[`{+f-ff-f+|+f-ff-f}] "
                           "F:[&&&p`/W////W////W////W////W] "
                           "p:FF "
                           "W:[`^F][{&&&&-f+f|-f+f}]",
                   "P",18.0,"0:2 16:3 13:4");
//   ▄█  ███▄▄▄▄    ▄█      ███     
//  ███  ███▀▀▀██▄ ███  ▀█████████▄ 
//  ███▌ ███   ███ ███▌    ▀███▀▀██ 
//  ███▌ ███   ███ ███▌     ███   ▀ 
//  ███▌ ███   ███ ███▌     ███     
//  ███  ███   ███ ███      ███     
//  ███  ███   ███ ███      ███     
//  █▀    ▀█   █▀  █▀      ▄████▀   
//                                  
void initLight() {
	float direction[]	  = {lightDir.x, lightDir.y, lightDir.z, 1.0f};
	float position[]	  = {lightPos.x,lightPos.y,lightPos.z,1.0f};
	float diffintensity[] = {0.7f, 0.7f, 0.7f, 1.0f};
	float ambient[]       = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);	
	glEnable(GL_LIGHT0);
}
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

	image texBee("work/res/assets/bee/bee.png");
	glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glGenTextures(1, &g_beeTexture); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, g_beeTexture); // Bind it as a 2D texture
	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Finnaly, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texBee.w, texBee.h, texBee.glFormat(), GL_UNSIGNED_BYTE, texBee.dataPointer());
}
void initShader() {
	g_volumetricShader = makeShaderProgram("work/res/shaders/volumetricShader.vert", "work/res/shaders/volumetricShader.frag");
	g_materialShader = makeShaderProgram("work/res/shaders/materialShader.vert", "work/res/shaders/materialShader.frag");
	g_occlusionShader = makeShaderProgram("work/res/shaders/occlusionShader.vert", "work/res/shaders/occlusionShader.frag");
	g_texShader = makeShaderProgram("work/res/shaders/texShader.vert", "work/res/shaders/texShader.frag");
	g_twoTexShader = makeShaderProgram("work/res/shaders/twoTexShader.vert", "work/res/shaders/twoTexShader.frag");
	g_blurShader = makeShaderProgram("work/res/shaders/blurShader.vert", "work/res/shaders/blurShader.frag");
	g_fongShader = makeShaderProgram("work/res/shaders/fongShader.vert", "work/res/shaders/fongShader.frag");
	g_texFongShader = makeShaderProgram("work/res/shaders/texFongShader.vert", "work/res/shaders/texFongShader.frag");
}
unsigned int createTexture(int w,int h,bool isDepth=false){
	unsigned int textureId;
	glGenTextures(1,&textureId);
	glBindTexture(GL_TEXTURE_2D,textureId);
	glTexImage2D(GL_TEXTURE_2D,0,(!isDepth ? GL_RGBA8 : GL_DEPTH_COMPONENT),w,h,0,isDepth ? GL_DEPTH_COMPONENT : GL_RGBA,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	int i;
	i=glGetError();
	if(i!=0){
		std::cout << "Error happened while loading the texture: " << i << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D,0);
	return textureId;
}
void init(){
	// FIRST FBO
	renderTexture0=createTexture(g_winWidth,g_winHeight);
	depthTexture0=createTexture(g_winWidth,g_winHeight,true);
	shadowMap0=createTexture(g_winWidth,g_winHeight,true);
	glGenFramebuffers(1,&FBO0);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTexture0,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTexture0,0);
	int i0=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i0!=GL_FRAMEBUFFER_COMPLETE){
		std::cout << "Framebuffer is not OK, status=" << i0 << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	// SECOND FBO
	renderTexture1=createTexture(g_winWidth,g_winHeight);
	depthTexture1=createTexture(g_winWidth,g_winHeight,true);
	shadowMap1=createTexture(g_winWidth,g_winHeight,true);
	glGenFramebuffers(1,&FBO1);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO1);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTexture1,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTexture1,0);
	int i1=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i1!=GL_FRAMEBUFFER_COMPLETE){
		std::cout << "Framebuffer is not OK, status=" << i1 << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);	
	
	// THIRD FBO
	renderTexture2=createTexture(g_winWidth,g_winHeight);
	depthTexture2=createTexture(g_winWidth,g_winHeight,true);
	shadowMap2=createTexture(g_winWidth,g_winHeight,true);
	glGenFramebuffers(1,&FBO2);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO2);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTexture2,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTexture2,0);
	int i2=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i2!=GL_FRAMEBUFFER_COMPLETE){
		std::cout << "Framebuffer is not OK, status=" << i2 << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	// FOURTH FBO
	renderTexture3=createTexture(g_winWidth,g_winHeight);
	depthTexture3=createTexture(g_winWidth,g_winHeight,true);
	glGenFramebuffers(1,&FBO3);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO3);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTexture3,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTexture3,0);
	int i3=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i3!=GL_FRAMEBUFFER_COMPLETE){
		std::cout << "Framebuffer is not OK, status=" << i3 << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}
//   ▄████████    ▄████████   ▄▄▄▄███▄▄▄▄      ▄████████    ▄████████    ▄████████ 
//  ███    ███   ███    ███ ▄██▀▀▀███▀▀▀██▄   ███    ███   ███    ███   ███    ███ 
//  ███    █▀    ███    ███ ███   ███   ███   ███    █▀    ███    ███   ███    ███ 
//  ███          ███    ███ ███   ███   ███  ▄███▄▄▄      ▄███▄▄▄▄██▀   ███    ███ 
//  ███        ▀███████████ ███   ███   ███ ▀▀███▀▀▀     ▀▀███▀▀▀▀▀   ▀███████████ 
//  ███    █▄    ███    ███ ███   ███   ███   ███    █▄  ▀███████████   ███    ███ 
//  ███    ███   ███    ███ ███   ███   ███   ███    ███   ███    ███   ███    ███ 
//  ████████▀    ███    █▀   ▀█   ███   █▀    ██████████   ███    ███   ███    █▀  
//                                                         ███    ███              
void setUpCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fovy * g_zoomFactor, float(g_winWidth) / float(g_winHeight), g_znear, g_zfar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, -1.5f, -10);
	glRotatef(g_xRotation, 1, 0, 0);
	glRotatef(g_yRotation, 0, 1, 0);
}
//     ▄████████  ▄████████    ▄████████ ███▄▄▄▄      ▄████████ 
//    ███    ███ ███    ███   ███    ███ ███▀▀▀██▄   ███    ███ 
//    ███    █▀  ███    █▀    ███    █▀  ███   ███   ███    █▀  
//    ███        ███         ▄███▄▄▄     ███   ███  ▄███▄▄▄     
//  ▀███████████ ███        ▀▀███▀▀▀     ███   ███ ▀▀███▀▀▀     
//           ███ ███    █▄    ███    █▄  ███   ███   ███    █▄  
//     ▄█    ███ ███    ███   ███    ███ ███   ███   ███    ███ 
//   ▄████████▀  ████████▀    ██████████  ▀█   █▀    ██████████ 
//                                                              
void drawScene(){
	// Begin Draw
	glPushMatrix();
	glTranslatef(0.20f,0.20f,2.0f);
	// FLOOR
	// glPushMatrix();
	// glColor4f(1,0,0,1);
	// glScalef(2.0,0.2,2.0);
	// glutSolidCube(1);
	// glPopMatrix();
	// obelisk
	glPushMatrix();
	glColor4f(0,1,0,1);
	glScalef(0.2,2.0,0.4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor4f(0,1,0,1);
	glTranslatef(1,0,0);
	glScalef(0.2,2.0,0.4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor4f(0,1,0,1);
	glTranslatef(0.5,0.5,0);
	glScalef(2.0,0.2,0.4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor4f(0,1,0,1);
	glTranslatef(0.5,-0.5,0);
	glScalef(2.0,0.2,0.4);
	glutSolidCube(1);
	glPopMatrix();

	// Balls
	glPushMatrix();
	glColor4f(0,0,1,1);
	glTranslatef(0.4,0.2,0);
	glutSolidSphere(0.1,20,20);
	glTranslatef(-0.8,0,0);
	glutSolidSphere(0.1,20,20);
	glPopMatrix();
	// End Draw
	glPopMatrix();
}
//   ▄█        ▄█     ▄██████▄     ▄█    █▄        ███     
//  ███       ███    ███    ███   ███    ███   ▀█████████▄ 
//  ███       ███▌   ███    █▀    ███    ███      ▀███▀▀██ 
//  ███       ███▌  ▄███         ▄███▄▄▄▄███▄▄     ███   ▀ 
//  ███       ███▌ ▀▀███ ████▄  ▀▀███▀▀▀▀███▀      ███     
//  ███       ███    ███    ███   ███    ███       ███     
//  ███▌    ▄ ███    ███    ███   ███    ███       ███     
//  █████▄▄██ █▀     ████████▀    ███    █▀       ▄████▀   
//  ▀
void drawLight(){
	glPushMatrix();
	glTranslatef(lightPos.x,lightPos.y,lightPos.z);
	glutSolidSphere(4,40,40);
	glPopMatrix();
}
//  ████████▄   ███    █▄     ▄████████ ████████▄  
//  ███    ███  ███    ███   ███    ███ ███   ▀███ 
//  ███    ███  ███    ███   ███    ███ ███    ███ 
//  ███    ███  ███    ███   ███    ███ ███    ███ 
//  ███    ███  ███    ███ ▀███████████ ███    ███ 
//  ███    ███  ███    ███   ███    ███ ███    ███ 
//  ███  ▀ ███  ███    ███   ███    ███ ███   ▄███ 
//   ▀██████▀▄█ ████████▀    ███    █▀  ████████▀  
//                                                 
void drawQuad(GLdouble winX, GLdouble winY,	GLdouble winZ){
    // glEnable(GL_TEXTURE_2D);
    int w = g_winWidth;
    int h = g_winHeight;

    //Convert to ortho
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, w, 0, h, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glBegin( GL_QUADS );
    glTexCoord2f(0,0);
    glVertex2f(0,0);  
    glTexCoord2f(1,0);
    glVertex2f(w,0);  
    glTexCoord2f(1,1);
    glVertex2f(w,h); 
    glTexCoord2f(0,1);
    glVertex2f(0,h);  
	glEnd();

}
//  ████████▄     ▄████████    ▄████████  ▄█     █▄  
//  ███   ▀███   ███    ███   ███    ███ ███     ███ 
//  ███    ███   ███    ███   ███    ███ ███     ███ 
//  ███    ███  ▄███▄▄▄▄██▀   ███    ███ ███     ███ 
//  ███    ███ ▀▀███▀▀▀▀▀   ▀███████████ ███     ███ 
//  ███    ███ ▀███████████   ███    ███ ███     ███ 
//  ███   ▄███   ███    ███   ███    ███ ███ ▄█▄ ███ 
//  ████████▀    ███    ███   ███    █▀   ▀███▀███▀  
//               ███    ███                          
void draw() {
	// TICK METHODS
	tree->tick();
	g_particleSystem->tick(1.f/60.f);
	g_particleSystemRope->tick(1.f/60.f);

	// UPDATE LIGHT POS
	if (g_lightParticle != nullptr){
		lightPos.x = g_lightParticle[0];
		lightPos.y = g_lightParticle[1];
		lightPos.z = g_lightParticle[2];
	}

	// Black background
	// glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	// Enable flags for normal rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	setUpCamera();

	// DRAW TO FBO ORIGINAL OCCLUSION MAP
	glBindFramebuffer(GL_FRAMEBUFFER,FBO0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// START OCCUSION DRAW 
		glUseProgram(g_occlusionShader);
		glUniform1i(glGetUniformLocation(g_occlusionShader, "isLight"),1);
		glUniform3f(glGetUniformLocation(g_occlusionShader, "lightColor"),volLightCol.x,volLightCol.y,volLightCol.z);
		drawLight();
		glUniform1i(glGetUniformLocation(g_occlusionShader, "isLight"),0);
		g_particleSystem->render();
		glPushMatrix();
			// glScalef(2,2,2);
			glRotatef(-90,1,0,0);
			tree->draw(5);
		glPopMatrix();
		glUseProgram(0);
// END OCCUSION DRAW
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	// DRAW ON QUAD RADIAL BLUR EFFECT
	glBindFramebuffer(GL_FRAMEBUFFER,FBO1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Enable Drawing texures
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,renderTexture0);//renderTexture, depthTexture, shadowMap
	    glUseProgram(g_volumetricShader);
	    // GET SCREEN SPACE COORDINATE OF LIGHT
	    GLdouble winX = 0;
	 	GLdouble winY = 0;
	 	GLdouble winZ = 0;
		GLdouble model_view[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
		GLdouble projection[16];
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		gluProject(lightPos.x,lightPos.y,lightPos.z,model_view,projection,viewport,&winX,&winY,&winZ);
		// cout << "winX: " << winX << "  winY: " << winY << "  winZ: " << winZ << endl;
		GLdouble normalizedWinX = winX/float(g_winWidth);
		GLdouble normalizedWinY = winY/float(g_winHeight);
		GLdouble normalizedWinZ = winZ;
		// cout << "normalizedWinX: " << normalizedWinX << "  normalizedWinY: " << normalizedWinY << "  winZ: " << winZ << endl;
		glUniform3f(glGetUniformLocation(g_volumetricShader, "lPos"),normalizedWinX,normalizedWinY,normalizedWinZ);
		drawQuad(normalizedWinX, normalizedWinY, normalizedWinZ);	
		glUseProgram(0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
// START OCCUSION DRAW		
		setUpCamera();
		glUseProgram(g_occlusionShader);
		glUniform1i(glGetUniformLocation(g_occlusionShader, "isLight"),0);
		g_particleSystem->render();
		glPushMatrix();
			// glScalef(2,2,2);
			glRotatef(-90,1,0,0);
			tree->draw(5);
		glPopMatrix();
		glEnable(GL_DEPTH_TEST);
		glUseProgram(0);
// END OCCUSION DRAW		
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	glBindFramebuffer(GL_FRAMEBUFFER,FBO3);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTexture3,0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,renderTexture1);
	    glUseProgram(g_volumetricShader);
   		glUniform3f(glGetUniformLocation(g_volumetricShader, "lPos"),normalizedWinX,normalizedWinY,normalizedWinZ);
		drawQuad(normalizedWinX, normalizedWinY, normalizedWinZ);	
		glUseProgram(0);		
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	glBindFramebuffer(GL_FRAMEBUFFER,FBO2);
		setUpCamera();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTexture0,0);
// START COLOR DRAW		
	    glUseProgram(g_fongShader);
		GLfloat ambient[] = { 0.20, 0.0, 0.0, 1.0 };
		GLfloat diffuse[] = { 0.5, 0.0, 0.0};
		GLfloat specular[] = {0.7, 0.6, 0.6};
		GLfloat shininess = 128.0f * 0.25f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
		glPushMatrix();
			// glScalef(2,2,2);
			glRotatef(-90,1,0,0);
			tree->draw(5);
		glPopMatrix();
		glUseProgram(0);

		glUseProgram(g_fongShader);
		g_particleSystemRope->render();
		glUseProgram(0);

		glUseProgram(g_texFongShader);
		glBindTexture(GL_TEXTURE_2D,g_beeTexture);
		g_particleSystem->render();
		glUseProgram(0);
// END COLOUR DRAW		
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	// Final draw scene to quad
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glUseProgram(g_twoTexShader);
	glUniform2f(glGetUniformLocation(g_twoTexShader, "lightPos"),normalizedWinX,normalizedWinY);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,renderTexture0);//renderTexture, depthTexture, shadowMap
    glUniform1i(glGetUniformLocation(g_twoTexShader, "texture0"), 0);
   
    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,renderTexture1);//renderTexture, depthTexture, shadowMap
    glUniform1i(glGetUniformLocation(g_twoTexShader, "texture1"), 1);
	
    glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,renderTexture3);//renderTexture, depthTexture, shadowMap
    glUniform1i(glGetUniformLocation(g_twoTexShader, "texture2"), 2);

	drawQuad(normalizedWinX, normalizedWinY, normalizedWinZ);
	glUseProgram(0);

	// Move the buffer we just drew to the front
	glutSwapBuffers();
	// Queue the next frame to be drawn straight away
	glutPostRedisplay();
}
//  ███    █▄      ███      ▄█   ▄█       
//  ███    ███ ▀█████████▄ ███  ███       
//  ███    ███    ▀███▀▀██ ███▌ ███       
//  ███    ███     ███   ▀ ███▌ ███       
//  ███    ███     ███     ███▌ ███       
//  ███    ███     ███     ███  ███       
//  ███    ███     ███     ███  ███▌    ▄ 
//  ████████▀     ▄████▀   █▀   █████▄▄██ 
//                              ▀         
void reshape(int w, int h) {
    if (h == 0) h = 1;
	g_winWidth = w;
	g_winHeight = h;
	init();
    glViewport(0, 0, g_winWidth, g_winHeight);  
}
//   ▄█  ███▄▄▄▄      ▄███████▄ ███    █▄      ███     
//  ███  ███▀▀▀██▄   ███    ███ ███    ███ ▀█████████▄ 
//  ███▌ ███   ███   ███    ███ ███    ███    ▀███▀▀██ 
//  ███▌ ███   ███   ███    ███ ███    ███     ███   ▀ 
//  ███▌ ███   ███ ▀█████████▀  ███    ███     ███     
//  ███  ███   ███   ███        ███    ███     ███     
//  ███  ███   ███   ███        ███    ███     ███     
//  █▀    ▀█   █▀   ▄████▀      ████████▀     ▄████▀   
//                                                     
void keyboardCallback(unsigned char key, int x, int y) {
	// cout << "Keyboard Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
		switch(key){
		case 'w': // 
			g_lightParticle[1]-=0.5; break;
		case 'a': // 
			g_lightParticle[0]-=0.5; break;
		case 's': // 
			g_lightParticle[1]+=0.5;break;
		case 'd': // 
			g_lightParticle[0]+=0.5;break;
		case 'q': // 
			g_lightParticle[2]+=0.5;break;
		case 'e': // 
			g_lightParticle[2]-=0.5;break;
		case '1': // 
			volLightCol.x = max(volLightCol.x-0.1f, 0.0f);break;
		case '2': // 
			volLightCol.y = max(volLightCol.y-0.1f, 0.0f);break;
		case '3': // 
			volLightCol.z = max(volLightCol.z-0.1f, 0.0f);break;
		case '4': // 
			volLightCol.x = min(volLightCol.x+0.1f, 1.0f);break;
		case '5': // 
			volLightCol.y = min(volLightCol.y+0.1f, 1.0f);break;
		case '6': // 
			volLightCol.z = min(volLightCol.z+0.1f, 1.0f);break;															
		}
}
void specialCallback(int key, int x, int y) {
	// cout << "Special Callback :: key=" << key << ", x,y=(" << x << "," << y << ")" << endl;
}
void mouseCallback(int button, int state, int x, int y) {
	// cout << "Mouse Callback :: button=" << button << ", state=" << state << ", x,y=(" << x << "," << y << ")" << endl;
	switch(button){
		case 0: // left mouse button
			g_mouseDown = (state==0); g_mousePos = vec2(x, y); break;
		case 2: // right mouse button
			if (state==0)g_useShader = !g_useShader; break;
		case 3: // scroll foward/up
			g_zoomFactor /= 1.1;break;
		case 4: // scroll back/down
			g_zoomFactor *= 1.1;break;
}}
void mouseMotionCallback(int x, int y) {
	// cout << "Mouse Motion Callback :: x,y=(" << x << "," << y << ")" << endl;
	if (g_mouseDown) {
		vec2 dif = vec2(x,y) - g_mousePos;
		g_mousePos = vec2(x,y);
		g_yRotation += 0.3 * dif.x;
		g_xRotation += 0.3 * dif.y;
}}
//     ▄▄▄▄███▄▄▄▄      ▄████████  ▄█  ███▄▄▄▄   
//   ▄██▀▀▀███▀▀▀██▄   ███    ███ ███  ███▀▀▀██▄ 
//   ███   ███   ███   ███    ███ ███▌ ███   ███ 
//   ███   ███   ███   ███    ███ ███▌ ███   ███ 
//   ███   ███   ███ ▀███████████ ███▌ ███   ███ 
//   ███   ███   ███   ███    ███ ███  ███   ███ 
//   ███   ███   ███   ███    ███ ███  ███   ███ 
//    ▀█   ███   █▀    ███    █▀  █▀    ▀█   █▀  
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
	initLight();
	initShader();
	initTexture();
	init();

	g_lights = new Lights();
	g_particleSystem = new SwarmParticleSystem(200, 6);
	g_particleSystem->create();

	g_particleSystemRope = new RopeParticleSystem(50, 6);
	g_particleSystemRope->create();

	glutMainLoop();
	// Don't forget to delete all pointers that we made
	return 0;
}