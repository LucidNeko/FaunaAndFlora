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


using namespace std;

class SwarmParticleSystem: public ParticleSystem {
private:

	float *m_pin;
public:
	SwarmParticleSystem(uint maxParticles, uint constraintIterations) : ParticleSystem(maxParticles, constraintIterations) {

	}

	void create() {

		float *a, *b;
		createParticle(-10, 0, 0, &a);
		createParticle( 10, 0, 0, &b);
		createParticle(  0, 0, 0, &m_pin);

		a[0] = -9;
		b[0] = 9;

		m_constraints.push_back(new ParticleConstraintPin(m_pin));

		for(uint i = 0; i < 50; i++) {
			float *p;
			createParticle(0,0,0,&p);

			m_constraints.push_back(new Attraction(p, m_pin, 0.1f, 0.5f));
			m_constraints.push_back(new Random(p, 0.005f));
		}

		m_constraints.push_back(new Attraction(a, b, 0.001f, 0.5f));
		m_constraints.push_back(new Attraction(b, a, 0.001f, 0.5f));

		m_constraints.push_back(new Random(a, 0.001f));
		m_constraints.push_back(new Random(b, 0.001f));

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
				}
			}
		}

		for(uint i = 0; i < m_particleCount-1; i++) {
			float *a = &m_particles[i*NUM_COMPONENTS];

			if(sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]) > 10) {
				m_constraints.push_back(new Attraction(a, m_pin, 0.00001f, 9.99f));
			}
		}

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

	// void render() {

	// }
};