#pragma once

#include <vector>

#include "comp308.hpp"
#include "particleConstraint.hpp"

class ParticleSystem {
protected:
	const uint NUM_COMPONENTS = 6;

	uint MAX_PARTICLES;
	uint m_particleCount;
	float *m_particles;

	uint m_constraintIterations;

	std::vector<ParticleConstraint*> m_constraints;

	float m_dragX = 0.98f;
	float m_dragY = 0.98f;
	float m_dragZ = 0.98f;
	float m_gravityX = 0.0f;
	float m_gravityY = 0;//-98.0f;
	float m_gravityZ = 0.0f;

	virtual void createConstraints() {

	}

	void integrateParticles(float delta);
	void solveConstraints(float delta);
public:
	ParticleSystem(uint maxParticles, uint constraintIterations) {
		MAX_PARTICLES = maxParticles;
		m_particleCount = 0;
		m_particles = new float[MAX_PARTICLES*NUM_COMPONENTS];

		m_constraintIterations = constraintIterations;
	}

	virtual ~ParticleSystem() {
		delete [] m_particles;
	}

	virtual void create() = 0;

	//creates a particle at (x,y,z)
	//returns true if successful
	//places a pointer to the particle into pointer
	bool createParticle(float x, float y, float z, float **pointer);

	void tick(float delta);

	virtual void render();
};