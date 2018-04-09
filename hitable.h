#pragma once
#include "ray.h"
#include"aabb.h"
#define M_PI 3.1415926

class material;


struct hitRecord
{
	float t;
	vec3 p;
	float u, v;
	vec3 normal;
	material *matPtr;
};
class hitable
{
public:
	virtual bool hit(const ray &r, float tMin, float tmax, hitRecord &rec) const = 0;
	virtual bool boundingBox(float t0, float t1, aabb& box) const = 0;
};
