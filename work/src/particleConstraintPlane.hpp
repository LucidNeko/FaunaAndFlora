#pragma once

#include "particleConstraint.hpp"

class ParticleConstraintPlane: public ParticleConstraint {
private:
	float *m_a; //particle

	bool m_cx;
	bool m_cy;
	bool m_cz;

	float m_x;
	float m_y;
	float m_z;
public:

	ParticleConstraintPlane(float *particle, bool x, bool y, bool z) {
		m_a = particle;

		m_cx = x;
		m_cy = y;
		m_cz = z;

		m_x = m_a[0];
		m_y = m_a[1];
		m_z = m_a[2];
	}

	//solve returns true if this constraint should be deleted
	bool solve() {
		if(m_cx) {
			m_a[0] = m_x;
		}

		if(m_cy) {
			m_a[1] = m_y;
		}

		if(m_cz) {
			m_a[2] = m_z;
		}

		return false;
	}

	void render() {
		
	}
};