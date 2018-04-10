#pragma once
#include "hitable.h"

class sphere :public hitable
{
public:
	sphere() {}
	vec3 center;
	float radius;
	material *maPtr;
	sphere(vec3 cen, float r,material *m) :center(cen), radius(r), maPtr(m) {};
	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const
	{
		vec3 dir = r.origin() - center;
		float a = dot(r.direction(), r.direction());
		float b = dot(dir, r.direction());
		float c = dot(dir, dir) - radius*radius;
		float discriminant = b*b - a*c;////
		if (discriminant > 0)
		{
			float temp = (-b - sqrt(discriminant)) / (a);
			if (temp<tMax&&temp>tMin)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.matPtr = maPtr;
				return true;
			}
			temp = (-b + sqrt(discriminant)) / (a);
			if (temp<tMax&&temp>tMin)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.matPtr = maPtr;
				return true;
			}
		}
		return false;
	}

	virtual bool boundingBox(float t0, float t1, aabb& box) const
	{
		box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
		return true;
	}
};
