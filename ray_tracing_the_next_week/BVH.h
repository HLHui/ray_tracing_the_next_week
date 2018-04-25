#pragma once
#include"hitable.h"
#include<algorithm>
using namespace std;


int boxXcompare(const void *a, const void *b)
{
	aabb boxLeft, boxRight;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
		cout << "no bounding box in bvhNode constructor" << endl;
	if (boxLeft.min().x() - boxRight.min().x() < 0.0)
		return -1;
	else
		return 1;
}
int boxYcompare(const void *a, const void *b)
{
	aabb boxLeft, boxRight;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
		cout << "no bounding box in bvhNode constructor" << endl;
	if (boxLeft.min().y() - boxRight.min().y() < 0.0)
		return -1;
	else
		return 1;
}
int boxZcompare(const void *a, const void *b)
{
	aabb boxLeft, boxRight;
	hitable *ah = *(hitable**)a;
	hitable *bh = *(hitable**)b;
	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
		cout << "no bounding box in bvhNode constructor" << endl;
	if (boxLeft.min().z() - boxRight.min().z() < 0.0)
		return -1;
	else
		return 1;
}

class bvhNode :public hitable 
{
public:
	aabb box;
	hitable* left;
	hitable* right;
	bvhNode() {}

	bvhNode(hitable** l, int n, float time0, float time1)
	{
		int axis = int(3 * rand() / (float)RAND_MAX);
		if (axis == 0)
		{
			qsort(l, n, sizeof(hitable*), boxXcompare);
		}
		else if (axis == 1)
		{
			qsort(l, n, sizeof(hitable*), boxYcompare);
		}
		else
		{
			qsort(l, n, sizeof(hitable*), boxZcompare);
		}
		if (n == 1)
		{
			left = right = l[0];
		}
		else if (n == 2)
		{
			left = l[0];
			right = l[1];
		}
		else
		{
			left = new bvhNode(l, n / 2, time0, time1);
			right = new bvhNode(l + n / 2, n - n / 2, time0, time1);
		}
		aabb boxLeft, boxRight;
		if(!left->boundingBox(time0,time1,boxLeft)||!right->boundingBox(time0,time1,boxRight))
			cout << "no bounding box in bvhNode constructor" << endl;
		box = surroundingBox(boxLeft, boxRight);
	}
	virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec)const
	{
		if (box.hit(r, tmin, tmax))
		{
			hitRecord leftRec, rightRec;
			bool hitLeft = left->hit(r, tmin, tmax, leftRec);
			bool hitRight = right->hit(r, tmin, tmax, rightRec);
			if (hitLeft && hitRight)
			{
				if (leftRec.t < rightRec.t)
				{
					rec = leftRec;
				}
				else
				{
					rec = rightRec;
				}
				return true;
			}
			else if(hitLeft)
			{
				rec = leftRec;
				return true;
			}
			else if (hitRight)
			{
				rec = rightRec;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	virtual bool boundingBox(float t0, float t1, aabb& b)const
	{
		b = box;
		return true;
	}

};

