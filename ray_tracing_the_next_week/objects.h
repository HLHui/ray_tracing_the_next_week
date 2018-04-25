#pragma once
#include"vec3.h"
#include"ray.h"
#include"triangle.h"
#include"kdtree.h"
#include"BVH.h"
#include"material.h"
#include"tiny_obj_loader.h"


struct ObjectIntersection
{
	bool hit;
	float distance;
	vec3 normal;
	material *m;

	ObjectIntersection(bool _hit, float _distance, vec3 _normal, material *_m)
	{
		hit = _hit;
		distance = _distance;
		normal = _normal;
		m = _m;
	}
};

class Object
{
public:
	vec3 mPosition;
	virtual ObjectIntersection getIntersection(const ray &r) = 0;
};

//class Mesh :public Object
//{
//private:
//	vector<tinyobj::shape_t> mShapes;
//	vector<tinyobj::material_t> mMaterials;
//	vector<material> materials;
//	vector<Triangle> tris;
//	material *m;
//	bvhNode bvh;
//public:
//	Mesh(vec3 _p, const char *filePath, material *_m)
//	{
//		mPosition = _p;
//		m = _m;
//		string mtlbasepath;
//		string inputfile = filePath;
//		unsigned long pos = inputfile.find_first_of("/");
//		mtlbasepath = inputfile.substr(0, pos + 1);
//		cout << "Loading " << filePath << "..." << endl;
//
//		//load mesh
//		string err = tinyobj::LoadObj(mShapes, mMaterials, inputfile.c_str(), mtlbasepath.c_str());
//		if (err.empty())
//		{
//			cerr << err << endl;
//			exit(1);
//		}
//
//		cout << " - Generating k-d tree..." << endl << endl;
//
//		long shapesSize, indicesSize, materialSize;
//		shapesSize = mShapes.size();
//		materialSize = mMaterials.size();
//
//		//load material/texture from obj;
//
//		for (int i = 0; i < materialSize; i++)
//		{
//			string texturePath = "";
//
//			if (!mMaterials[i].diffuse_texname[0] == '/')
//			{
//				texturePath = mMaterials[i].diffuse_texname;
//			}
//			texturePath = mtlbasepath + mMaterials[i].diffuse_texname;
//			materials.push_back()
//		}
//
//	}
//	virtual ObjectIntersection getIntersection(const ray &r)
//	{
//		float t = 0, tmin = INFINITY;
//		vec3 normal;
//		vec3 color;
//		bool hit = false;
//
//	}
//};