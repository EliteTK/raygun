#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "vec.h"
#include "sdl.h"
#include "SDL2/SDL.h"

static const double width = 640, height = 480, fov = 5;

struct sphere {
	double *point;
	double radius;
};

double *light;
struct sphere sphere;

colour_t cast_ray(double *origin, double *direction)
{
	colour_t retval;

	double closest_len = vec_dot(origin, sphere.point);
	double closest_dist = vec_len(vec_addm(vec_addm(vec_blank(3), origin, closest_len), sphere.point, -1));

	if (closest_dist >= sphere.radius) {
		retval = (colour_t){{0, 0, 0, 255}};
		goto fail;
	}

	double *hit_pos = vec_perm(vec_addm(vec_blank(3), origin, closest_len - sqrt(sphere.radius * sphere.radius - closest_dist * closest_dist)));
	double *hit_normal = vec_perm(vec_norm(vec_addm(hit_pos, sphere.point, -1)));

	/*double *hit_reflect = vec_addm(vec_addm(vec_blank(3), hit_normal, -vec_dot(origin, hit_normal)), origin, 2);*/

	/*double specular = vec_dot(hit_reflect, vec_norm(vec_addm(light, hit_pos, -1)));*/
	/*if (specular < 0)*/
		/*specular = 0;*/

	double diffuse = vec_dot(hit_normal, vec_norm(vec_addm(light, hit_pos, -1)));
	if (diffuse < 0)
		diffuse = 0;

	double bright = diffuse/* + specular*/;
	if (bright > 1)
		bright = 1;

	retval = (colour_t){{bright * 255, bright * 255, bright * 255, 255}};

	vec_del(hit_pos);
	vec_del(hit_normal);
fail:
	return retval;
}

int main(void)
{
	double *eye = vec_perm(vec(0, 0, 0));
	sphere = (struct sphere){vec_perm(vec(0.0, 0.0, -0.9)), 0.3};
	light = vec_perm(vec(1.0, -1.0, -0.0));

	sdl_prepare(width, height);

	for (size_t i = 0; i < width * height; i++)
		sdl_buffer[i] = (colour_t){{0, 0, 0, 255}};

	for (double fov = 0; fov < 180; fov += 0.5) {
		double plane_hwidth = sin((fov / 180.0 * M_PI) / 2.0);
		double dpp = plane_hwidth / (width / 2.0);
		double plane_hheight = (height / 2.0) * dpp;
		double hdpp = dpp / 2;

		uint32_t begun = SDL_GetTicks();
		for (uint16_t x = 0; x < width; x++)
			for (uint16_t y = 0; y < height; y++) {
				double *ray = vec_perm(vec_norm(vec(plane_hwidth - x * dpp + hdpp, -plane_hheight + y * dpp + hdpp, -1)));

				sdl_buffer[y * (uint16_t)width + x] = cast_ray(ray, eye);
				vec_del(ray);
			}
		sdl_draw();
	}


	/*sleep(10);*/

	vec_del(eye);
	vec_del(sphere.point);
	vec_del(light);
	return 0;
}
