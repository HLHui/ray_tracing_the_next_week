#pragma once
#include "hitable.h"
#include "vec2.h"

class triangle :public hitable
{
public:
	triangle() {}
	vec3 v0,v1,v2;
	material *ma;
	vec3 normal;
	triangle(vec3 _v0,vec3 _v1,vec3 _v2, material *m) :v0(_v0),v1(_v1),v2(_v2), ma(m)
	{
		normal = cross(v1 - v0, v2 - v0);
	}
	virtual bool hit(const ray &r, float tmin, float tmax, hitRecord &rec) const
	{

		vec3 v0v1 = v1 - v0;
		vec3 v0v2 = v2 - v0;
		vec3 ao = r.ori - v0;
		float D = v0v1[0] * v0v2[1] * (-r.dir[2]) + v0v1[1] * v0v2[2] * (-r.dir[0]) + v0v1[2] * v0v2[0] * (-r.dir[1]) -
			(-r.dir[0] * v0v2[1] * v0v1[2] - r.dir[1] * v0v2[2] * v0v1[0] - r.dir[2] * v0v2[0] * v0v1[1]);
		float D1 = ao[0] * v0v2[1] * (-r.dir[2]) + ao[1] * v0v2[2] * (-r.dir[0]) + ao[2] * v0v2[0] * (-r.dir[1]) -
			(-r.dir[0] * v0v2[1] * ao[2] - r.dir[1] * v0v2[2] * ao[0] - r.dir[2] * v0v2[0] * ao[1]);

		float D2 = v0v1[0] * ao[1] * (-r.dir[2]) + v0v1[1] * ao[2] * (-r.dir[0]) + v0v1[2] * ao[0] * (-r.dir[1]) -
			(-r.dir[0] * ao[1] * v0v1[2] - r.dir[1] * ao[2] * v0v1[0] - r.dir[2] * ao[0] * v0v1[1]);
		
		float D3 = v0v1[0] * v0v2[1] * ao[2] + v0v1[1] * v0v2[2] * ao[0] + v0v1[2] * v0v2[0] * ao[1] -
			(ao[0] * v0v2[1] * v0v1[2] + ao[1] * v0v2[2] * v0v1[0] + ao[2] * v0v2[0] * v0v1[1]);

		float a = D1 / D;
		float b = D2 / D;
		float temp = D3 / D;

		if (temp < 0)
		{
			return false;
		}

		if (a >= 0 && b >= 0 && a + b < 1)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = normal;
			rec.matPtr = ma;
			return true;
		}
		return false;

	}
	virtual bool boundingBox(float t0, float t1, aabb& box)const
	{
		vec3 bl = vec3(min(min(v0[0], v1[0]), v2[0]), min(min(v0[1], v1[1]), v2[1]), min(min(v0[2], v1[2]), v2[2]));
		vec3 tr = vec3(max(max(v0[0], v1[0]), v2[0]), max(max(v0[1], v1[1]), v2[1]), max(max(v0[2], v1[2]), v2[2]));
		box = aabb(bl, tr);
		return true;
	}
};

/*bool inPolyonTest(vec2 *vertexesUV, int number)
{
	int sh, nsh;
	int nc = 0;
	if (vertexesUV[0].v < 0)
	{
		sh = -1;
	}
	else
	{
		sh = 1;
	}

	for (int j = 0; j < number; j++)
	{
		if (vertexesUV[j + 1].v < 0)
		{
			nsh = -1;
		}
		else
		{
			nsh = 1;
		}
		if (sh != nsh)
		{
			if (vertexesUV[j].u > 0 && vertexesUV[j + 1].u > 0)
			{
				nc++;
			}
			else
			{
				if (vertexesUV[j].u > 0 && vertexesUV[j + 1].u > 0)
				{
					if ((vertexesUV[j].u - vertexesUV[j].v)*(vertexesUV[j + 1].u - vertexesUV[j].u) / (vertexesUV[j + 1].v - vertexesUV[j].v) > 0)
					{
						nc++;
					}
				}
			}
		}
		sh = nsh;
	}
	if (nc % 2)
	{
		return true;
	}
	return false;
}*/
//class polygon :public hitable
//{
//public:
//	polygon() {}
//	vec3 *vertexes;
//	int number;
//	material *ma;
//	polygon(vec3 *v, int n, material *m) : vertexes(v), number(n), ma(m) {}
//	virtual bool hit(const ray &r, float tmin, float tmax, hitRecord &rec) const
//	{
//		vec3 polyN;
//		for (int i = 0; i < number - 2; i++)
//		{
//			polyN = unitVector(cross((vertexes[i] - vertexes[i + 1]), (vertexes[i + 1] - vertexes[i + 2])));
//			if (!vectorEqual(polyN, vec3(0, 0, 0)))
//			{
//				break;
//			}
//		}
//		float polyD = -(dot(polyN, vertexes[0]));
//		float vd = dot(polyN, r.dir);
//		float v0 = -(dot(polyN, r.ori) + polyD);
//		if (vd == 0)
//		{
//			return false;
//		}
//		else
//		{
//			float t = v0 / vd;
//			vec3 pi = r.point_at_parameter(t);
//			float temp = abs(polyN.x());
//			int i = 1;
//			if (temp <= abs(polyN.y()))
//			{
//				temp = abs(polyN.y());
//				i++;
//			}
//			if (temp <= abs(polyN.z()))
//			{
//				i++;
//			}
//			int maxx = number + 1;
//			vec2 vertexesUV[4];
//			if (i == 1)
//			{
//				for (int i = 0; i < number; i++)
//				{
//					vertexesUV[i] = vec2(vertexes[i].y(), vertexes[i].z());
//				}
//				vertexesUV[number] = vec2(pi.y(), pi.z());
//			}
//			else if (i == 2)
//			{
//				for (int i = 0; i < number; i++)
//				{
//					vertexesUV[i] = vec2(vertexes[i].x(), vertexes[i].z());
//				}
//				vertexesUV[number] = vec2(pi.x(), pi.z());
//			}
//			else if (i == 3)
//			{
//				for (int i = 0; i < number; i++)
//				{
//					vertexesUV[i] = vec2(vertexes[i].x(), vertexes[i].y());
//				}
//				vertexesUV[number] = vec2(pi.x(), pi.y());
//			}
//
//			for (int i = 0; i < number; i++)
//			{
//				vertexesUV[i] = vertexesUV[i] - vertexesUV[number];
//			}
//			vertexesUV[i] = vertexesUV[0];
//			if (inPolyonTest(vertexesUV, number))
//			{
//				if (t<tmax&&t>tmin)
//				{
//					rec.t = t;
//					rec.p = r.point_at_parameter(rec.t);
//					rec.normal = polyN;
//					rec.matPtr = ma;
//					return true;
//				}
//			}
//		}
//	}
//	virtual bool boundingBox(float t0, float t1, aabb& box)const
//	{
//		box=aabb()
//		return true;
//	}
//
//};
