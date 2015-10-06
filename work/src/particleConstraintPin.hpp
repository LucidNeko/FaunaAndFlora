#pragma once

#include "particleConstraint.hpp"

class ParticleConstraintPin: public ParticleConstraint {
private:
	uint m_a; //particle

	float m_x;
	float m_y;
	float m_z;
public:

	ParticleConstraintPin(uint particle, float x, float y, float z) {
		m_a = particle;
	}

	//solve returns true if this constraint should be deleted
	bool solve(float *particles) {
		particles[m_a]   = m_x;
		particles[m_a+1] = m_y;
		particles[m_a+2] = m_z;

		return false;
	}

	void render(float *particles) {
		glColor3f(0,1,0);
		glBegin(GL_LINES);
			glVertex3f(m_x-1, m_y-1, m_z-1);
			glVertex3f(m_x+1, m_y+1, m_z+1);

			glVertex3f(m_x+1, m_y-1, m_z-1);
			glVertex3f(m_x-1, m_y+1, m_z+1);

			glVertex3f(m_x-1, m_y-1, m_z+1);
			glVertex3f(m_x+1, m_y+1, m_z-1);

			glVertex3f(m_x+1, m_y-1, m_z+1);
			glVertex3f(m_x-1, m_y+1, m_z-1);
		glEnd();
	}
};