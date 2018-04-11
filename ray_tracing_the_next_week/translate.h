#pragma once
#include"hitable.h"

class translate :public hitable
{
public:
	vec3 offset;
	hitable *ptr;
	translate(hitable *p,const vec3& displacement):ptr(p),offset(displacement){}
	virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec)const
	{
		ray movedR(r.origin() - offset, r.direction(), r.time());
		if (ptr->hit(movedR, tmin, tmax, rec))
		{
			rec.p += offset;
			return true;
		}
		return false;
	}

	virtual bool boundingBox(float t0, float t1, aabb& box)const
	{
		if (ptr->boundingBox(t0, t1, box))
		{
			box = aabb(box.min() + offset, box.max() + offset);
			return true;
		}
		return false;
	}
};

class rotateY :public hitable
{
public:
	hitable *ptr;
	float sinTheta;
	float cosTheta;
	bool hasbox;
	aabb bbox;
	rotateY(hitable *p, float angle):ptr(p)
	{
		float radius = (M_PI / 180.0)*angle;
		sinTheta = sin(radius);
		cosTheta = cos(radius);
		hasbox = ptr->boundingBox(0, 1, bbox);
		vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
		vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					float x = i*bbox.max().x() + (1 - i)*bbox.min().x();
					float y = j*bbox.max().y() + (1 - j)*bbox.min().y();
					float z = k*bbox.max().z() + (1 - j)*bbox.min().y();
					float newx = cosTheta*x + sinTheta*z;
					float newz = -sinTheta*x + cosTheta*z;
					vec3 tester(newx, y, newz);
					for (int c = 0; c < 3; c++)
					{
						if (tester[c] > max[c])
						{
							max[c] = tester[c];
						}
						if (tester[c] < min[c])
						{
							min[c] = tester[c];
						}
					}
				}
			}
		}
		bbox = aabb(min, max);
	}
	virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec)const
	{
		vec3 origin = r.origin();
		vec3 direction = r.direction();
		origin[0] = cosTheta*r.origin()[0] - sinTheta*r.origin()[2];
		origin[2] = sinTheta*r.origin()[0] + cosTheta*r.origin()[2];
		direction[0] = cosTheta*r.direction()[0] - sinTheta*r.direction()[2];
		direction[2] = sinTheta*r.direction()[0] + cosTheta*r.direction()[2];
		ray rotateR(origin, direction, r.time());
		if (ptr->hit(rotateR, tmin, tmax, rec))
		{
			vec3 p = rec.p;
			vec3 normal = rec.normal;
			p[0] = cosTheta*rec.p[0] + sinTheta*rec.p[2];
			p[2] = -sinTheta*rec.p[0] + cosTheta*rec.p[2];
			normal[0] = cosTheta*rec.normal[0] + sinTheta*rec.normal[2];
			normal[2] = -sinTheta*rec.normal[0] + cosTheta*rec.normal[2];
			rec.p = p;
			rec.normal = normal;
			return true;
		}
		return false;
	}
	virtual bool boundingBox(float t0, float t1, aabb& box)const
	{
		box = bbox;
		return hasbox;
	}
};