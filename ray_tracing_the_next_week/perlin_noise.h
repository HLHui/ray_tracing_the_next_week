#pragma once
#include"vec3.h"
#include<cmath>
using namespace std;

//static float *perlinQuerate()
//{
//	float *p = new float[256];
//	for (int i = 0; i < 256; i++)
//	{
//		p[i] = rand() / float(RAND_MAX);
//	}
//	return p;
//}

static vec3 *perlinQuerate()
{
	vec3 *p = new vec3[256];
	for (int i = 0; i < 256; i++)
	{
		p[i] = unitVector(vec3(-1, +2 * rand() / float(RAND_MAX), -1 + 2 * rand() / float(RAND_MAX)));
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

//inline float trilinearInterp(float c[2][2][2], float u, float v, float w)
//{
//	float accum = 0;
//	for (int i = 0; i < 2; i++)
//	{
//		for (int j = 0; j < 2; j++)
//		{
//			for (int k = 0; k < 2; k++)
//			{
//				accum += (i*u + (1 - i)*(1 - u))*(j*v + (1 - j)*(1 - v))*(k*w + (1 - k)*(1 - w))*c[i][j][k];
//			}
//		}
//	}
//	return accum;
//}

inline float trilinearInterp(vec3 c[2][2][2], float u, float v, float w)
{
	float uu = u*u*(3 - 2 * u);
	float vv = v*v*(3 - 2 * v);
	float ww = w*w*(3 - 2 * w);
	float accum = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				vec3 weightV(u - i, v - j, w - k);
				accum += (i*uu + (1 - i)*(1 - uu))*(j*vv + (1 - j)*(1 - vv))*(k*ww + (1 - k)*(1 - ww))*dot(c[i][j][k],weightV);
			}
		}
	}
	return accum;
}

inline float perlinInterp(vec3 c[2][2][2],float u, float v, float w)
{
	float uu = u*u*(3 - 2 * u);
	float vv = v*v*(3 - 2 * v);
	float ww = w*w*(3 - 2 * w);
	float accum = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				vec3 weightV(u - i, v - j, w - k);
				accum += (i*uu + (1 - i)*(1 - uu))*(j*vv + (1 - j)*(1 - vv))*(k*ww + (1 - k)*(1 - ww))*dot(c[i][j][k], weightV);
			}
		}
	}
	return accum;
}

//TODO turbulence




class perlin
{
public:
	static float *randfloat;
	static int *permX;
	static int *permY;
	static int *permZ;
	static vec3 *ranvec;
	float noise(const vec3& p)const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		//smooth
		/*int i = int(4 * p.x()) & 255;
		int j = int(4 * p.y()) & 255;
		int k = int(4 * p.z()) & 255;*/
		//smooth1
		//int i = int( p.x());
		//int j = int( p.y());
		//int k = int( p.z());
		//hermite cubic,smoother
		//u = u*u*(3 - 2 * u);
		//v = v*v*(3 - 2 * v);
		//w = w*w*(3 - 2 * w);
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());
		vec3 c[2][2][2];
		for (int di = 0; di < 2; di++)
		{
			for (int dj = 0; dj < 2; dj++)
			{
				for (int dk = 0; dk < 2; dk++)
				{
//					c[di][dj][dk] = randfloat[permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^ permZ[(k + dk) & 255]];
	
					c[di][dj][dk] = ranvec[permX[(i + di)&255] ^ permY[(j + dj)&255] ^ permZ[(k + dk)&255]];//???????
				}
			}
		}
//		return randfloat[permX[i] ^ permY[j] ^ permZ[k]];
//		return trilinearInterp(c, u, v, w);

		
		return perlinInterp(c, u, v, w);
		
	}
	float turb(const vec3& p, int depth = 7) const
	{
		float accum = 0;
		vec3 tempP = p;
		float weight = 1.0;
		for (int i = 0; i < depth; i++)
		{
			accum += weight*noise(tempP);
			weight *= 0.5;
			tempP *= 2;
		}
		return fabs(accum);
	}

};


//float *perlin::randfloat = perlinQuerate();
//randown unit vectors
vec3 *perlin::ranvec = perlinQuerate();
int *perlin::permX = perlinQueratePerm();
int *perlin::permY = perlinQueratePerm();
int *perlin::permZ = perlinQueratePerm();