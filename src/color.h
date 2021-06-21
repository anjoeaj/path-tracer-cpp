#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

void write_color(std::ostream &out, color pixel_color){
	// cas the float to int and write to the stream
	out << static_cast<int>(255.999 * pixel_color.x()) << " "
		<< static_cast<int>(255.999 * pixel_color.y()) << " "
		<< static_cast<int>(255.999 * pixel_color.z()) << "\n";
}

#endif