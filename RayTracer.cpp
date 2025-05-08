// RayTracer.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include <stdint.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

#include "include/color.h"
#include "include/vec3.h"

#define IM_W 256
#define IM_H 256
#define N_CH 3


typedef enum {
	PNG = 0,
	PPM = 1
} format_t;

int create_image(int w, int h, int nCh, format_t format);

int main()
{
	if (create_image(IM_W, IM_H, N_CH, PNG))
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