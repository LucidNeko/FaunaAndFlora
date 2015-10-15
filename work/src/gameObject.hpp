#pragma once

#include <cmath>	
#include <vector>	

#include "comp308.hpp"
#include "IRenderable.hpp"
#include "ITickable.hpp"

using namespace comp308;
using namespace std;

class GameObject : public IRenderable, ITickable {
private:
	IRenderable *m_renderable;

	vector<GameObject *> m_children;

	float m_time = 0;
	float m_rotZ = 0;
public:
	float *m_particle;

	GameObject(float *particle, IRenderable *renderable) {
		m_particle = particle;
		m_renderable = renderable;
	};

	~GameObject() {
		for(uint i = 0; i <m_children.size(); i++) {
			delete m_children[i];
		}
	};

	void addChild(GameObject *child) {
		m_children.push_back(child);
	}

	virtual void tick(float delta) {
		for(uint i = 0; i < m_children.size(); i++) {
			m_children[i]->tick(delta);
		}

		if(m_children.size() == 0) {
			m_time += delta;
			m_rotZ = sin(m_time) * 30;
		}
	};

	virtual void render() {
		glPushMatrix();
			if(m_children.size() != 0) {
				glTranslatef(m_particle[0], m_particle[1], m_particle[2]);

				vec3 pos = vec3(m_particle[0], m_particle[1], m_particle[2]);
				vec3 oldPos = vec3(m_particle[3], m_particle[4], m_particle[5]);
				vec3 dir = normalize(pos - oldPos);

				//calculate rotation axis and angle
				vec3 axis = cross(vec3(0,0,1), dir);
				float angle = acos(dot(vec3(0,0,1), dir)/length(dir));

				//rotate
				glRotatef(angle * 180.0f / 3.14159265359f, axis.x, axis.y, axis.z);
				glScalef(0.1f, 0.1f, 0.1f);
			} else {
				glRotatef(m_rotZ, 0, 0, 1);
			}

			m_renderable->render();
		for(uint i = 0; i < m_children.size(); i++) {
			m_children[i]->render();
		}

		glPopMatrix();
	};
};