#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "comp308.hpp"

class Lights{
private:
	void pointLight();
	void spotLight();
	void directionLight();
	void ambientLight();
public:
	Lights();
};