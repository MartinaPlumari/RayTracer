#pragma once

#include <iostream>
#include "vec3.h"


using color = vec3;

void write_color_to_file(std::ostream& out, const color& pixel_color)
{
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	//translate the [0,1] component values to the byte range [0, 255]
	int rbyte = int(255.999 * r);
	int gbyte = int(255.999 * g);
	int bbyte = int(255.999 * b);

	//writes out the pixel color components
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

void write_color_to_matrix(const color& pixel_color, uint8_t* pixels, uint32_t* id)
{
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	//translate the [0,1] component values to the byte range [0, 255]
	pixels[(*id)++] = static_cast<uint8_t> (255.999*r);
	pixels[(*id)++] = static_cast<uint8_t> (255.999*g);
	pixels[(*id)++] = static_cast<uint8_t> (255.999*b);

}