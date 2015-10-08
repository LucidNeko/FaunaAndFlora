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

class BasicParticleSystem: public ParticleSystem {
private:

public:
	BasicParticleSystem(uint maxParticles, uint constraintIterations) : ParticleSystem(maxParticles, constraintIterations) {

	}

	void create() {

		float **p = new float*[MAX_PARTICLES];

		createParticle(0,0,0,&p[0]);
		m_constraints.push_back(new ParticleConstraintPin(p[0]));

		float PI = 3.1415926535897932384626433832795;
		float radius = 2;
		uint nSides = 8;
		uint layers = 10;
		for(uint layer = 1; layer < layers; layer++) {
			for(double i = 0; i < 2 * PI; i += PI / (nSides/2)) {
				createParticle(0 + cos(i) * radius*layer, layer*1.5f, 0 + sin(i) * radius*layer, &p[m_particleCount]);
			}
		}

		for(uint i = 1; i < m_particleCount; i += nSides) {
			m_constraints.push_back(new ParticleConstraintDistance(p[i], p[i+nSides-1], 0.01));
			for(uint j = 1; j < nSides; j++) {
				m_constraints.push_back(new ParticleConstraintDistance(p[i+ j-1], p[i+ j], 0.01));								
			}
		}

		for(uint i = 1; i < nSides+1; i++) {
			m_constraints.push_back(new ParticleConstraintDistance(p[0], p[i], 0.01));				
		}


		for(uint i = 1 + nSides; i < m_particleCount; i += nSides) {
			for(uint j = 0; j < nSides; j++) {
				m_constraints.push_back(new ParticleConstraintDistance(p[i+ j], p[i+ j - nSides], 0.01));								
			}
		}

		createParticle(0, -10, 0, &p[m_particleCount]);
		m_constraints.push_back(new ParticleConstraintPin(p[m_particleCount-1]));

		for(uint i = 1; i < m_particleCount-1; i++) {
			m_constraints.push_back(new ParticleConstraintDistance(p[m_particleCount-1], p[i], 20, 0.01f));								
		}

		delete[] p;



		// float *p;
		// bool isValid;
		// while((isValid = createParticle(0, 0, 0, &p)) == true);

		// ParticleConstraint *pin = new ParticleConstraintPin(&m_particles[0], 0, 0, 0);
		// m_constraints.push_back(pin);

		// for(uint i = 1; i < m_particleCount-1; i++) {
		// 	for(uint j = 2; j < m_particleCount; j++) {
		// 		ParticleConstraint *constraint = new ParticleConstraintDistance(&m_particles[i*NUM_COMPONENTS],&m_particles[j*NUM_COMPONENTS],5.f,0.05f);
		// 		m_constraints.push_back(constraint);
		// 	}
		// }

		// ParticleConstraint *constraint = new ParticleConstraintDistance(&m_particles[0],&m_particles[6],5.f,0.05f);
		// m_constraints.push_back(constraint);

		// float *p[MAX_PARTICLES];
		// for(uint i = 0; i < MAX_PARTICLES; i++) {
		// 	createParticle(i, sin(i), 0, &p[i]);
		// }

		// // createParticle(10, 0, 0, &p[MAX_PARTICLES-1]);

		// //pin base
		// m_constraints.push_back(new ParticleConstraintPin(p[0], 0, 0, 0));

		// for(uint i = 2; i < m_particleCount; i++) {
		// 	m_constraints.push_back(new ParticleConstraintAngle(p[i-2], p[i-1], p[i], 0.349066f, 1.0f));
		// }

		// float *p_core[10];
		// for(uint i = 0; i < 10; i++) {
		// 	createParticle(i, i, 0, &p_core[i]);
		// }

		// float *p_supp[32];
		// for(uint i = 0; i < 32; i+=4) {
		// 	createParticle(1+ i/4 -0.25f, 1+ i/4 + 0.25f, 0, &p_supp[i]);
		// 	createParticle(1+ i/4 +0.25f, 1+ i/4 - 0.25f, 0, &p_supp[i+1]);

		// 	// m_constraints.push_back(new ParticleConstraintPlane(p_supp[i], false, false, true));
		// 	// m_constraints.push_back(new ParticleConstraintPlane(p_supp[i+1], false, false, true));

		// 	createParticle(1+ i/4, 1+ i/4, + 0.25f, &p_supp[i+2]);
		// 	createParticle(1+ i/4, 1+ i/4, - 0.25f, &p_supp[i+3]);

		// 	// m_constraints.push_back(new ParticleConstraintPlane(p_supp[i+2], false, false, true));
		// 	// m_constraints.push_back(new ParticleConstraintPlane(p_supp[i+3], false, false, true));
		// }		

		// m_constraints.push_back(new ParticleConstraintPin(p_core[0]));
		// // m_constraints.push_back(new ParticleConstraintPin(p_core[1]));
		// // m_constraints.push_back(new ParticleConstraintPin(p_core[2]));
		// // m_constraints.push_back(new ParticleConstraintPin(p_core[3]));
		// // m_constraints.push_back(new ParticleConstraintPin(p_core[4]));
		// // m_constraints.push_back(new ParticleConstraintPin(p_core[5]));

		// float *p_brace;
		// createParticle(-1, -4, 0, &p_brace);

		// m_constraints.push_back(new ParticleConstraintPin(p_brace));
		// for(uint i = 0; i < 10; i++) {
		// 	m_constraints.push_back(new ParticleConstraintDistance(p_brace, p_core[i], 10, 0.01f));
		// }


		// //core branch
		// for(uint i = 1; i < 10; i++) {
		// 	m_constraints.push_back(new ParticleConstraintDistance(p_core[i-1], p_core[i], 1.0f));
		// }

		// // 1 -> 0/1
		// // 2 -> 2/3
		// // 3 -> 4/5

		// //kites
		// for(uint i = 1, j = 0; i < 9; i++) {
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i-1], p_supp[j], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i-1], p_supp[j+1], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i-1], p_supp[j+2], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i-1], p_supp[j+3], 1.0f));

		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i], p_supp[j], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i], p_supp[j+1], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i], p_supp[j+2], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i], p_supp[j+3], 1.0f));

		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_supp[j], p_supp[j+2], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_supp[j+2], p_supp[j+1], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_supp[j+1], p_supp[j+3], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_supp[j+3], p_supp[j], 1.0f));

		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i+1], p_supp[j], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i+1], p_supp[j+1], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i+1], p_supp[j+2], 1.0f));
		// 	// m_constraints.push_back(new ParticleConstraintDistance(p_core[i+1], p_supp[j+3], 1.0f));

		// 	j+=4;
		// }


		// for(uint i = 1; i < MAX_PARTICLES; i++) {
		// 	m_constraints.push_back(new ParticleConstraintDistance(p[i-1], p[i], 0.01f));
		// }

		// //pin end
		// m_constraints.push_back(new ParticleConstraintPin(p[MAX_PARTICLES-1], 10, 0, 0));		

		// for(uint i = 2; i < MAX_PARTICLES; i++) {
		// 	m_constraints.push_back(new ParticleConstraintDistance(p[i-1], p[i], 0.01f));
		// 	if(i > 1) {
		// 		float *m_a = p[i-2];
		// 		float *m_b = p[i];

		// 		float dx = m_a[0] - m_b[0];
		// 		float dy = m_a[1] - m_b[1];
		// 		float dz = m_a[2] - m_b[2];

		// 		float distance = sqrt(dx*dx + dy*dy + dz*dz);

		// 		if( i < MAX_PARTICLES-1) {
		// 			m_constraints.push_back(new ParticleConstraintDistance(p[0], p[i], 0.01f));
		// 		} else {
		// 			m_constraints.push_back(new ParticleConstraintDistance(p[i-2], p[i], distance*0.5f, 0.01f));
		// 		}
		// 	}
		// }


		cout << "Created " << m_particleCount << " particles." << endl;
	}

	// void render() {

	// }
};