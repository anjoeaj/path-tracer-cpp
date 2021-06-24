#ifndef SPHERE_H
#define SPHERE_H

#include "rtutils.h"
#include "hittable.h"
class sphere : public hittable {
	public:
		sphere(){}
		sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m){}; 

		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
		
	public:
		point3 center;
		double radius;	
		shared_ptr<material> mat_ptr;
};
/*
	Ray sphere intersection is essentially solving the quadratic equation
	t^2 b⋅b+2tb⋅(A−C)+(A−C)⋅(A−C)−r^2=0 where t is the unknown

	

	equations derivation
	x2+y2+z2=R2 when sphere is at origin
	(x−Cx)2+(y−Cy)2+(z−Cz)2=r2 where C is the center of the sphere
	
	(P−C)⋅(P−C)=(x−Cx)2+(y−Cy)2+(z−Cz)2 => the dot product whwe
	(P−C)⋅(P−C)=r2
	where P is the point x,y,z and C is the center x,y,z

	from the ray equation we know that P(t) = A + tb where A is the origin of the ray and b is the direction
	substitute it in the above eqn
	(P(t)−C)⋅(P(t)−C)=r2
	(A+tb−C)⋅(A+tb−C)=r2
	t2b⋅b+2tb⋅(A−C)+(A−C)⋅(A−C)−r2=0

	Now this is a quadratic equation of format ax^2 + bx + c = 0
	find out the discriminant b^2 - 4ac

*/
// double hit_sphere(const point3& center, double radius, const ray& r){
// 	// find out the discriminant
// 	vec3 origin_to_center = r.origin() - center;
// 	auto a = dot(r.direction(), r.direction());
// 	auto b = 2.0 * dot(r.direction(),origin_to_center);
// 	auto c = dot(origin_to_center, origin_to_center) - radius*radius;
// 	auto discriminant = b*b - 4*a*c;

// 	// add the 2 quadratic eqn solutions for cases where disc < 0 and > 0
// 	if (discriminant < 0){
// 		// sqrt of -ve num is not real so return -1.
// 		return -1.0;
// 	} else{
// 		return (- b - sqrt(discriminant))/(2*a);
// 	}
// 	return (discriminant > 0);
// }
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 origin_to_center = r.origin() - center;
	auto a = r.direction().len_squared(); // because a.a = len of the vec squared 
	auto half_b = dot(r.direction(),origin_to_center);
	auto c = origin_to_center.len_squared() - radius*radius;
	auto discriminant = half_b * half_b - a*c;
	if (discriminant < 0){
		// sqrt of -ve num is not real so return false.
		return false;
	}
	auto sqrt_d = sqrt(discriminant); 

	auto root = (-half_b - sqrt_d) / a;
	if (root < t_min || root > t_max) {
		// find out the other root
		root = (-half_b + sqrt_d)/a;
		if (root < t_min || root > t_max){
			return false;
		}
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 outward_normal = (rec.p - center)/radius;
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;

	return true;
}
#endif