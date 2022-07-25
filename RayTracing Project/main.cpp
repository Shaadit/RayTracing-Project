#include <iostream>
#include <limits.h>  
#include "sdltemplate.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

vec3 color(const ray& r, hitable* world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001, _MM_FIXUP_MAX_FLOAT, rec))
	{
		ray scattered;
		vec3 attenuation;

		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}

	}
	else
	{	
		// SKY
		vec3 unit_dir = unit_vector(r.direction());
		float a = 0.5 * (unit_dir.y() + 1.0);
		return (1.0 - a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);
	}
}


int main() {

	//Size of our Window
	int width = 800;
	int height = 400;
	
	int ns = 10;

	sdltemplate::sdl("Ray Tracing", width, height);
	sdltemplate::loop();

	hitable* list[5];
	list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.8,0.3,0.3)));
	list[1] = new sphere(vec3(0,-100.5,-1), 100,new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2),0.2));
	list[3] = new sphere(vec3(-1,0,-1), 0.5, new metal(vec3(0.8, 0.8, 0.8),0.8));
	list[4] = new sphere(vec3(0.0,1.0,-1), 0.5, new metal(vec3(0.8, 0.8, 0.8), 0.7));

	hitable_list* world = new hitable_list(list, 5);

	camera cam;

	for (int y = height - 1; y >= 00; y--) {
		for (int x = 0; x < width; x++) {

			vec3 col(0, 0, 0);

			for (int s = 0; s < ns; s++)
			{
				float u = float(x + (rand() / (RAND_MAX + 1.0))) / float(width);
				float v = float(y + (rand() / (RAND_MAX + 1.0))) / float(height);

				ray r = cam.getRay(u, v);

				vec3 p = r.pointAtParameter(2.0);
				col += color(r, world, 0);
			}

			//Color

			col /= float(ns);

			col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			sdltemplate::setDrawColor(sdltemplate::createColor(ir, ig, ib, 255));
			sdltemplate::drawPoint(x, height - y);
		}
	}
	while (sdltemplate::running) {
		sdltemplate::loop();
	}
}