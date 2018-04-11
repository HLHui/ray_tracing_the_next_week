#pragma once
#include "ray.h"
#include"aabb.h"
#include"texture.h"
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


class flipNormals :public hitable
{
public:
	flipNormals(hitable *p):ptr(p){}
	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec)const
	{
		if (ptr->hit(r, tMin, tMax, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual bool boundingBox(float t0, float t1, aabb& box)const
	{
		return ptr->boundingBox(t0, t1, box);
	}
	hitable *ptr;
};


