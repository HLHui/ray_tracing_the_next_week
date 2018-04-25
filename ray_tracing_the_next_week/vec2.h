#pragma once
#include<math.h>
#include<stdlib.h>
#include<iostream>

using namespace std;


//点和向量的定义及运算，设置向量的三个坐标值，通过运算符重载 对向量进行求和、求差、点积、叉积运算，
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