#pragma once

#include <cmath>

#include "particleConstraint.hpp"

class Follow: public ParticleConstraint {
private:
	float *m_src;
	float *m_dst;

	float m_speed;

public:
	Follow(float *src, float *dst, float speed) {
		m_src = src;
		m_dst = dst;

		m_speed = speed;
	}

	bool solve() {
		float dx = m_dst[0] - m_src[0];
		float dy = m_dst[1] - m_src[1];
		float dz = m_dst[2] - m_src[2];

		if(dx == 0 && dy == 0 && dz == 0) {
			dy = 1;
		}

		//break condition
		if(sqrt(dx*dx + dy*dy + dz*dz) < 1.2f) {
			return true;
		}

		m_src[0] += dx * m_speed;
		m_src[1] += dy * m_speed;
		m_src[2] += dz * m_speed;

		return false;
	}

	void render() {
		glColor3f(0.5f,0.5f,0);
		glBegin(GL_LINES);
			glVertex3f(m_src[0], m_src[1], m_src[2]);
			glVertex3f(m_dst[0], m_dst[1], m_dst[2]);
		glEnd();
	}
};

class Move: public ParticleConstraint {
private:
	float *m_a;

	float m_speed;
public: 
	Move(float *a, float speed) {
		m_a = a;
		m_speed = speed;
	}

	bool solve() {
		float dx = m_a[0] - m_a[3];
		float dy = m_a[1] - m_a[4];
		float dz = m_a[2] - m_a[5];

		if(dx == 0 && dy == 0 && dz == 0) {
			dy = 1;
		}

		float d = sqrt(dx*dx + dy*dy + dz*dz);
		float ds = 1/d;

		//normalize
		dx *= ds;
		dy *= ds;
		dz *= ds;

		dx += -5 + (rand()/(double(RAND_MAX)))*10;
		dy += -5 + (rand()/(double(RAND_MAX)))*10;
		dz += -5 + (rand()/(double(RAND_MAX)))*10;

		d = sqrt(dx*dx + dy*dy + dz*dz);
		ds = 1/d;		

		dx *= ds;
		dy *= ds;
		dz *= ds;

		m_a[0] += dx * m_speed;
		m_a[1] += dy * m_speed;
		m_a[2] += dz * m_speed;

		return false;
	}

	void render() {
		float dx = m_a[0] - m_a[3];
		float dy = m_a[1] - m_a[4];
		float dz = m_a[2] - m_a[5];

		glColor3f(0,1,0);
		glBegin(GL_LINES);
			// glVertex3f(m_a[3], m_a[4], m_a[5]);
			glVertex3f(m_a[0], m_a[1], m_a[2]);
			glVertex3f(m_a[0] + dx*5, m_a[1]+ dy*5, m_a[2]+ dz*5);
		glEnd();
	}
};

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