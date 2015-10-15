#pragma once

class ITickable {
public:
	virtual ~ITickable() {}
	virtual void tick(float delta) = 0;
};