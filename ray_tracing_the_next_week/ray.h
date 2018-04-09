#pragma once
#include"vec3.h"
using namespace std;

class ray
{
public:
	ray() {};
	ray(const vec3& _ori, const vec3& _dir, float ti = 0.0)
	{
		ori = _ori;
		dir = _dir;
		Time = ti;
	}
	const vec3& origin() const{ return ori; }
	const vec3& direction() const{ return dir; }
	float time()const { return Time; }
	vec3 point_at_parameter(float t) const { return ori + t*dir; }

	vec3 ori;
	vec3 dir;
	float Time;
};