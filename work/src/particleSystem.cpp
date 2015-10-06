#include <vector>
#include <random>

#include "particleSystem.hpp"
#include "particleConstraint.hpp"
#include "particleConstraintDistance.hpp"
#include "particleConstraintPin.hpp"
#include "comp308.hpp"

using namespace std;

extern GLuint g_mrtShader;

ParticleSystem::ParticleSystem() {
	for(uint i = 0; i < MAX_PARTICLES*6; i+=6) {
		particles[i]   = (rand()/(double(RAND_MAX) + 1)) < 0.5f ? -0.01f : 0.01f;
		particles[i+1] = (rand()/(double(RAND_MAX) + 1)) < 0.5f ? -0.01f : 0.01f;
		particles[i+2] = (rand()/(double(RAND_MAX) + 1)) < 0.5f ? -0.01f : 0.01f;
		particles[i+3] = 0; //(rand()/(double(RAND_MAX) + 1)) < 0.5f ? -0.01f : 0.01f;
		particles[i+4] = 0; //(rand()/(double(RAND_MAX) + 1)) < 0.5f ? -0.01f : 0.01f;
		particles[i+5] = 0; //(rand()/(double(RAND_MAX) + 1)) < 0.5f ? -0.01f : 0.01f;
	}

	for(uint i = 6; i < (MAX_PARTICLES-1)*6; i+=6) {
		for(uint j = 12; j < MAX_PARTICLES*6; j+=6) {
			ParticleConstraint *constraint = new ParticleConstraintDistance(i,j,5,0.01f);
			constraints.push_back(constraint);
		}
	}

	ParticleConstraint *pin = new ParticleConstraintPin(0, 0, 0, 0);
	constraints.push_back(pin);

	// GLuint renderbufferID;
	// glGenRenderbuffers(1, &renderbufferID);
	// glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);
	// glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, )
}

void ParticleSystem::tick(float delta) {
	integrateParticles(delta);

	for(uint i = 0; i < 6; i++) {
		for(uint j = 0; j < constraints.size(); j++) {
			constraints[j]->solve(particles);
		}
	}
}

void ParticleSystem::render() {
	// glUseProgram(g_mrtShader);

	for(uint i = 0; i < constraints.size(); i++) {
		constraints[i]->render(particles);
	}

	glColor3f(1,0,0);
	glBegin(GL_TRIANGLES);
		glVertex2i(-1, -1);
		glVertex2i(1, -1);
		glVertex2i(0, 1);

		glVertex2i(1, -1);
		glVertex2i(-1, -1);
		glVertex2i(0, 1);
	glEnd();

	glPushMatrix();

	//Create a new GLUquadric object; to allow you to draw cylinders
	GLUquadric *quad = gluNewQuadric();
	if (quad == 0) {
		cerr << "Not enough memory to allocate space to draw" << endl;
		exit(EXIT_FAILURE);
	}

	for(uint i = 0; i < MAX_PARTICLES*6; i+=6) {
		float x = particles[i];
		float y = particles[i+1];
		float z = particles[i+2];

		// cout << "x:" << x << " y:" << y << " z:" << z << endl;

		glPushMatrix();
			glTranslatef(x, y, z);
			gluSphere(quad, 0.25f, 16, 16);
		glPopMatrix();
	}

	gluDeleteQuadric(quad);
	glPopMatrix();

	// glUseProgram(0);
}

void ParticleSystem::integrateParticles(float delta) {
	for(uint i = 0; i < MAX_PARTICLES*6; i+=6) {
		//values
		float x = particles[i];
		float y = particles[i+1];
		float z = particles[i+2];

		//old values
		float ox = particles[i+3];
		float oy = particles[i+4];
		float oz = particles[i+5];

		//modulators
		float drag = 0.4f;
		float gravity = 0.0f;

		//integrate //nx = x + vel*drag + acc * delta^2
		float nx = x + (x - ox)*drag + gravity*delta*delta; 
		float ny = y + (y - oy)*drag + gravity*delta*delta; 
		float nz = z + (z - oz)*drag + gravity*delta*delta; 

		//assign old values
		particles[i+3] = x;
		particles[i+4] = y;
		particles[i+5] = z;

		//assign new values
		particles[i]   = nx;
		particles[i+1] = ny;
		particles[i+2] = nz;
	}
}
