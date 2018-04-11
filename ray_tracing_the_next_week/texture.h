#pragma once
#include"ray.h"
#include"perlin_noise.h"

class texture
{
public:
	virtual vec3 value(float u, float v, const vec3& p)const = 0;
};

class constantTexture :public texture
{
public:
	vec3 color;
	constantTexture(){}
	constantTexture(vec3 c):color(c){}
	virtual vec3 value(float u, float v, const vec3& p)const
	{
		return color;
	}

};

class checkerTexture :public texture
{
public:

	checkerTexture(){}
	checkerTexture(texture *t0,texture *t1):even(t0),odd(t1){}
	virtual vec3 value(float u, float v, const vec3& p)const
	{
		float sines = sin(10 * p.x())*sin(10*p.y())*sin(10 * p.z());
		if (sines < 0)
		{
			return odd->value(u, v, p);
		}
		else
		{
			return even->value(u, v, p);
		}
	}
		texture *odd;
	texture *even;
};

class noiseTexture :public texture
{
public:
	noiseTexture() {}
	noiseTexture(float sc):scale(sc){}
	virtual vec3 value(float u, float v, const vec3& p)const
	{
//		return vec3(1, 1, 1)*noise.noise(p);

//		return vec3(1, 1, 1)*noise.turb(scale*p);
//		return vec3(1, 1, 1)*0.5*(1 + sin(scale*p.z() + 10 * noise.turb(p)));//turblence2
//		return vec3(1, 1, 1)*(1 + sin(scale*p.z() + 10 * noise.turb(p)));//turblence2
//		return vec3(1, 1, 1)*0.1*(1 + sin(scale*p.z() + 10 * noise.turb(p)));//turbence4;
		return vec3(1, 1, 1)*0.4*(1 + sin(scale*p.z() + 10 * noise.turb(p)));//turbence5;
//		return vec3(1, 1, 1)*0.4*(1 + sin(scale*p.x() + 5 * noise.turb(scale*p)));//simple light;


	}
	perlin noise;
	float scale;
};

