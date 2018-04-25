#pragma once
#include<vector>
#include"ray.h"
#include"triangle.h"
using namespace std;
class KDNode
{
public:
	aabb box;
	KDNode *left;
	KDNode *right;
	vector<Triangle*> triangles;
	bool leaf;
	KDNode() {};
	KDNode *build(vector<Triangle*> &tris, int depth)
	{
		KDNode *node = new KDNode();
		node->leaf = false;
		node->triangles = vector<Triangle*>();
		node->left = NULL;
		node->right = NULL;
		node->box = aabb();
		if (tris.size() == 0)
		{
			return node;
		}
		if (depth > 25 || tris.size() <= 6)
		{
			node->triangles = tris;
			node->leaf = true;
			node->box = tris[0]->getBoundingBox();
			for (long i = 1; i < tris.size(); i++)
			{
				node->box.expand(tris[i]->getBoundingBox());
			}
			node->left = new KDNode();
			node->right = new KDNode();
			node->left->triangles = vector<Triangle*>();
			node->right->triangles = vector<Triangle*>();
			return node;
		}

		node->box = tris[0]->getBoundingBox();
		vec3 midpt;
		float trisRecp = 1.0 / tris.size();
		for (long i = 1; i < tris.size(); i++)
		{
			node->box.expand(tris[i]->getBoundingBox());
			midpt = midpt + (tris[i]->getMidpoint()*trisRecp);
		}

		vector<Triangle*> leftTris;
		vector<Triangle*> rightTris;
		int axis = node->box.getLongestAxis();

		for (long i = 0; i < tris.size(); i++)
		{
			if (axis == 0)
			{
				if (midpt.x >= tris[i]->getMidpoint().x)
				{
					rightTris.push_back(tris[i]);
				}
				else
				{
					leftTris.push_back(tris[i]);
				}
			}
			else if (axis == 1)
			{
				if (midpt.y >= tris[i]->getMidpoint().y)
				{
					rightTris.push_back(tris[i]);
				}
				else
				{
					leftTris.push_back(tris[i]);
				}
			}
			else if (axis == 2)
			{
				if (midpt.z >= tris[i]->getMidpoint().z)
				{
					rightTris.push_back(tris[i]);
				}
				else
				{
					leftTris.push_back(tris[i]);
				}
			}
		}

		if (tris.size() == leftTris.size() || tris.size() == rightTris.size())
		{
			node->triangles = tris;
			node->leaf = true;
			node->box = tris[0]->getBoundingBox();

			for (long i = 1; i < tris.size(); i++)
			{
				node->box.expand(tris[i]->getBoundingBox());
			}

			node->leaf = new KDNode();
			node->right = new KDNode();
			node->left->triangles = vector<Triangle*>();
			node->right->triangles = vector<Triangle*>();

			return node;
		}
	}

	bool hit(KDNode *node, const ray &r, float &tmin, float tmax, vec3 &normal, vec3 &c)
	{
		float dist;
		if (node->box.hit(r, tmin, tmax))
		{

		}
	}
};