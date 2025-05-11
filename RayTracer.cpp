// RayTracer.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "rtmath.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main()
{
	/* world settings */

	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
 
	
	/* create camera */
	camera c;
	c.aspect_ratio = 16.0 / 9.0;
	c.image_width = 400;
	if (!c.render(world))
	{
		std::cerr << "Problems in the creation of the image!" << std::endl;
	}

}

