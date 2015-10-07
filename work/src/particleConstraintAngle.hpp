#pragma once

#include "comp308.hpp"
#include "particleConstraint.hpp"
#include <cmath>
#include <random>

using namespace comp308;

class ParticleConstraintAngle: public ParticleConstraint {
private:
	//particles
	float *m_a; 
	float *m_b;
	float *m_c;

	float m_restAtoB;
	float m_restCtoB;

	float m_angle;
	float m_softness;
public:

	// ParticleConstraintAngle(float *a, float *b, float *c, float softness) {
	// 	m_a = a;
	// 	m_b = b;
	// 	m_c = c;

	// 	vec3 va = vec3(m_a[0] - m_b[0], m_a[1] - m_b[1], m_a[2] - m_b[2]);
	// 	vec3 vb = vec3(m_c[0] - m_b[0], m_c[1] - m_b[1], m_c[2] - m_b[2]);

	// 	m_restAtoB = length(va);
	// 	m_restCtoB = length(vb);

	// 	va = normalize(va);
	// 	vb = normalize(vb);
		
	// 	// vec3 axis = cross(va, vb);
	// 	m_angle = acos(dot(va, vb));

	// 	// m_angle = atan2(normalize(cross(va, vb)), dot(va, vb));
	// 	m_softness = softness;
	// }	

	ParticleConstraintAngle(float *a, float *b, float *c, float angle, float softness) {
		m_a = a;
		m_b = b;
		m_c = c;

		vec3 va = vec3(m_a[0] - m_b[0], m_a[1] - m_b[1], m_a[2] - m_b[2]);
		vec3 vb = vec3(m_c[0] - m_b[0], m_c[1] - m_b[1], m_c[2] - m_b[2]);

		m_restAtoB = length(va);
		m_restCtoB = length(vb);

		m_angle = angle;
		m_softness = softness;
	}

	//solve returns true if this constraint should be deleted
	bool solve() {

		float length = sqrt(m_restAtoB*m_restAtoB + m_restCtoB*m_restCtoB - 2*m_restAtoB*m_restCtoB*cos(m_angle));

		

		//difference
		float dx = m_a[0] - m_c[0];
		float dy = m_a[1] - m_c[1];
		float dz = m_a[2] - m_c[2];

		//dislodge if overlapping
		if(dx == 0 && dy == 0 && dz == 0) {
			dx = -1 + (rand()/(double(RAND_MAX) + 1))*2 * 0.00001f;
			dy = -1 + (rand()/(double(RAND_MAX) + 1))*2 * 0.00001f;
			dz = -1 + (rand()/(double(RAND_MAX) + 1))*2 * 0.00001f;
		}

		float d = sqrt(dx*dx + dy*dy + dz*dz);

		if(d < 0.01f) {
			// d = 0;
			d = length;
		} else {
			d = (length - d) / d;
		}

		//translation
		float tx = dx * 0.5f * d * m_softness;
		float ty = dy * 0.5f * d * m_softness;
		float tz = dz * 0.5f * d * m_softness;

		//reassign
		m_a[0] += tx;
		m_a[1] += ty;
		m_a[2] += tz;

		m_c[0] -= tx;
		m_c[1] -= ty;
		m_c[2] -= tz;


		return false; //don't delete
	}

	void render() {
		glColor3f(0,0,1);
		glBegin(GL_LINES);
			glVertex3f(m_a[0], m_a[1], m_a[2]);
			glVertex3f(m_b[0], m_b[1], m_b[2]);

			glVertex3f(m_b[0], m_b[1], m_b[2]);
			glVertex3f(m_c[0], m_c[1], m_c[2]);
		glEnd();
	}
};