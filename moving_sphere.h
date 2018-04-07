#pragma once
#include"ray.h"
#include"vec3.h"
#include"camera.h"
#include"hitable.h"
#include"aabb.h"
#include<math.h>
using namespace std;
class movingSphere :public hitable
{
public:
	movingSphere() {}
	movingSphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m) :
		center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), matPtr(m) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec) const
	{
		vec3 direction = r.origin() - center(r.time());
		float a = dot(r.direction(), r.direction());
		float b = dot(direction, r.direction());
		float c = dot(direction, direction) - radius*radius;
		float discriminant = b*b - a*c;
		if (discriminant > 0)
		{
			float temp = (-b - sqrt(discriminant)) / a;
			if (temp > tmin && temp < tmax)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center(r.time())) / radius;
				rec.matPtr = matPtr;
				return true;
			}
			temp = (-b + sqrt(discriminant)) / a;
			if (temp > tmin && temp < tmax)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center(r.time())) / radius;
				rec.matPtr = matPtr;
				return true;
			}
		}
		return false;
	}
	virtual bool boundingBox(float t0, float t1, aabb& box) const {
		aabb box0(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
		aabb box1(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
		box = surroundingBox(box0, box1);
		return true;
	}


	vec3 center(float time) const
	{
		return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
	}
	vec3 center0, center1;
	float time0, time1;
	float radius;
	material *matPtr;
};