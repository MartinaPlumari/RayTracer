// RayTracer.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include <stdint.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

#include "include/color.h"
#include "include/vec3.h"

#define N_CHANNELS 3

typedef enum {
	PNG = 0,
	PPM = 1
} format_t;

int create_image(int w, int h, int nCh, format_t format);

int main()
{
	/* setting image dimensions */
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;
	int image_height = int((image_width + 0.9) / aspect_ratio);
	//safety check: the height should always be at least 1
	image_height = (image_height < 1) ? 1 : image_height;

	/* setting viewport dimensions */
	auto viewport_height = 2.0;
	int viewport_width = viewport_height * (double(image_width) / image_height);

	
	if (create_image(image_width, image_height, N_CHANNELS, PNG))
		return -1;

}

int create_image(int w, int h, int nCh, format_t format) 
{
	uint8_t* pixels = new uint8_t[w * h * nCh];
	uint32_t id = 0;
	
	//decide if it makes sense to extend and to make more robust and flexible (channel managing)
	assert(nCh == 3);
	
	if (format == PPM) {
		std::cout << "P3\n" << w << ' ' << h << "\n" << "\n255\n";
	}

	for (int j = 0; j < h; j++) {
		if(format == PPM)
			std::clog << "\rScanlines remaining: " << (h - j) << ' ' << std::flush;
		for (int i = 0; i < w; i++) 
		{
			auto pixel_color = color(double(i) / (w - 1), double(j) / (h - 1), 0);

			if (format == PPM)
				write_color_to_file(std::cout, pixel_color);
			else if (format == PNG) 
				write_color_to_matrix(pixel_color, pixels, &id);
		}
	}

	if(format == PPM)
		std::clog << "\rDone.                  " << std::endl;

	else if (format == PNG)
	{
		int retVal = stbi_write_png("image.png", w, h, nCh, pixels, w * nCh);
		if(retVal == 0)
		{
			std::cerr << "Error in saving png image" << std::endl;
			return -1;
		}
	}

	return 0;
}