#pragma once

#include <vector>

#include "comp308.hpp"
#include "particleConstraint.hpp"

class ParticleSystem {
private:
	const uint MAX_PARTICLES = 50;
	float *particles = new float[MAX_PARTICLES*6];
	std::vector<ParticleConstraint*> constraints;

	void integrateParticles(float delta);
public:
	ParticleSystem();
	~ParticleSystem() {
		delete [] particles;
	}

	void tick(float delta);

	void render();
};