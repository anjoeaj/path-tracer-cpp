#ifndef RTUTILS_H
#define RTUTILS_H

#include<cmath>
#include<memory>

// Usings
using std::shared_ptr;
using std::sqrt;
using std::make_shared;

// Constants
const double pi = 3.1415926535897932385;
const double infinity = std::numeric_limits<double>::infinity();

// util funcs
inline double degrees_to_radians(double degrees){
	return degrees * pi / 180.0;
}

// Common headers
#include "vec3.h"
#include "ray.h"
#endif