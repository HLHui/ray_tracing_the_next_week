#pragma once
#include "hitable.h"
#include"vec3.h"
#include"texture.h"

vec3 reflect(const vec3 &v, const vec3 &n)
{
	return v - 2 * dot(v, n)*n;
}

bool refract(const vec3 &v, const vec3 &n, float niOverNt, vec3 &refracted)
{
	vec3 uv = unitVector(v);
	float dt = dot(uv, n);
	float discriminat = 1.0 - niOverNt*niOverNt*(1 - dt*dt);
	if (discriminat > 0)
	{
		refracted = niOverNt*(uv - n*dt) - n*sqrt(discriminat);
		return true;
	}
	else
	{
		return false;
	}
}

float schlick(float cosine, float refIdx)
{
	float r0 = (1 - refIdx) / (1 + refIdx);
	r0 = r0*r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}

vec3 randomInUnitSphere()
{
	vec3 p(1, 1, 1);
	while (p.squaredLength() >= 1.0)
	{
		// Generate 3 random values between 0 and 1 (floating point)
		float ux = (rand() / (float)RAND_MAX);
		float uy = (rand() / (float)RAND_MAX);
		float uz = (rand() / (float)RAND_MAX);

		p = 2.0*vec3(ux, uy, uz) - vec3(1, 1, 1);
	}
	return p;
}

class material
{
public:

	virtual bool scatter(const ray &rIn, const hitRecord &rec, vec3 &attenuation, ray &scatter) const = 0;
	virtual vec3 emitted(float u, float v, const vec3& p) const
	{
		return vec3(0, 0, 0);
	}
};

class lambertian :public material
{
public:
	lambertian(texture *a) : albedo(a) {}

	virtual bool scatter(const ray &rIn, const hitRecord &rec, vec3 &attenuation, ray &scattered) const
	{
		vec3 target = rec.p + rec.normal + randomInUnitSphere();
		scattered = ray(rec.p, target - rec.p, rIn.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
	texture *albedo;
};

class metal :public material
{
public:
	metal(const vec3 &a, float f):albedo(a)
	{
		if (f < 1)
		{
			fuzz = f;
		}
		else
		{
			fuzz = 1;
		}
	}
	virtual bool scatter(const ray &rIn, const hitRecord &rec, vec3 &attenuation, ray &scattered) const
	{
		vec3 reflected = reflect(unitVector(rIn.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz*randomInUnitSphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	vec3 albedo;
	float fuzz;
};

class dielectric :public material
{
public:
	float refIdx;
	dielectric(float ri) :refIdx(ri) {}
	virtual bool scatter(const ray &rIn, const hitRecord &rec, vec3 &attenuation, ray &scattered) const
	{
		vec3 outwardNormal;
		vec3 reflected = reflect(rIn.direction(), rec.normal);
		float niOverNt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;
		float reflectProb;
		float cosine;

		if (dot(rIn.direction(), rec.normal) > 0)
		{
			outwardNormal = -rec.normal;
			niOverNt = refIdx;
			cosine = refIdx*dot(rIn.direction(), rec.normal) / rIn.direction().length();
		}
		else
		{
			outwardNormal = rec.normal;
			niOverNt = 1.0 / refIdx;
			cosine = -dot(rIn.direction(), rec.normal) / rIn.direction().length();
		}

		if (refract(rIn.direction(), outwardNormal, niOverNt, refracted))
		{
			reflectProb = schlick(cosine, refIdx);
		}
		else
		{
			scattered = ray(rec.p, reflected);
			reflectProb = 1.0;
		}
		if (rand() / (float)RAND_MAX < reflectProb)
		{
			scattered = ray(rec.p, reflected);
		}
		else
		{
			scattered = ray(rec.p, refracted);
		}
		return true;
	}
};


//light emite material
class diffuseLight :public material
{
public:
	texture *emit;
	diffuseLight(texture * a):emit(a){}
	virtual bool scatter(const ray & rIn, const hitRecord& rec, vec3& attenuation, ray& scattered)const
	{
		return false;
	}
	virtual vec3 emitted(float u, float v, const vec3& p)const
	{
		return emit->value(u, v, p);
	}
};


class isotropic :public material
{
public:
	isotropic(texture* a) :albedo(a) {}
	texture *albedo;
	virtual bool scatter(const ray& rIn, const hitRecord& rec, vec3& attenuation, ray& scattered)const
	{
		scattered = ray(rec.p, randomInUnitSphere());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}
};

class constantMedium :public hitable {
public:
	constantMedium(hitable *b, float d, texture *a) :boundary(b), density(d)
	{
		phaseFunction = new isotropic(a);
	}
	virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec)const
	{
		bool db = (rand() / float(RAND_MAX) < 0.00001);
		db = false;
		hitRecord rec1, rec2;
		if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1))
		{
			if (boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2))
			{
				if (db)
				{
					cerr << endl << "t0 t1 " << rec1.t << " " << rec2.t << endl;
				}
				if (rec1.t < tmin)
				{
					rec1.t = tmin;
				}
				if (rec2.t > tmax)
				{
					rec2.t = tmax;
				}
				if (rec1.t > rec2.t)
				{
					return false;
				}
				if (rec1.t < 0)
				{
					rec1.t = 0;
				}
				float distanceInsideBoundary = (rec2.t - rec1.t)*r.direction().length();
				float hitDistance = -(1 / density)*log(rand() / float(RAND_MAX));
				if (hitDistance < distanceInsideBoundary)
				{
					if (db)cerr << "hitDistance = " << hitDistance << endl;
					rec.t = rec1.t + hitDistance / r.direction().length();
					if (db)cerr << "rec.t = " << rec.t << endl;
					rec.p = r.point_at_parameter(rec.t);
					if (db)cerr << "rec.p = " << rec.p << endl;
					rec.normal = vec3(1, 0, 0);
					rec.matPtr = phaseFunction;
					return true;
				}
			}
		}
		return false;
	}
	virtual bool boundingBox(float t0, float t1, aabb& box)const
	{
		return boundary->boundingBox(t0, t1, box);
	}
	hitable *boundary;
	float density;
	material *phaseFunction;
};