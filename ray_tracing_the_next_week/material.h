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
	virtual vec3 emitted(float u, float v, vec3& p) const
	{
		return vec3(0, 0, 0);
	}
	virtual bool scatter(const ray &rIn, const hitRecord &rec, vec3 &attenuation, ray &scatter) const = 0;
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

