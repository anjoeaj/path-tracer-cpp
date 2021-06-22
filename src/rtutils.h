#ifndef RTUTILS_H
#define RTUTILS_H

#include <cmath>
#include <memory>
#include <cstdlib>
#include <random>
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

// inline double random_double(){
// 	// rand num [0,1)
// 	return rand()/(RAND_MAX + 1.0);
// }

inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0.0,1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(double min, double max){
	// rand num in [min,max)
	return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max){
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

// Common headers
#include "vec3.h"
#include "ray.h"
#endif