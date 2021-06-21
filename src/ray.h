#include "vec3.h"

class ray{
	public:
		ray(){}

		// to initialize member variables before the body of the constructor
		// weird but ok.
		ray(const point3& origin, const vec3& direction)
			: orig(origin), dir(direction){}

		// get origin
		point3 origin() const{return orig;}

		// get dir
		vec3 direction() const {return dir;}

		// This func returns thr point along this ray at t steps
		point3 at(double t) const{
			return orig + t * dir;
		}

	public:
		point3 orig;
		vec3 dir;
};