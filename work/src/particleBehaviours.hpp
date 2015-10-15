#pragma once

#include <cmath>

#include "particleConstraint.hpp"

class Pot: public ParticleConstraint {
private:
	float *m_particle;
	float m_minY = -9.9f;
	float m_maxY = -6.9f;
	float box = 1.5f;
public:
	Pot(float *particle) {
		m_particle = particle;
	}

	bool solve() {
		float x = m_particle[0];
		float y = m_particle[1];
		float z = m_particle[2];

		
//v1
		// if(x > -box && x < box && z > -box && z < box && y > m_minY && y < m_maxY) {
		// 	float tx = m_particle[0];
		// 	float ty = m_particle[1];
		// 	float tz = m_particle[2];

		// 	// m_particle[0] = m_particle[3];
		// 	// m_particle[1] = m_particle[4];
		// 	// m_particle[2] = m_particle[5];

		// 	// m_particle[3] = tx;
		// 	// m_particle[4] = ty;
		// 	// m_particle[5] = tz;

		// 	float absx = abs(tx);
		// 	float absy = abs(ty);
		// 	float absz = abs(tz);

		// 	//x vs yz
		// 	if(absx > absy && absx > absz) {

		// 	}
		// }

//v2
		// if(x > -box && x < box && z > -box && z < box && y > m_minY && y < m_maxY) {
		// 	//we are in the box
		// 	float x1 = x - -box;
		// 	float x2 = x - box;
		// 	float nx = abs(x1) > abs(x2) ? x1 : x2; 

		// 	float y1 = y - m_minY;
		// 	float y2 = y - m_maxY;
		// 	float ny = abs(y1) > abs(y2) ? y1 : y2;

		// 	float z1 = z - -box;
		// 	float z2 = z - box;
		// 	float nz = abs(z1) > abs(z2) ? z1 : z2;

		// 	float absnx = abs(nx);
		// 	float absny = abs(ny);
		// 	float absnz = abs(nz);

		// 	if(absnx < absny && absnx < absnz) {
		// 		m_particle[0] = m_particle[3] = nx;
		// 	} else if(absny < absnx && absny < absnz) {
		// 		m_particle[1] = m_particle[4] = ny;
		// 	} else if(absnz < absnx && absnz < absny) {
		// 		m_particle[2] = m_particle[5] = nz;
		// 	}
		// }

		return false;
	};

	void render() {

	}
};

class Ground: public ParticleConstraint {
private:
	float *m_particle;
	float m_minY = -9.9f;
	//float m_maxY = -11.5f; //fly out through hanging cloth
	float m_maxY = -13.5f;
	float box = 3.5f * 1.75f; //1.667f because scale of cloth
public:
	Ground(float *particle) {
		m_particle = particle;
	}

	bool solve() {
		float x = m_particle[0];
		float y = m_particle[1];
		float z = m_particle[2];

		if(x > -box && x < box && z > -box && z < box && y < m_minY && y > m_maxY) {
		//in box
			if(abs(y-m_minY) < abs(y-m_maxY)) {
				m_particle[1] = m_minY;
				m_particle[4] = m_minY;
			} else {
				m_particle[1] = m_maxY;
				m_particle[4] = m_maxY;
			}
		}

		return false;
	};

	void render() {

	}
};

class Wind: public ParticleConstraint {
private:
	float *m_p;
	float *m_windDir;

	float m_speed;

public:
	Wind(float *particle, float *windDir, float speed) {
		m_p = particle;
		m_windDir = windDir;

		m_speed = speed;
	}

	bool solve() {
		m_p[0] += m_windDir[0] * m_speed;
		m_p[1] += m_windDir[1] * m_speed;
		m_p[2] += m_windDir[2] * m_speed;

		return false;
	}

	void render() {
		glColor3f(0.5f,0.5f,0);
		glBegin(GL_LINES);
			glVertex3f(m_p[0], m_p[1], m_p[2]);
			glVertex3f(m_p[0] + m_windDir[0], m_p[1] + m_windDir[1], m_p[2] + m_windDir[2]);
		glEnd();
	}
};

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
		//kill off from time
		if(m_lifetime != 0 && m_time >= m_lifetime) {
			return true;
		}

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