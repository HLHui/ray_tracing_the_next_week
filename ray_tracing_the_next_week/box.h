#pragma once
#include"hitable.h"
#include"rectangle.h"
class box :public hitable
{
public:
	vec3 pmin, pmax;
	hitable *listMa;
	box(){}
	box(const vec3& p0, const vec3& p1, material *ma)
	{
		pmin = p0;
		pmax = p1;
		hitable **list = new hitable*[6];
		list[0] = new xyRect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ma);
		list[1] = new flipNormals(new xyRect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ma));
		list[2] = new xzRect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ma);
		list[3] = new flipNormals(new xzRect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ma));
		list[4] = new yzRect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ma);
		list[5] = new flipNormals(new yzRect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ma));
		listMa = new hitableList(list, 6);
	}

	virtual bool hit(const ray& r, float t0, float t1, hitRecord& rec)const
	{
		return listMa->hit(r, t0, t1, rec);
	}

	virtual bool boundingBox(float t0, float t1, aabb& box)const
	{
		box = aabb(pmin, pmax);
		return true;
	}
};