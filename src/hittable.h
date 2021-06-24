#ifndef HITTABLE_H
#define HITTABLE_H

// #include "ray.h"
#include "rtutils.h"
class material;

struct hit_record{
	point3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	double t;
	bool front_face;

	// lets define a standard that our normals always point outwards of the surface.
	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		// If the ray and the normal face in the same direction, the ray is inside the object, 
		// if the ray and the normal face in the opposite direction, then the ray is outside the object.
		// This can be determined by taking the dot product of the two vectors, where if their dot is positive, the ray is inside the sphere.
		front_face = dot(r.direction(), outward_normal) < 0.0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
	public:
		virtual bool hit(const ray& ray, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif