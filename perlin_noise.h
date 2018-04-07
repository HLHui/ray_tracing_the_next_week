#pragma once
#include"vec3.h"
#include<cmath>
using namespace std;

static float *perlinQuerate()
{
	float *p = new float[256];
	for (int i = 0; i < 256; i++)
	{
		p[i] = rand() / float(RAND_MAX);
	}
	return p;
}

void permute(int *p, int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		int target = int(rand() / float(RAND_MAX)*(i + 1));
		int tmp = p[i];
		p[i] = p[target];
	}
}

static int* perlinQueratePerm()
{
	int *p = new int[256];
	for (int i = 0; i < 256; i++)
	{
		p[i] = i;
	}
	permute(p, 256);
	return p;
}

class perlin
{
public:
	static float *randfloat;
	static int *permX;
	static int *permY;
	static int *permZ;

	float noise(const vec3& p)const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = int(4 * p.x()) & 255;
		int j = int(4 * p.y()) & 255;
		int k = int(4 * p.z()) & 255;
		return randfloat[permX[i] ^ permY[j] ^ permZ[k]];
	}
	
};


float *perlin::randfloat = perlinQuerate();
int *perlin::permX = perlinQueratePerm();
int *perlin::permY = perlinQueratePerm();
int *perlin::permZ = perlinQueratePerm();