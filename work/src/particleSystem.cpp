#include <vector>
#include <random>

#include "particleSystem.hpp"
#include "particleConstraint.hpp"
#include "particleConstraintDistance.hpp"
#include "particleConstraintPin.hpp"

using namespace std;

bool ParticleSystem::createParticle(float x, float y, float z, float **pointer) {
	if(m_particleCount >= MAX_PARTICLES) {
		return false;
	}

	uint idx = m_particleCount * NUM_COMPONENTS;

	//pos
	m_particles[idx]   = x;
	m_particles[idx+1] = y;
	m_particles[idx+2] = z;

	//old pos
	m_particles[idx+3] = x;
	m_particles[idx+4] = y;
	m_particles[idx+5] = z;

	*pointer = &m_particles[idx];

	m_particleCount++;

	return true;
}

void ParticleSystem::tick(float delta) {
	integrateParticles(delta);

	createConstraints();

	for(uint i = 0; i < m_constraintIterations; i++) {
		solveConstraints(delta);
	}

	// cout << "Constraints: " << m_constraints.size() << endl;
}

void ParticleSystem::render() {
	for(uint i = 0; i < m_constraints.size(); i++) {
		m_constraints[i]->render();
	}

	// glColor3f(1,0,0);
	// glBegin(GL_TRIANGLES);
	// 	glVertex2i(-1, -1);
	// 	glVertex2i(1, -1);
	// 	glVertex2i(0, 1);

	// 	glVertex2i(1, -1);
	// 	glVertex2i(-1, -1);
	// 	glVertex2i(0, 1);
	// glEnd();

	glPushMatrix();

	//Create a new GLUquadric object; to allow you to draw cylinders
	GLUquadric *quad = gluNewQuadric();
	if (quad == 0) {
		cerr << "Not enough memory to allocate space to draw" << endl;
		exit(EXIT_FAILURE);
	}

	glColor3f(1,0,0);
	for(uint i = 0; i < m_particleCount*6; i+=6) {
		float x = m_particles[i];
		float y = m_particles[i+1];
		float z = m_particles[i+2];

		glPushMatrix();
			glTranslatef(x, y, z);
			gluSphere(quad, 0.1f, 8, 8);
		glPopMatrix();
	}

	gluDeleteQuadric(quad);
	glPopMatrix();
}

void ParticleSystem::integrateParticles(float delta) {
	for(uint i = 0; i < m_particleCount*NUM_COMPONENTS; i+=NUM_COMPONENTS) {
		//values
		float x = m_particles[i];
		float y = m_particles[i+1];
		float z = m_particles[i+2];

		//old values
		float ox = m_particles[i+3];
		float oy = m_particles[i+4];
		float oz = m_particles[i+5];

		//modulators
		// float drag = 0.4f;
		// float gravity = 0.0f;

		//integrate //nx = x + vel*drag + acc * delta^2
		float nx = x + (x - ox)*m_dragX + m_gravityX*delta*delta; 
		float ny = y + (y - oy)*m_dragY + m_gravityY*delta*delta; 
		float nz = z + (z - oz)*m_dragZ + m_gravityZ*delta*delta; 

		//assign old values
		m_particles[i+3] = x;
		m_particles[i+4] = y;
		m_particles[i+5] = z;

		//assign new values
		m_particles[i]   = nx;
		m_particles[i+1] = ny;
		m_particles[i+2] = nz;
	}
}

void ParticleSystem::solveConstraints(float delta) {
	for(uint i = 0; i < m_constraints.size(); i++) {
		if(m_constraints[i]->solve()) {
			m_constraints.erase(m_constraints.begin() + i);
		}
	}
}
