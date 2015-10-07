#pragma once

#include "particleConstraint.hpp"

class Attraction: public ParticleConstraint {
private:
	float *m_a; //particle
	float *m_b;

	float m_threshold;
	float m_speed;
public:

	Attraction(float *a, float *b, float speed, float threshold) {
		m_a = a;
		m_b = b;

		m_speed = speed;
		m_threshold = threshold;
	}

	//solve returns true if this constraint should be deleted
	bool solve() {
		//difference
		float dx = m_b[0] - m_a[0];
		float dy = m_b[1] - m_a[1];
		float dz = m_b[2] - m_a[2];

		float d = sqrt(dx*dx + dy*dy + dz*dz);

		if(d < m_threshold) {
			return true;
		}

		//translation
		float tx = dx * d/d * m_speed;
		float ty = dy * d/d * m_speed;
		float tz = dz * d/d * m_speed;

		//reassign
		m_a[0] += tx;
		m_a[1] += ty;
		m_a[2] += tz;

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

class Random: public ParticleConstraint {
private:
	float *m_a; //particle

	float m_speed;
public:

	Random(float *a, float speed) {
		m_a = a;

		m_speed = speed;
	}

	//solve returns true if this constraint should be deleted
	bool solve() {
		if((rand()/(double(RAND_MAX) + 1)) < 0.01f) {
			m_a[0] += (-1 + (rand()/(double(RAND_MAX) + 1))*2) * m_speed;
			m_a[1] += (-1 + (rand()/(double(RAND_MAX) + 1))*2) * m_speed;
			m_a[2] += (-1 + (rand()/(double(RAND_MAX) + 1))*2) * m_speed;
		}

		return false; //don't delete	
	}

	void render() {
		// glColor3f(0,1,0);
		// glBegin(GL_LINES);
		// 	glVertex3f(m_a[0], m_a[1], m_a[2]);
		// 	glVertex3f(m_b[0], m_b[1], m_b[2]);
		// glEnd();
	}
};