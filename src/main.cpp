#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>



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
bool hit_sphere(const point3& center, double radius, const ray& r){
	// find out the discriminant
	vec3 origin_to_center = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(r.direction(),origin_to_center);
	auto c = dot(origin_to_center, origin_to_center) - radius*radius;
	auto discriminant = b*b - 4*a*c;
	// std::cerr << "Discriminant - " << discriminant << "\n";
	return (discriminant > 0);
}
color ray_color(const ray& r){
	if(hit_sphere(point3(0,0,-1),0.5, r)){
		// std::cerr << "HITTT";
		return color(1,0,0);
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