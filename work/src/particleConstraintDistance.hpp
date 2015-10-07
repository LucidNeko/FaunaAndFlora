#pragma once

#include "particleConstraint.hpp"
#include <cmath>

class ParticleConstraintDistance: public ParticleConstraint {
private:
	//particles
	float *m_a; 
	float *m_b;

	float m_restingDistance;
	float m_softness;
public:

	ParticleConstraintDistance(float *a, float *b, float restingDistance, float softness) {
		m_a = a;
		m_b = b;
		m_restingDistance = restingDistance;
		m_softness = softness;
	}

	//solve returns true if this constraint should be deleted
	bool solve() {
		//difference
		float dx = m_a[0] - m_b[0];
		float dy = m_a[1] - m_b[1];
		float dz = m_a[2] - m_b[2];
		float d = sqrt(dx*dx + dy*dy + dz*dz);

		if(d < 0.01f) {
			d = 0;
		} else {
			d = (m_restingDistance - d) / d;
		}

		//translation
		float tx = dx * 0.5f * d * m_softness;
		float ty = dy * 0.5f * d * m_softness;
		float tz = dz * 0.5f * d * m_softness;

		//reassign
		m_a[0] += tx;
		m_a[1] += ty;
		m_a[2] += tz;

		m_b[0] -= tx;
		m_b[1] -= ty;
		m_b[2] -= tz;

		return false; //don't delete
	}

	void render() {
		glColor3f(0,1,0);
		glBegin(GL_LINES);
			glVertex3f(m_a[0], m_a[1], m_a[2]);
			glVertex3f(m_b[0], m_b[1], m_b[2]);
		glEnd();
	}
};