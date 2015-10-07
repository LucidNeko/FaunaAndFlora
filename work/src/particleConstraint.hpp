#pragma once

class ParticleConstraint {
private:

public:
	//solve returns true if this constraint should be deleted
	virtual bool solve() = 0;
	virtual void render() = 0;
};