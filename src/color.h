#ifndef COLOR_H
#define COLOR_H

#include "rtutils.h"

void write_color(std::ostream &out, color pixel_color, double samples_per_pixel){
	pixel_color = pixel_color/samples_per_pixel;
	auto r = clamp(pixel_color.r(), 0.0, 0.999);
	auto g = clamp(pixel_color.g(), 0.0, 0.999);
	auto b = clamp(pixel_color.b(), 0.0, 0.999);
	// cas the float to int and write to the stream
	out << static_cast<int>(255.999 * r) << " "
		<< static_cast<int>(255.999 * g) << " "
		<< static_cast<int>(255.999 * b) << "\n";
}

#endif