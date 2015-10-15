#pragma once

class IRenderable {
public:
	virtual ~IRenderable() {}
	virtual void render() = 0;
};