// RayTracer.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include <stdint.h>
#include <vector>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

#include "include/color.h"
#include "include/vec3.h"
#include "include/ray.h"
#include "include/sphere.h"

#define N_CHANNELS 3

typedef enum {
	PNG = 0,
	PPM = 1
} format_t;

color ray_color(const ray& r);

int main()
{
	/* setting image dimensions */
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;
	int image_height = int(image_width / aspect_ratio);
	//safety check: the height should always be at least 1
	image_height = (image_height < 1) ? 1 : image_height;

	/* camera settings */
	auto focal_length = 1.0;
	auto camera_center = point3(0, 0, 0);
	/* setting viewport dimensions */
	/* we don't use aspect ratio because we need the real aspect ratio, not the ideal one. The proportions of the viewport have to be exactly the same as the image */
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(image_width) / image_height);

	/* calculate the vectors across the horizontal and down the vertical viewport edges (V_u and V_v)*/
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);

	/* calculate the horizontal and vertical delta vectors from pixel to pixel */
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	/* calculate the location of the upper left pixel -> the camera looks towards the negative side of z */
	auto viewport_upper_left = camera_center 
							   - vec3(0, 0, focal_length) -viewport_u/2 - viewport_v/2;
	auto pixel00_location = viewport_upper_left 
							+ (pixel_delta_u + pixel_delta_v) * 0.5; 

	/* render */
	//ti prego metti questa roba in una funzione
	uint8_t* pixels = new uint8_t[image_width * image_height * N_CHANNELS];
	uint32_t id = 0;
	format_t format = PNG;

	if(format == PPM)
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n" << "\n255\n";

	for (int j = 0; j < image_height; j++) {
		if (format == PPM)
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; i++)
		{
			auto pixel_center = pixel00_location + (i * pixel_delta_u) + (j * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r);

			if (format == PPM)
				write_color_to_file(std::cout, pixel_color);
			else if (format == PNG)
				write_color_to_matrix(pixel_color, pixels, &id);
		}
	}

	if (format == PPM)
		std::clog << "\rDone.                  " << std::endl;

	else if (format == PNG)
	{
		int retVal = stbi_write_png("image.png", image_width, image_height, N_CHANNELS, pixels, image_width * N_CHANNELS);
		if (retVal == 0)
		{
			std::cerr << "Error in saving png image" << std::endl;
			return -1;
		}
	}

}

//function that returns the color for a given scene ray
color ray_color(const ray& r)
{
	point3 sphere_center(0.0, 0.0, -1.0);
	sphere s(sphere_center, 0.5);
	std::vector<sphere> s_v = { s, sphere(point3(-0.8, 0.0,-1.0), 0.3), sphere(point3(0.8, 0.0,-1.0), 0.3) };

	//if t<0 we don't do anything because the sphere would be behind the camera
	for (auto s : s_v) {
		hit_record rec;
		if (s.hit(r, 0.0, 100.0, rec)) {
			return 0.5 * color(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
		}
	}
	
	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0); // 0<a<1 the variation is due to the modification of the y component
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
