#pragma once
#include<algorithm>
#include<float.h>
#include"vec3.h"
#include"material.h"
#include"aabb.h"
#include"hitable.h"
using namespace std;

struct Triangle:public hitable
{
	vec3 v0, v1, v2;
	vec3 e1, e2;
	vec3 normal, t0, t1, t2;
	material *m;

	Triangle(vec3 _v0, vec3 _v1, vec3 _v2, material *_m)
	{
		v0 = _v0;
		v1 = _v1;
		v2 = _v2;
		e1 = v1 - v0;
		e2 = v2 - v0;
		normal = cross(e1, e2).normal();
		m = _m;
	}

	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec)const
	{
		float u, v, temp;
		vec3 pvec = cross(r.dir, e2);
		float det = dot(e1, pvec);
		//parallel
		if (det == 0)
		{
			return false;
		}
		float invDet = 1. / det;
		vec3 tvec = r.ori - v0;
		u = dot(tvec, pvec)*invDet;
		if (u < 0 || u>1)
		{
			return false;
		}
		vec3 qvec = cross(tvec, e1);
		v = dot(r.dir, qvec)*invDet;
		if (v < 0 || u + v > 1)
		{
			return false;
		}
		temp = dot(e2, qvec)*invDet;
		if (temp < tMin&&temp>0)
		{
			rec.u = u;
			rec.v = v;
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = normal;
			rec.matPtr = m;
			return true;
		}
		return false;


		/*vec3 ab = v1 - v0;
		vec3 ac = v2 - v0;
		vec3 ao = r.ori - v0;
		float D = ab[0] * ac[1] * (-r.dir[2]) + ab[1] * ac[2] * (-r.dir[0]) + ab[2] * ac[0] * (-r.dir[1]) -
			(-r.dir[0] * ac[1] * ab[2] - r.dir[1] * ac[2] * ab[0] - r.dir[2] * ac[0] * ab[1]);

		cout << D << endl << endl;
		_getch();
		float D1 = ao[0] * ac[1] * (-r.dir[2]) + ao[1] * ac[2] * (-r.dir[0]) + ao[2] * ac[0] * (-r.dir[1]) -
			(-r.dir[0] * ac[1] * ao[2] - r.dir[1] * ac[2] * ao[0] - r.dir[2] * ac[0] * ao[1]);

		float D2 = ab[0] * ao[1] * (-r.dir[2]) + ab[1] * ao[2] * (-r.dir[0]) + ab[2] * ao[0] * (-r.dir[1]) -
			(-r.dir[0] * ao[1] * ab[2] - r.dir[1] * ao[2] * ab[0] - r.dir[2] * ao[0] * ab[1]);

		float D3 = ab[0] * ac[1] * ao[2] + ab[1] * ac[2] * ao[0] + ab[2] * ac[0] * ao[1] -
			(ao[0] * ac[1] * ab[2] + ao[1] * ac[2] * ab[0] + ao[2] * ac[0] * ab[1]);

		float u = D1 / D;
		float v = D2 / D;
		float temp = D3 / D;

		rec.t = temp;
		rec.p = r.point_at_parameter(rec.t);
		rec.normal = normal;
		rec.matPtr = m;
		return true;

		if (u > 0 && v > 0 && u + v < 1)
		{
			cout << u << " , " << v << endl;
			_getch();
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = normal;
			rec.matPtr = m;
			return true;
		}
		return false;*/
	}

	virtual bool boundingBox(float t0, float t1, aabb& box)const
	{
		vec3 bl = vec3(min(min(v0[0], v1[0]), v2[0]), min(min(v0[1], v1[1]), v2[1]), min(min(v0[2], v1[2]), v2[2]));
		vec3 tr = vec3(max(max(v0[0], v1[0]), v2[0]), max(max(v0[1], v1[1]), v2[1]), max(max(v0[2], v1[2]), v2[2]));
		box=aabb(bl, tr);
		return true;
	}

	/*aabb getBoundingBox()
	{
		vec3 bl = vec3(min(min(v0.x, v1.x), v2.x), min(min(v0.y, v1.y), v2.y), min(min(v0.z, v1.z), v2.z));
		vec3 tr = vec3(max(max(v0.x, v1.x), v2.x), max(max(v0.y, v1.y), v2.y), max(max(v0.z, v1.z), v2.z));
		return aabb(bl, tr);
	}*/

	vec3 getMidpoint()
	{
		return (v0 + v1 + v2) / 3;
	}

	//bool intersect(ray& r, float t, float tmin, vec3& norm)const
	//{
	//	float u, v, temp = 0;
	//	vec3 pvec = cross(r.dir, e2);
	//	float det = dot(e1, pvec);
	//	if (det == 0)
	//	{
	//		return false;
	//	}
	//	float invDet = 1. / det;
	//	vec3 tvec = r.ori - v0;
	//	u = dot(tvec, pvec)*invDet;
	//	if (u < 0 || u>1)
	//	{
	//		return false;
	//	}
	//	vec3 qvec = cross(tvec, e1);
	//	v = dot(r.dir, qvec)*invDet;
	//	if (v < 0 || u + v > 1)
	//	{
	//		return false;
	//	}
	//	temp = dot(e2, qvec)*invDet;
	//	if (temp < tmin&&temp>0)
	//	{
	//		t = temp;
	//		norm = normal;
	//		return true;
	//	}
	//	return false;
	//}
	
	
	vec3 barycentric(vec3 p)
	{
		vec3 _v2 = p - v0;
		float d00 = dot(e1, e1);
		float d01 = dot(e1, e2);
		float d11 = dot(e2, e2);
		float d20 = dot(_v2, e1);
		float d21 = dot(_v2, e2);
		float d = d00*d01 - d01*d11;
		float v = (d11*d20 - d01*d21) / d;
		float w = (d00*d21 - d01*d20) / d;
		float u = 1 - v - w;
		return vec3(u, v, w);
	}

	//vec3 getColorAt(vec3 p)
	//{
	//	if (m == NULL)
	//	{
	//		return vec3(1, 0, 1);
	//	}
	//	vec3 b = barycentric(p);
	//	vec3 c = vec3();
	//	c += t0*b.x;
	//	c += t1*b.y;
	//	c += t2*b.z;
	//}

};
