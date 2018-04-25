#pragma once
#include"vec3.h"

vec3 randonInUnitSphere()
{
	vec3 p(1, 1, 1);
	while (dot(p, p) >= 1.0)
	{
		p = 2 * vec3(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)) - vec3(1, 1, 1);
	}
	return p;
}

vec3 randomOnUnitSphere()
{
	vec3 p(1, 1, 1);
	while (dot(p, p) >= 1.0)
	{
		p = 2 * vec3(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)) - vec3(1, 1, 1);
	}
	return unitVector(p);
}