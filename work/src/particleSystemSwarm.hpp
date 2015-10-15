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

class SwarmParticleSystem: public ParticleSystem {
private:
	float *m_pin;

	IRenderable *bee;
	IRenderable *leftWing;
	IRenderable *rightWing;
	vector<GameObject *> m_renderables;
public:
	SwarmParticleSystem(uint maxParticles, uint constraintIterations) : ParticleSystem(maxParticles, constraintIterations) {

	}

	void create() {
		createParticle(  0, 0, 0, &m_pin);
		m_constraints.push_back(new ParticleConstraintPin(m_pin));

		for(uint i = 0; i < 50; i++) {
			float *p;
			createParticle(-5 + (rand()/(double(RAND_MAX) + 1))*10, 
						   -5 + (rand()/(double(RAND_MAX) + 1))*10,
						   -5 + (rand()/(double(RAND_MAX) + 1))*10, &p);

			m_constraints.push_back(new Move(p, 0.005f));
		}


		//create bees //1 so skip pin
		bee = new OBJLoader("work/res/assets/bee/bee_body.obj");
		leftWing = new OBJLoader("work/res/assets/bee/bee_leftWing.obj");
		rightWing = new OBJLoader("work/res/assets/bee/bee_rightWing.obj");
		

		for(uint i = 1; i < m_particleCount; i++) {
			GameObject *renderable = new GameObject(&m_particles[i*NUM_COMPONENTS], bee);
			GameObject *lw = new GameObject(nullptr, leftWing);
			GameObject *rw = new GameObject(nullptr, rightWing);
			renderable->addChild(lw);
			renderable->addChild(rw);
			m_renderables.push_back(renderable);
		}

		cout << "Created " << m_particleCount << " particles." << endl;
	}

	void createConstraints() {
		for(uint i = 0; i < m_particleCount-1; i++) {
			for(uint j = 1; j < m_particleCount; j++) {
				float *a = &m_particles[i*NUM_COMPONENTS];
				float *b = &m_particles[j*NUM_COMPONENTS];

				float dx = a[0] - b[0];
				float dy = a[1] - b[1];
				float dz = a[2] - b[2];

				if(sqrt(dx*dx + dy*dy + dz*dz) < 2) {
					m_constraints.push_back(new ParticleConstraintDistance(a, b, 1, 0.01f));
					// if((rand()/(double(RAND_MAX) + 1)) < 0.001f) {
					// 	m_constraints.push_back(new Follow(a, b, 0.00001f));
					// }
				}
			}
		}

		//if far away whip back into center
		for(uint i = 0; i < m_particleCount-1; i++) {
			float *a = &m_particles[i*NUM_COMPONENTS];

			if(sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]) > 2) {
				m_constraints.push_back(new Attraction(a, m_pin, 0.000001f, 1.99f));
			}
		}

		//if near light
		for(uint i = 0; i < m_particleCount-1; i++) {
			float *a = &m_particles[i*NUM_COMPONENTS];
			float *b = g_lightParticle;

			float dx = a[0] - b[0];
			float dy = a[1] - b[1];
			float dz = a[2] - b[2];

			// if(sqrt(dx*dx + dy*dy + dz*dz) > 2) {
			// 	m_constraints.push_back(new Attraction(a, m_pin, 0.000001f, 1.99f));
			// }

			if(sqrt(dx*dx + dy*dy + dz*dz) < 15) {
				m_constraints.push_back(new Attraction(a, b, 0.000001f, 10.f));
			}
		}

		//random attraction
		for(uint i = 0; i < m_particleCount; i++) {
			if((rand()/(double(RAND_MAX) + 1)) < 0.001f) {
				uint j = uint((rand()/(double(RAND_MAX) + 1)) * m_particleCount);

				if(i != j) {
					m_constraints.push_back(new Attraction(&m_particles[i*NUM_COMPONENTS], &m_particles[j*NUM_COMPONENTS], 0.0001f, 3.f));
					m_constraints.push_back(new Attraction(&m_particles[j*NUM_COMPONENTS], &m_particles[i*NUM_COMPONENTS], 0.0001f, 3.f));
				}

			}
		}

	}

	void render() {
		glPushMatrix();
		glTranslatef(0,3,0);
		glScalef(0.3f, 0.3f, 0.3f);
		for(uint i = 0; i < m_renderables.size(); i++) {
			m_renderables[i]->tick(0.1f);
			m_renderables[i]->render();
		}	
		glPopMatrix();
	}
};