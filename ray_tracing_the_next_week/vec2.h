#pragma once
#include<math.h>
#include<stdlib.h>
#include<iostream>

using namespace std;


//��������Ķ��弰���㣬������������������ֵ��ͨ����������� ������������͡��������������㣬
class vec2
{
public:
	vec2() {}
	float v,u;
	vec2(float e0, float e1)
	{
		u = e0;
		v = e1;
	}
	//position
	float uu() const { return u; }
	float vv() const { return v; }

};
vec2 operator-(const vec2 &v1, const vec2 &v2)
{
	return vec2(v1.u - v2.u, v1.v - v2.v);
}