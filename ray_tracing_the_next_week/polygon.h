#pragma once
#include "hitable.h"
#include "vec2.h"

bool inPolyonTest(vec2 *vertexesUV, int number)
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
}

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
		/*vec3 polyN;
		polyN = unitVector(cross((v0 - v1), (v1 - v2)));
		float polyD = -(dot(polyN, v0));
		float vd = dot(polyN, r.dir);
		float v00 = -(dot(polyN, r.ori) + polyD);
		if (vd == 0)
		{
			return false;
		}
		else
		{
			float t = v00 / vd;
			vec3 pi = r.point_at_parameter(t);
			float temp = abs(polyN.x());
			int i = 1;
			if (temp <= abs(polyN.y()))
			{
				temp = abs(polyN.y());
				i++;
			}
			if (temp <= abs(polyN.z()))
			{
				i++;
			}
			vec2 vertexesUV[4];
			if (i == 1)
			{
				vertexesUV[0] = vec2(v0.y(), v0.z());
				vertexesUV[1] = vec2(v1.y(), v1.z());
				vertexesUV[2] = vec2(v2.y(), v2.z());
				vertexesUV[3] = vec2(pi.y(), pi.z());
			}
			else if (i == 2)
			{
				vertexesUV[0] = vec2(v0.x(), v0.z());
				vertexesUV[1] = vec2(v1.x(), v1.z());
				vertexesUV[2] = vec2(v2.x(), v2.z());

				vertexesUV[3] = vec2(pi.x(), pi.z());
			}
			else if (i == 3)
			{
				vertexesUV[0] = vec2(v0.x(), v0.y());
				vertexesUV[1] = vec2(v1.x(), v1.y());
				vertexesUV[2] = vec2(v2.x(), v2.y());

				vertexesUV[3] = vec2(pi.x(), pi.y());
			}

			vertexesUV[0] = vertexesUV[0] - vertexesUV[3];
			vertexesUV[1] = vertexesUV[1] - vertexesUV[3];
			vertexesUV[2] = vertexesUV[2] - vertexesUV[3];

			vertexesUV[i] = vertexesUV[0];
			if (inPolyonTest(vertexesUV, 4))
			{
				if (t<tmax&&t>tmin)
				{
					rec.t = t;
					rec.p = r.point_at_parameter(rec.t);
					rec.normal = polyN;
					rec.matPtr = ma;
					return true;
				}
			}
			return false;
		}*/

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

		/*float thist, t, u, v;
		vec3 v0v1 = v1 - v0;
		vec3 v0v2 = v2 - v0;
		vec3 pvec = cross(r.dir, v0v2);
		float det = dot(pvec, v0v1);
		float kEpsilon = 0.00001;
		if (det < kEpsilon)return false;
		float inDet = 1 / det;
		vec3 tvec = r.ori - v0;
		u = dot(tvec, pvec)*inDet;
		if (u < 0 || u>1)
		{
			return false;
		}
		vec3 qvec = cross(tvec, v0v1);
		v = dot(r.dir, qvec)*inDet;
		if (v < 0 || u + v>1)
		{
			return false;
		}
		t = dot(v0v2, qvec)*inDet;
		if (t < 0)
		{
			return false;
		}

		rec.p = r.point_at_parameter(t);
		rec.t = t;
		rec.normal = normal;
		rec.matPtr = ma;
		return true;*/

	}
	virtual bool boundingBox(float t0, float t1, aabb& box)const
	{
		vec3 bl = vec3(min(min(v0[0], v1[0]), v2[0]), min(min(v0[1], v1[1]), v2[1]), min(min(v0[2], v1[2]), v2[2]));
		vec3 tr = vec3(max(max(v0[0], v1[0]), v2[0]), max(max(v0[1], v1[1]), v2[1]), max(max(v0[2], v1[2]), v2[2]));
		box = aabb(bl, tr);
		return true;
	}
};


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
