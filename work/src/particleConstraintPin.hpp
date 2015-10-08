#pragma once

#include "particleConstraint.hpp"

class ParticleConstraintPin: public ParticleConstraint {
private:
	float *m_a; //particle

	float m_x;
	float m_y;
	float m_z;
public:

	ParticleConstraintPin(float *particle) {
		m_a = particle;

		m_x = m_a[0];
		m_y = m_a[1];
		m_z = m_a[2];
	}

	ParticleConstraintPin(float *particle, float x, float y, float z) {
		m_a = particle;

		m_x = x;
		m_y = y;
		m_z = z;
	}

	//solve returns true if this constraint should be deleted
	bool solve() {
		m_a[0] = m_x;
		m_a[1] = m_y;
		m_a[2] = m_z;

		return false;
	}

	void render() {
		glColor3f(0,1,0);
		glBegin(GL_LINES);
			glVertex3f(m_x-0.5f, m_y-0.5f, m_z-0.5f);
			glVertex3f(m_x+0.5f, m_y+0.5f, m_z+0.5f);

			glVertex3f(m_x+0.5f, m_y-0.5f, m_z-0.5f);
			glVertex3f(m_x-0.5f, m_y+0.5f, m_z+0.5f);

			glVertex3f(m_x-0.5f, m_y-0.5f, m_z+0.5f);
			glVertex3f(m_x+0.5f, m_y+0.5f, m_z-0.5f);

			glVertex3f(m_x+0.5f, m_y-0.5f, m_z+0.5f);
			glVertex3f(m_x-0.5f, m_y+0.5f, m_z-0.5f);
		glEnd();
	}
};