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
#include "comp308.hpp"

using namespace std;
using namespace comp308;

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
		float baseZ = -20.0f;

		createParticle(baseX, baseY, baseZ, &m_pin);
		m_constraints.push_back(new ParticleConstraintPin(m_pin));

		for(uint i = 0; i < 25; i++) {
			float *p;
			createParticle(baseX, baseY - i*(height/25), baseZ, &p);

			g_lightParticle = p;

			float *p_before = &m_particles[(m_particleCount-2) * NUM_COMPONENTS];
			m_constraints.push_back(new ParticleConstraintDistance(p_before, p, 0.3f, false));
		}

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
			float *first = &m_particles[0];
			glTranslatef(first[0], first[1], first[2]);
			// for(uint i = 6; i < m_particleCount*6; i+=6) {
			for(uint i = 1; i < m_particleCount; i++) {
				float *ap = &m_particles[(i-1) * NUM_COMPONENTS];
				float *bp = &m_particles[(i  ) * NUM_COMPONENTS];

				vec3 a = vec3(ap[0], ap[1], ap[2]);
				vec3 b = vec3(bp[0], bp[1], bp[2]);
				vec3 d = normalize(b-a);
				float l = length(b-a);

				//calculate rotation axis and angle
				vec3 axis = cross(vec3(0,0,1), d);
				float angle = acos(dot(vec3(0,0,1), d)/length(d));

				glPushMatrix();
					//rotate
					glRotatef(angle * 180.0f / 3.14159265359f, axis.x, axis.y, axis.z);
				
					gluCylinder(quad, 0.1f, 0.1f, l, 16, 1 + l*16);
				glPopMatrix();

				vec3 trans = d * l;
				glTranslatef(trans.x, trans.y, trans.z);
			}
		glPopMatrix();

		//last
		glPushMatrix();
			float *last = &m_particles[(m_particleCount-1) * NUM_COMPONENTS];
			glTranslatef(last[0], last[1], last[2]);
			gluSphere(quad, 2.f, 32, 32);
		glPopMatrix();

		gluDeleteQuadric(quad);
	}
};