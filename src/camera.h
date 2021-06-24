#ifndef CAMERA_H
#define CAMERA_H

#include "rtutils.h"
class camera
{
public:
	camera(
	    point3 look_from,
	    point3 look_at,
	    vec3 vup,	 // up-vector
	    double vfov, //the vertical field of view in degrees
	    double aspect_ratio,
	    double aperture,
	    double focus_dist)
	{
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = viewport_height * aspect_ratio;

		// Calculate the orthonormal basis (u,v,w) to describe the camera orientation
		w = unit_vector(look_from - look_at);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		origin = look_from;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

		lens_radius = aperture / 2;
	}

	ray get_ray(double s, double t)
	{
		vec3 rand_point_on_lens = lens_radius * random_in_unit_disk();
		vec3 offset = u * rand_point_on_lens.x() + v * rand_point_on_lens.y();
		vec3 perturbed_origin = origin + offset;
		return ray(perturbed_origin, lower_left_corner + s * horizontal + t * vertical - perturbed_origin);
	}

private:
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	double lens_radius;
};
#endif