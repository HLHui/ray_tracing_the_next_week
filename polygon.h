#pragma once
#include "hitable.h"
#include "sphere.h"

class polygon :public hitable
{
	public:
		polygon() {}
		polygon(vec3 *v,int n, material *m) : vertexes(v), number(n), ma(m) {}
		virtual bool hit(const ray &r, float tmin, float tmax, hitRecord rec) const;
		vec3 *vertexes;
		int number;
		material *ma;
};