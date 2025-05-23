#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

//Constants
const double inf = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility functions
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

//Common headers

#include "ray.h"
#include "vec3.h"
#include "color.h"
#include "interval.h"