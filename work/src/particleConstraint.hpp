#pragma once

class ParticleConstraint {
private:

public:
	//solve returns true if this constraint should be deleted
	virtual bool solve(float *particles) = 0;
	virtual void render(float *particles) = 0;
};