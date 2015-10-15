#pragma once

#include <cmath>
#include <vector>

#include "particleSystem.hpp"
#include "particleConstraint.hpp"
#include "particleConstraintDistance.hpp"
#include "particleConstraintAngle.hpp"
#include "particleConstraintPin.hpp"
#include "particleConstraintPlane.hpp"
#include "particleBehaviours.hpp"
#include "OBJLoader.hpp"
#include "gameObject.hpp"
#include "IRenderable.hpp"

using namespace std;

extern float *g_lightParticle;

class RopeParticleSystem: public ParticleSystem {
private:
	float *m_pin;

	IRenderable *bee;
	IRenderable *leftWing;
	IRenderable *rightWing;
	vector<GameObject *> m_renderables;
public:
	RopeParticleSystem(uint maxParticles, uint constraintIterations) : ParticleSystem(maxParticles, constraintIterations) {

	}

	void create() {
	m_gravityY = -98.0f;

		uint height = 10;
		float baseX = 10.0f;
		float baseY = 5.5f + height;
		float baseZ = -30.0f;

		createParticle(baseX, baseY, baseZ, &m_pin);
		m_constraints.push_back(new ParticleConstraintPin(m_pin));

		for(uint i = 0; i < 20; i++) {
			float *p;
			createParticle(baseX, baseY - i*(height/20), baseZ, &p);

			g_lightParticle = p;

			float *p_before = &m_particles[(m_particleCount-2) * NUM_COMPONENTS];
			m_constraints.push_back(new ParticleConstraintDistance(p_before, p, 0.2f, false));
		}

		// g_lightParticle[0] = -0.01f;

		cout << "Created " << m_particleCount << " particles." << endl;
	}

	void createConstraints() {

	}

	void render() {

		GLUquadric *quad = gluNewQuadric();
		if (quad == 0) {
			cerr << "Not enough memory to allocate space to draw" << endl;
			exit(EXIT_FAILURE);
		}

		glPushMatrix();
			for(uint i = 0; i < m_particleCount*6; i+=6) {
				float x = m_particles[i];
				float y = m_particles[i+1];
				float z = m_particles[i+2];

				glPushMatrix();
					glTranslatef(x, y, z);
					gluSphere(quad, 0.1f, 8, 8);
				glPopMatrix();
			}
		glPopMatrix();

		gluDeleteQuadric(quad);
	}
};