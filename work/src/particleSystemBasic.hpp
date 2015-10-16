#pragma once

#include <cmath>
#include <vector>

#include "particleSystem.hpp"
#include "particleConstraint.hpp"
#include "particleConstraintDistance.hpp"
#include "particleConstraintAngle.hpp"
#include "particleConstraintPin.hpp"
#include "particleConstraintPlane.hpp"


using namespace std;

class Box : public ParticleConstraint {
private:
	float *m_particle;

	float m_minX = -10;
	float m_maxX =  10;

	float m_minY = -10;
	float m_maxY =  10;

	float m_minZ = -10;
	float m_maxZ =  10;
public:
	Box(float *particle) {
		m_particle = particle;
	}

	bool solve() {
		float x = m_particle[0];
		float y = m_particle[1];
		float z = m_particle[2];

		float extra = 1.f;

		if(x < m_minX) m_particle[0] = m_minX + extra;
		if(x > m_maxX) m_particle[0] = m_maxX - extra;

		if(y < m_minY) m_particle[1] = m_minY + extra;
		if(y > m_maxY) m_particle[1] = m_maxY - extra;

		if(z < m_minZ) m_particle[2] = m_minZ + extra;
		if(z > m_maxZ) m_particle[2] = m_maxZ - extra;

		return false;
	}

	void render() {

	}

};

class Ball : public ParticleConstraint {
private:
	float *m_a;
	float *m_b;

	float m_radiusA;
	float m_radiusB;
	float m_restingDistance;
public:
	Ball(float *a, float *b, float radiusA, float radiusB) {
		m_a = a;
		m_b = b;
		m_radiusA = radiusA;
		m_radiusB = radiusB;
		m_restingDistance = m_radiusA + m_radiusB;
	}

	bool solve() {

		//difference
		float dx = m_a[0] - m_b[0];
		float dy = m_a[1] - m_b[1];
		float dz = m_a[2] - m_b[2];

		//dislodge if overlapping
		if(dx == 0 && dy == 0 && dz == 0) {
			dx = -1 + (rand()/(double(RAND_MAX) + 1))*2 * 0.00001f;
			dy = -1 + (rand()/(double(RAND_MAX) + 1))*2 * 0.00001f;
			dz = -1 + (rand()/(double(RAND_MAX) + 1))*2 * 0.00001f;
		}

		float d = sqrt(dx*dx + dy*dy + dz*dz);

		if(d > m_restingDistance) {
			return false;
		}

		if(d < 0.001f) {
			// d = 0;
			d = m_restingDistance;
		} else {
			d = (m_restingDistance - d) / d;
		}

		//translation
		float tx = dx * 0.5f * d;
		float ty = dy * 0.5f * d;
		float tz = dz * 0.5f * d;

		//reassign
		m_a[0] += tx;
		m_a[1] += ty;
		m_a[2] += tz;

		m_b[0] -= tx;
		m_b[1] -= ty;
		m_b[2] -= tz;

		return false;
	}

	void render() {

	}
};

class BasicParticleSystem: public ParticleSystem {
private:

public:
	BasicParticleSystem(uint maxParticles, uint constraintIterations) : ParticleSystem(maxParticles, constraintIterations) {

	}

	void create() {
		m_gravityY = -100;
		m_dragX = m_dragZ = 0.98f;
		m_dragY = 0.96f;

		for(uint i = 0; i < 20; i++) {
			float *p;
			// createParticle(-5 + (rand()/(double(RAND_MAX) + 1))*10,
			// 			   -5 + (rand()/(double(RAND_MAX) + 1))*10,
			// 			   -5 + (rand()/(double(RAND_MAX) + 1))*10, &p);
			createParticle(-4, 10 - i*0.4f, -4, &p);
			p[0] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			p[1] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			p[2] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			m_constraints.push_back(new Box(p));

			createParticle(-4, 10 - i*0.4f, 4, &p);
			p[0] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			p[1] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			p[2] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			m_constraints.push_back(new Box(p));

			createParticle(4, 10 - i*0.4f, -4, &p);
			p[0] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			p[1] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			p[2] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			m_constraints.push_back(new Box(p));

			createParticle(4, 10 - i*0.4f, 4, &p);
			p[0] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			p[1] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			p[2] += -0.1f + (rand()/(double(RAND_MAX) + 1))*0.2f;
			m_constraints.push_back(new Box(p));
			// p[0] = p[1] = p[2] = 0;

			
		}

		for(uint i = 0; i < m_particleCount-1; i++) {
			for(uint j = 1; j < m_particleCount; j++) {
				m_constraints.push_back(new Ball(&m_particles[i*NUM_COMPONENTS],
												 &m_particles[j*NUM_COMPONENTS],
												 0.2f,
												 0.2f));
			}
		}



		cout << "Created " << m_particleCount << " particles." << endl;
	}

	void render() {
		//Create a new GLUquadric object; to allow you to draw cylinders
		GLUquadric *quad = gluNewQuadric();
		if (quad == 0) {
			cerr << "Not enough memory to allocate space to draw" << endl;
			exit(EXIT_FAILURE);
		}

		for(uint i = 0; i < m_particleCount; i++) {
			float *p = &m_particles[i*NUM_COMPONENTS];

			glPushMatrix();
				glTranslatef(p[0], p[1], p[2]);
				gluSphere(quad, 0.2f, 8, 8);
			glPopMatrix();
		}

		gluDeleteQuadric(quad);
	}
};