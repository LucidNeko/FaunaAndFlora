#pragma once

class ParticleConstraint {
protected:
	float m_time = 0;
	float m_lifetime = 0;
public:
	//solve returns true if this constraint should be deleted
	virtual bool solve() = 0;
	virtual void render() = 0;

	void tick(float delta) {
		m_time += delta;
	};

	void setLifetime(float lifetime) {
		m_lifetime = lifetime;
	};
};