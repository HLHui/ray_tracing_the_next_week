#pragma once
#include"vec3.h"
#include<algorithm>
#include"ray.h"
using namespace std;

float ffmin(float a, float b) { return min(a, b); }
float ffmax(float a, float b) { return max(a, b); }

class aabb
{
public:
	vec3 _min;
	vec3 _max;
	aabb() {}
	aabb(const vec3& a, const vec3& b)
	{
		_min = a;
		_max = b;
	}
	vec3 min() const { return _min; }
	vec3 max() const { return _max; }
	bool hit(const ray& r, float tmin, float tmax)const
	{
		for (int a = 0; a < 3; a++)
		{
			float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a], (_max[a] - r.origin()[a]) / r.direction()[a]);
			float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a], (_max[a] - r.origin()[a]) / r.direction()[a]);
			tmin = ffmax(t0, tmin);
			tmax = ffmin(t1, tmax);
			if (tmax <= tmin)
			{
				return false;
			}
		}
		return true;
	}
	void expand(const aabb& box)
	{
		if (box._min[0] < _min[0])
		{
			_min[0] = box._min[0];
		}
		if (box._min[1] < _min[1])
		{
			_min[1] = box._min[1];
		}
		if (box._min[2] < _min[2])
		{
			_min[2] = box._min[2];
		}

		if (box._max[0] < _max[0])
		{
			_max[0] = box._max[0];
		}
		if (box._max[1] < _max[1])
		{
			_max[1] = box._max[1];
		}
		if (box._max[2] < _max[2])
		{
			_max[2] = box._max[2];
		}
	}

	void expand(const vec3 &vec)
	{
		if (vec[0] < _min[0])_min[0] = vec[0];
		if (vec[1] < _min[1])_min[1] = vec[1];
		if (vec[2] < _min[2])_min[2] = vec[2];
	}
};

aabb surroundingBox(aabb box0, aabb box1)
{
	vec3 small(fmin(box0.min().x(), box1.min().x()), fmin(box0.min().y(), box1.min().y()), fmin(box0.min().z(), box1.min().z()));
	vec3 big(fmax(box0.max().x(), box1.max().x()), fmax(box0.max().y(), box1.max().y()), fmax(box0.max().z(), box1.max().z()));
	return aabb(small, big);
}