#pragma once

/*									CAMERA CLASS									*/		
/*	Responsible for the construction and dispatch of rays into the world			*/
/*  The class then uses the results of these rays to construct the rendered image   */

#include "hittable.h"

typedef enum {
	PNG = 0,
	PPM = 1
} format_t;

class camera {

public:
	/* Public camera params */
	double	 aspect_ratio = 1.0;
	int		 image_width = 100;			//rendered image width in pixel count
	format_t format = PNG;

	int render(const hittable& world) 
	{
		initialize();

		uint8_t* pixels = new uint8_t[image_width * image_height * n_channels];
		uint32_t id = 0;
		format_t format = PNG;

		if (format == PPM)
			std::cout << "P3\n" << image_width << ' ' << image_height << "\n" << "\n255\n";

		for (int j = 0; j < image_height; j++) {
			if (format == PPM)
				std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; i++)
			{
				auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
				auto ray_direction = pixel_center - center;
				ray r(center, ray_direction);

				color pixel_color = ray_color(r, world);

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
			int retVal = stbi_write_png("output/image.png", image_width, image_height, n_channels, pixels, image_width * n_channels);
			if (retVal == 0)
			{
				std::cerr << "Error in saving png image" << std::endl;
				return -1;
			}
		}
	}

private:
	/* Private camera variables */
	int		image_height;		//rendered image height
	int		n_channels;
	point3  center;				//camera center coordinates
	point3  pixel00_loc;		//location of pixel 0,0
	vec3	pixel_delta_u;		//offset to pixel to the right
	vec3	pixel_delta_v;		//offset to the pixel below

	void initialize()
	{
		//image dimensions
		n_channels = 3;
		image_height = int(image_width / aspect_ratio);
		//safety check: the height should always be at least 1
		image_height = (image_height < 1) ? 1 : image_height;

		center = point3(0.0, 0.0, 0.0);

		//determine viewport dimensions 
		double focal_length = 1.0;
		double viewport_height = 2.0;
		auto viewport_width = viewport_height * (double(image_width) / image_height);

		//viewport coordinate system vectors
		auto viewport_u = vec3(viewport_width, 0, 0);
		auto viewport_v = vec3(0, -viewport_height, 0);

		//horizontal and vertical delta vectors (magnitude = distance between 2 pixels)
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// location of the upper left pixel -> the camera looks towards the negative side of z
		auto viewport_upper_left = center
			- vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left
			+ (pixel_delta_u + pixel_delta_v) * 0.5;
	}

	//function that returns the color for a given scene ray
	color ray_color(const ray& r, const hittable& world) const
	{
		hit_record rec;

		if (world.hit(r, interval(0, inf), rec))
			return 0.5 * (rec.normal + color(1, 1, 1));

		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0); // 0<a<1 the variation is due to the modification of the y component
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

};