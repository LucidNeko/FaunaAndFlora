#pragma once

#include <cmath>
#include <vector>

#include "comp308.hpp"
#include "particleSystem.hpp"
#include "particleConstraint.hpp"
#include "particleConstraintDistance.hpp"
#include "particleConstraintAngle.hpp"
#include "particleConstraintPin.hpp"
#include "particleConstraintPlane.hpp"
#include "particleBehaviours.hpp"
#include "OBJLoader.hpp"

using namespace std;

struct vert {
	float *p;
	// int t = 0;
	float *n;
};

struct tri {
	vert v[3];
};

class OBJParticleSystem: public ParticleSystem {
private:
	OBJLoader *m_obj;

	float *m_wind = new float[3];

	std::vector<tri> m_tris;
public:
	OBJParticleSystem(OBJLoader *obj, uint maxParticles, uint constraintIterations) : ParticleSystem(maxParticles, constraintIterations) {
		m_obj = obj;
	}

	void create() {

		m_wind[0] = 0;
		m_wind[1] = -0.05f;
		m_wind[2] = 0;

		m_gravityY = -98.0f;

		float scale = m_obj->getScale();

		for(uint i = 1; i < m_obj->m_points.size(); i++) {
			
			uint idx = m_particleCount * NUM_COMPONENTS;
			
			m_particles[idx]   = m_obj->m_points[i].x * scale;
			m_particles[idx+1] = m_obj->m_points[i].y * scale;
			m_particles[idx+2] = m_obj->m_points[i].z * scale;

			m_particles[idx+3] = m_obj->m_points[i].x * scale;
			m_particles[idx+4] = m_obj->m_points[i].y * scale;
			m_particles[idx+5] = m_obj->m_points[i].z * scale;

			m_particleCount++;
		}

		for(uint i = 0; i < m_obj->m_triangles.size(); i++) {
			tri t;

			uint idx1 = m_obj->m_triangles[i].v[0].p-1; //-1 for 0 start idx
			uint idx2 = m_obj->m_triangles[i].v[1].p-1; //-1 for 0 start idx
			uint idx3 = m_obj->m_triangles[i].v[2].p-1; //-1 for 0 start idx

			vert v1; v1.p = &m_particles[idx1 * NUM_COMPONENTS];
			vert v2; v2.p = &m_particles[idx2 * NUM_COMPONENTS];
			vert v3; v3.p = &m_particles[idx3 * NUM_COMPONENTS];

			t.v[0] = v1;
			t.v[1] = v2;
			t.v[2] = v3;

			// for(uint j = 0; j < 3; j++) {
			// 	float dx = t.v[j].p[0] - t.v[(j+1)%3].p[0];
			// 	float dy = t.v[j].p[1] - t.v[(j+1)%3].p[1];
			// 	float dz = t.v[j].p[2] - t.v[(j+1)%3].p[2];
			// 	float d = sqrt(dx*dx + dy*dy + dz*dz);

			// 	m_constraints.push_back(new ParticleConstraintDistance(t.v[j].p, t.v[(j+1)%3].p, d*1.5f, 0.5f, false));				
			// }

			m_constraints.push_back(new ParticleConstraintDistance(v1.p, v2.p, 0.9f, false));
			m_constraints.push_back(new ParticleConstraintDistance(v2.p, v3.p, 0.9f, false));
			m_constraints.push_back(new ParticleConstraintDistance(v3.p, v1.p, 0.9f, false));

			t.v[0].n = &(m_obj->m_normals[m_obj->m_triangles[i].v[0].n].x);
			t.v[1].n = &(m_obj->m_normals[m_obj->m_triangles[i].v[1].n].x);
			t.v[2].n = &(m_obj->m_normals[m_obj->m_triangles[i].v[2].n].x);

			m_tris.push_back(t);
		}

		float *p;
		createParticle(0,0,0, &p); //pin

		for(uint i = 0; i < m_particleCount-1; i++) {
			float *particle = &m_particles[i*NUM_COMPONENTS];
			if(particle[1] >= -0.1f) {
				m_constraints.push_back(new ParticleConstraintPin(particle));
			} else {
				m_constraints.push_back(new Wind(particle, m_wind, 0.005f));
			}
		}

		// {
		// 	float *a = &m_particles[(84) * NUM_COMPONENTS];
		// 	float *b = &m_particles[(572) * NUM_COMPONENTS];
		// 	float *c = &m_particles[(272) * NUM_COMPONENTS];

		// 	// m_constraints.push_back(new ParticleConstraintDistance(a,b,0.8f, false));
		// }



		// for(uint i = 0; i < m_particleCount-1; i++) {

		// 	float *a = p;
		// 	float *b = &m_particles[i*NUM_COMPONENTS];

		// 	float dx = a[0] - b[0];
		// 	float dy = a[1] - b[1];
		// 	float dz = a[2] - b[2];
		// 	float d = sqrt(dx*dx + dy*dy + dz*dz);

		// 	m_constraints.push_back(new ParticleConstraintDistance(p, &m_particles[i * NUM_COMPONENTS], d*1.5f, 0.001f, false));
		// }

		// createParticle(  0, 0, 0, &m_pin);
		// m_constraints.push_back(new ParticleConstraintPin(m_pin));

		// for(uint i = 0; i < 50; i++) {
		// 	float *p;
		// 	createParticle(-5 + (rand()/(double(RAND_MAX) + 1))*10, 
		// 				   -5 + (rand()/(double(RAND_MAX) + 1))*10,
		// 				   -5 + (rand()/(double(RAND_MAX) + 1))*10, &p);

		// 	m_constraints.push_back(new Move(p, 0.001f));
		// }

		cout << "Created " << m_particleCount << " particles." << endl;
	}

	void createConstraints() {

		float x = m_wind[0];
		float y = m_wind[1];
		float z = m_wind[2];

		x += -0.4f + (rand()/(double(RAND_MAX) + 1)) *0.8f;
		y = -0.2f + (rand()/(double(RAND_MAX) + 1)) *0.2f;
		z += -0.4f + (rand()/(double(RAND_MAX) + 1)) *0.8f;

		if(x == 0 && y == 0 && z == 0) {
			x = 1;
		}

		float d = 1.f/sqrt(x*x + y*y + z*z);

		x *= d;
		y *= d;
		z *= d;

		m_wind[0] = x;
		m_wind[1] = y;
		m_wind[2] = z;


		// float r = (rand()/(double(RAND_MAX) + 1));
		// if(r < 0.2f) {

		// 	float *p = &m_particles[int(r*m_particleCount) * NUM_COMPONENTS];

		// 	r = -1 + (rand()/(double(RAND_MAX) + 1)) *2;

		// 	p[0] += r*1;
		// 	p[1] += r*1;
		// 	p[2] += r*1;
		// }
	}

	void render() {
		glBegin(GL_TRIANGLES);
		for(uint i = 0; i < m_tris.size(); i++) {
			tri t = m_tris[i];

			//Get the 3 points of this triangle
			float *a = t.v[0].p;
			float *b = t.v[1].p;
			float *c = t.v[2].p;

			float *an = t.v[0].n;
			float *bn = t.v[1].n;
			float *cn = t.v[2].n;

			glNormal3f(an[0], an[1], an[2]);
			glVertex3f(a[0], a[1], a[2]);
			glNormal3f(bn[0], bn[1], bn[2]);
			glVertex3f(b[0], b[1], b[2]);
			glNormal3f(cn[0], cn[1], cn[2]);
			glVertex3f(c[0], c[1], c[2]);
			
		}
		glEnd();
	}
};