#pragma once

#include "particleConstraint.hpp"
#include <cmath>

class ParticleConstraintDistance: public ParticleConstraint {
private:
	//particles
	uint m_a; 
	uint m_b;

	float m_restingDistance;
	float m_tolerance;
public:

	ParticleConstraintDistance(uint a, uint b, float restingDistance, float tolerance) {
		m_a = a;
		m_b = b;
		m_restingDistance = restingDistance;
		m_tolerance = tolerance;
	}

	//solve returns true if this constraint should be deleted
	bool solve(float *particles) {
		float ax = particles[m_a];
		float ay = particles[m_a+1];
		float az = particles[m_a+2];

		float bx = particles[m_b];
		float by = particles[m_b+1];
		float bz = particles[m_b+2];

		//difference
		float dx = ax - bx;
		float dy = ay - by;
		float dz = az - bz;
		float d = sqrt(dx*dx + dy*dy + dz*dz);

		float t = m_restingDistance - m_tolerance;

		//difference as scaler
		// if(d == 0) {
		// 	d = m_restingDistance;
		// } else {
		// 	d = (m_restingDistance - d) / d;
		// }

		if(d < 0.01f) {
			d = 0;
		} else {
			d = (m_restingDistance - d) / d;
		}

		//translation
		float tx = dx * 0.5f * d * 0.01f;
		float ty = dy * 0.5f * d * 0.01f;
		float tz = dz * 0.5f * d * 0.01f;

		//reassign
		particles[m_a]   = ax + tx;
		particles[m_a+1] = ay + ty;
		particles[m_a+2] = az + tz;

		particles[m_b]   = bx - tx;
		particles[m_b+1] = by - ty;
		particles[m_b+2] = bz - tz;

		return false; //don't delete
	}

	void render(float *particles) {
		float ax = particles[m_a];
		float ay = particles[m_a+1];
		float az = particles[m_a+2];

		float bx = particles[m_b];
		float by = particles[m_b+1];
		float bz = particles[m_b+2];

		glColor3f(0,1,0);
		glBegin(GL_LINES);
			glVertex3f(ax, ay, az);
			glVertex3f(bx, by, bz);
		glEnd();
	}
};