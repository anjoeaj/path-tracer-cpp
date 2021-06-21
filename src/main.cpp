#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

color ray_color(const ray& r){
	vec3 unit_dir = unit_vector(r.direction());
	auto t = (unit_dir.y() + 1.0 ) / 2.0;
	return (1.0 - t) * color(1.0,1.0,1.0) + t*color(0.5,0.7,1.0);
}

int main(){
	// Create Image dimensions
	const auto aspect_ratio = 16.0/9.0;
	const int width = 400;
	const int height = static_cast<int> (width/aspect_ratio);

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
			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}
	std::cerr <<"\nDone";



}