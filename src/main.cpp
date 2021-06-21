#include "color.h"
#include "vec3.h"

#include <iostream>

int main(){
	// Create Image dimensions
	const int height = 256;
	const int width = 256;
	
	// Now create a PPM format for storing the image data.
	// Check wikipedia for the format
	std::cout <<"P3\n"<<width<<" "<<height<<"\n255\n";

	// Put the pixel values into the subsequent lines.
	for (int j = height - 1; j >= 0; j--){
		std::cerr << "\rScanlines remaining: "<<j << " " <<std::flush;
		for (int i = 0; i < width ; i ++){
			double r = (double)i/(width - 1);
			double g = (double)j/(height - 1);
			double b = 0.25;

			color pixel_color(r,g,b);
			write_color(std::cout, pixel_color);
		}
	}
	std::cerr <<"\nDone";



}