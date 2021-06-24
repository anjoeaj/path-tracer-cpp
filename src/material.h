#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtutils.h"
#include "hittable.h"
// struct hit_record;

class material
{
public:
	virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
public:
	lambertian(const color &a) : albedo(a) {}

	virtual bool scatter(
		const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
	{
		auto scatter_direction = rec.normal + random_unit_vector();

		// check for zero vec3
		if (scatter_direction.near_zero())
		{
			//If the random unit vector we generate is exactly opposite the normal vector,
			// the two will sum to zero, which will result in a zero scatter direction vector.
			scatter_direction = rec.normal;
		}

		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

public:
	color albedo;
};

class metal : public material
{
public:
	metal(const color &a, const double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
	{
		auto scatter_direction = reflect(unit_vector(r_in.direction()), rec.normal);
		attenuation = albedo;
		scattered = ray(rec.p, scatter_direction + fuzz * random_in_unit_sphere());
		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	color albedo;
	double fuzz;
};

class dielectric : public material
{
public:
	dielectric(const double i) : ir(i) {}

	virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
	{
		attenuation = color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir; // air refr index is 1.0

		vec3 unit_direction = unit_vector(r_in.direction());

		// to handle total internal reflection
		// sinθ=√(1−cos^2θ)
		// cosθ=R⋅n
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 new_direction;
		if (cannot_refract || schlick_appx(cos_theta, refraction_ratio) > random_double())
		{
			// must reflect
			new_direction = reflect(unit_direction, rec.normal);
		}
		else
		{
			// must refract
			new_direction = refract(unit_vector(r_in.direction()), rec.normal, refraction_ratio);
		}
		scattered = ray(rec.p, new_direction);
		return true;
	}

public:
	double ir; // index of refraction

private:
	static double schlick_appx(double cosine, double refr_ratio)
	{
		// Schlick's approximation for reflection - to handle reflection at steep angles
		auto r0 = (1 - refr_ratio) / (1 + refr_ratio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};
#endif