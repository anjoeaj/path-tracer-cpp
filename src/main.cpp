#include "rtutils.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <iostream>

double hit_sphere(const point3 &center, double radius, const ray &r)
{
	// this is a simplified version of th original hit_sphere by factoring out 2 from the equation
	// find out the discriminant
	vec3 origin_to_center = r.origin() - center;
	auto a = r.direction().len_squared(); // because a.a = len of the vec squared
	auto half_b = dot(r.direction(), origin_to_center);
	auto c = origin_to_center.len_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	// add the 2 quadratic eqn solutions for cases where disc < 0 and > 0
	if (discriminant < 0)
	{
		// sqrt of -ve num is not real so return -1.
		return -1.0;
	}
	else
	{
		return (-half_b - sqrt(discriminant)) / a;
	}
}
color ray_color(const ray &r, const hittable &world, int depth)
{
	hit_record rec;

	// when the max bounce is reached, stop gathering light
	if (depth <= 0)
	{
		return color(0, 0, 0);
	}

	if (world.hit(r, 0.001, infinity, rec))
	{
		// find out the new direction for the ray to bounce to next time
		ray scattered;
		color attenuation;

		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			// gather the colors recursively
			return attenuation * ray_color(scattered, world, depth - 1);
		}

		return color(0, 0, 0);
		// point3 target = rec.p + random_in_hemisphere(rec.normal);
		// return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
	}
	vec3 unit_dir = unit_vector(r.direction());
	auto t = (unit_dir.y() + 1.0) / 2.0;
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main()
{
	// Create Image dimensions
	const auto aspect_ratio = 16.0 / 9.0;
	const int width = 500;
	const int height = static_cast<int>(width / aspect_ratio);
	const int samples_per_pixel = 500;
	const int max_depth = 50;

	// Create the world
	hittable_list world;
	world = random_scene();

	//create materials
	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_shared<dielectric>(1.5);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));

	//interesting and easy trick with dielectric spheres is to note that if you use a negative radius,
	// the geometry is unaffected, but the surface normal points inward.
	// This can be used as a bubble to make a hollow glass sphere:
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1), 0.5, material_right));
	// world.add(make_shared<sphere>(point3(-0.25,0.5,-0.5), 0.25));
	// auto R = cos(pi/4);
	// // hittable_list world;

	// auto material_left  = make_shared<lambertian>(color(0,0,1));
	// auto material_right = make_shared<lambertian>(color(1,0,0));

	// world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
	// world.add(make_shared<sphere>(point3( R, 0, -1), R, material_right));
	//Camera details
	point3 look_from(13,2,3);
	point3 look_at(0,0,0);
	vec3 vup(0,1,0);
	auto dist_to_focus = 10;//(look_from - look_at).length();
	auto aperture = 0.1;
	auto fov = 20;
	camera cam(look_from, look_at, vup, fov, aspect_ratio, aperture, dist_to_focus); 

	// Now create a PPM format for storing the image data.
	// Check wikipedia for the format
	std::cout << "P3\n"
		  << width << " " << height << "\n255\n";

	// Put the pixel values into the subsequent lines.
	for (int j = height - 1; j >= 0; j--)
	{
		std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
		for (int i = 0; i < width; i++)
		{
			color pixel_color = color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++)
			{
				double u = (i + random_double()) / (width - 1);
				double v = (j + random_double()) / (height - 1);

				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone";
}