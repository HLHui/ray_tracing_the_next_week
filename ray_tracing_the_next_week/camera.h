#pragma once
#include"ray.h"
#include<random>
#define M_PI 3.1415926

vec3 random_in_unit_disk()
{
	vec3 p;
	do
	{
		float ux = rand() / (float)RAND_MAX;
		float uz = rand() / (float)RAND_MAX;
		p = 2.0*vec3(ux, 0, uz) - vec3(1, 0, 1);
	} while (dot(p, p) >= 1.0);
	return p;
}

class camera
{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focusDisk, float t0, float t1)
	{
		time0 = t0;
		time1 = t1;
		lensRadius = aperture / 2;
		float theta = vfov*M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect*halfHeight;
		origin = lookfrom;
		w = unitVector(lookfrom - lookat);
		u = unitVector(cross(vup, w));
		v = cross(w, u);
		leftLow = origin - halfWidth*focusDisk*u - halfHeight*focusDisk*v - focusDisk*w;
		horizontal = 2 * halfWidth*focusDisk* u;
		vertical = 2 * halfHeight* focusDisk*v;
	}
	ray getRay(float s, float t)
	{
		vec3 rd = lensRadius * random_in_unit_disk();
		vec3 offset = (u * rd.x()) + (v * rd.z());
		float time = time0 + (rand() / float(RAND_MAX))*(time1 - time0);
		return ray(origin + offset, leftLow + s * horizontal + t * vertical - origin - offset);
	}
	vec3 leftLow;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
	vec3 u, v, w;
	float time0, time1;
	float lensRadius;
};