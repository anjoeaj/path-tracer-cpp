#include "rtutils.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"


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
color ray_color(const ray& r, const  hittable& world){
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {
		return (rec.normal + 1) / 2.0;	
	}
	vec3 unit_dir = unit_vector(r.direction());
	auto t = (unit_dir.y() + 1.0 ) / 2.0;
	return (1.0 - t) * color(1.0,1.0,1.0) + t*color(0.5,0.7,1.0);
}
int main(){
	// Create Image dimensions
	const auto aspect_ratio = 16.0/9.0;
	const int width = 400;
	const int height = static_cast<int> (width/aspect_ratio);

	// Create the world
	hittable_list world;
	world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

	//Camera details
	// the viewport is essentially the projection plane
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * aspect_ratio;
	auto focal_length = 1.0; // the length from camera to the projection plane

	auto origin = point3(0,0,0);
	auto horizontal = vec3(viewport_width,0,0);
	auto vertical = vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0,0,focal_length);
	
	// Now create a PPM format for storing the image data.
	// Check wikipedia for the format
	std::cout <<"P3\n"<<width<<" "<<height<<"\n255\n";

	// Put the pixel values into the subsequent lines.
	for (int j = height - 1; j >= 0; j--){
		std::cerr << "\rScanlines remaining: "<<j << " " <<std::flush;
		for (int i = 0; i < width ; i ++){
			double u = (double)i/(width - 1);
			double v = (double)j/(height - 1);


			ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
			color pixel_color = ray_color(r, world);
			write_color(std::cout, pixel_color);
		}
	}
	std::cerr <<"\nDone";



}