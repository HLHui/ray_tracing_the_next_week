#pragma once

#include "hitable.h"
#include"aabb.h"
class hitableList:public hitable
{
public:
	hitableList(){}
	hitable **list;
	int listSize;
	virtual bool hit(const ray&r, float tMin, float tMax, hitRecord&rec) const 
	{
		hitRecord tempRec;
		bool hitAnything = false;
		double closestSoFar = tMax;
		for (int i = 0; i < listSize; i++)
		{
			if (list[i]->hit(r, tMin, closestSoFar, tempRec))
			{
				hitAnything = true;
				closestSoFar = tempRec.t;
				rec = tempRec;
			}
		}

		return hitAnything;
	}
	virtual bool boundingBox(float t0, float t1, aabb& box)const
	{
		if (listSize < 1)return false;
		aabb tempBox;
		bool firstTrue = list[0]->boundingBox(t0, t1, tempBox);
		if (!firstTrue)
		{
			return false;
		}
		else
		{
			box = tempBox;
		}
		for (int i = 1; i < listSize; i++)
		{
			if (list[0]->boundingBox(t0, t1, tempBox))
			{
				box = surroundingBox(box, tempBox);
			}
			else
				return false;
		}
		return true;

	}
	hitableList(hitable **l, int n)
	{
		list = l;
		listSize = n;
	}

};


///*依次判断列表中所有物体是否被光线撞到，每次判断一个。若有被撞到，则将撞点信息保存在hit_record结构体中。我们可以看到rec是可能被写多次的，最终保存的值是后一次的值，也就是真正有效的值是后一次的值，也就是离观测点最近的物体的有效撞点（“有效撞点”：对于单个物体，会筛选出一个局部有效撞点；对于多个物体，从所有单个物体各自的局部有效撞点筛选出最终一个整体有效撞点）。因为不管这条光线依次撞击了多少个物体产生多少个撞点，我们能看到的只是离我们最近的撞点* /
///*如果当前撞点在范围内，则将当前撞点的距离设置为范围的最大值。也就是后面只考虑比该撞点更近的撞点。趋势是：找到的撞点是越来越近的，最终找到最近的撞点。*/  
