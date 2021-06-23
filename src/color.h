#ifndef COLOR_H
#define COLOR_H

#include "rtutils.h"

void write_color(std::ostream &out, color pixel_color, double samples_per_pixel){
	pixel_color = pixel_color/samples_per_pixel;
	auto r = pixel_color.r();
	auto g = pixel_color.g();
	auto b = pixel_color.b();

	// gamma correct the pixels. Use gamma 2
	r = sqrt(r);
	g = sqrt(g);
	b = sqrt(b);

	// clamp between [0,1)
	r = clamp(r, 0.0, 0.999);
	g = clamp(g, 0.0, 0.999);
	b = clamp(b, 0.0, 0.999);
	// cas the float to int and write to the stream
	out << static_cast<int>(255.999 * r) << " "
		<< static_cast<int>(255.999 * g) << " "
		<< static_cast<int>(255.999 * b) << "\n";
}

#endif