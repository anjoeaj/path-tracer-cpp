#include "rtutils.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r){
	// this is a simplified version of th original hit_sphere by factoring out 2 from the equation
	// find out the discriminant
	vec3 origin_to_center = r.origin() - center;
	auto a = r.direction().len_squared(); // because a.a = len of the vec squared 
	auto half_b = dot(r.direction(),origin_to_center);
	auto c = origin_to_center.len_squared() - radius*radius;
	auto discriminant = half_b * half_b - a*c;

	// add the 2 quadratic eqn solutions for cases where disc < 0 and > 0
	if (discriminant < 0){
		// sqrt of -ve num is not real so return -1.
		return -1.0;
	} else{
		return (-half_b - sqrt(discriminant))/a;
	}

}
color ray_color(const ray& r, const  hittable& world, int depth){
	hit_record rec;

	// when the max bounce is reached, stop gathering light
	if (depth <= 0) {
		return color(0,0,0);
	}

	if (world.hit(r, 0.001, infinity, rec)) {
		// find out the new random direction for the ray to bounce to next time
		point3 target = rec.p + random_in_hemisphere(rec.normal);
		return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
	}
	vec3 unit_dir = unit_vector(r.direction());
	auto t = (unit_dir.y() + 1.0 ) / 2.0;
	return (1.0 - t) * color(1.0,1.0,1.0) + t*color(0.5,0.7,1.0);
}
int main(){
	// Create Image dimensions
	const auto aspect_ratio = 16.0/9.0;
	const int width = 300;
	const int height = static_cast<int> (width/aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

	// Create the world
	hittable_list world;
	world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
	// world.add(make_shared<sphere>(point3(-0.25,0.5,-0.5), 0.25));
	//Camera details
	camera cam;
	
	// Now create a PPM format for storing the image data.
	// Check wikipedia for the format
	std::cout <<"P3\n"<<width<<" "<<height<<"\n255\n";

	// Put the pixel values into the subsequent lines.
	for (int j = height - 1; j >= 0; j--){
		std::cerr << "\rScanlines remaining: "<<j << " " <<std::flush;
		for (int i = 0; i < width ; i ++){
			color pixel_color = color(0,0,0);
			for (int s = 0; s < samples_per_pixel; s++){
				double u = (i + random_double()) / (width - 1);
				double v = (j + random_double()) / (height - 1);

				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
		write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr <<"\nDone";



}