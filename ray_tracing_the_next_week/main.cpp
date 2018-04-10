#include<iostream>
#include<fstream>
#include"camera.h"
#include"hitable.h"
#include"sphere.h"
#include"hitableList.h"
#include"float.h"
#include"moving_sphere.h"
#include"texture.h"
#include"BVH.h"
#include"material.h"
using namespace std;

vec3 color(const ray &r, hitable *world, int depth)
{
	hitRecord rec;
	/*if (world->hit(r, 0.001, (numeric_limits<float>::max)(), rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation *color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unitDirection = unitVector(r.direction());
		float t = 0.5*(unitDirection.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
	}*/
	if (world->hit(r, 0.001, (numeric_limits<float>::max)(), rec))
	{
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.matPtr->emitted(rec.u, rec.v, rec.p);

		if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered))
		{
			return emitted + attenuation *color(scattered, world, depth + 1);
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		return vec3(0, 0, 0);
	}
}

hitable *randomScene()
{
	int n = 500;
	hitable **list = new hitable *[n + 1];
	texture *checker = new checkerTexture(new constantTexture(vec3(0.2, 0.3, 0.1)), new constantTexture(vec3(0.9, 0.9, 0.9)));
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float chooseMat = rand() / float(RAND_MAX);
			vec3 center(a + 0.9*rand() / float(RAND_MAX), 0.2, b + 0.9*rand() / float(RAND_MAX));

			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				float rad = 0.2;
				if (chooseMat < 0.7)
				{
					list[i++] = new movingSphere(center, center + vec3(0, rand() / float(RAND_MAX), 0), 0.0, 2.0, 0.2,
						new lambertian(new constantTexture(vec3(rand() / float(RAND_MAX)*rand() / float(RAND_MAX), rand() / float(RAND_MAX)*rand() / float(RAND_MAX), rand() / float(RAND_MAX)*rand() / float(RAND_MAX)))));
				}
				else if (chooseMat < 0.95)
				{
					list[i++] = new sphere(center, rad, new metal(vec3(0.5*(1 + rand() / float(RAND_MAX)), 0.5*(1 + rand() / float(RAND_MAX)), 0.5*(1 + rand() / float(RAND_MAX))), 0.5*(1 + rand() / float(RAND_MAX))));
				}
				else
				{
					list[i++] = new sphere(center, rad, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(2, 1.0, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(2, 1.0, 0), -0.9, new dielectric(1.5));
	list[i++] = new sphere(vec3(-2, 1, -3), 1.0, new lambertian(new constantTexture(vec3(0.4, 0.2, 0.6))));
	list[i++] = new sphere(vec3(-4, 1.0, 2), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(0, 1, 2), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));


	return new hitableList(list, i);
}

hitable *twoSphere()
{
	//
	/*texture *checker = new checkerTexture(new constTexture(vec3(0.1, 0.1, 0.1)), new constTexture(vec3(0.9, 0.9, 0.9)));
	int n = 50;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
	list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));
	return new hitableList(list, 2);*/

	texture *pertext = new noiseTexture();
	hitable **list = new hitable*[2];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	return new hitableList(list, 2);
}

hitable *simpleLight()
{
	texture *pertext = new noiseTexture(4);
	hitable **list = new hitable*[4];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	list[2] = new sphere(vec3(0, 7, 0), 2, new diffuseLight(new constantTexture(vec3(4,4,4))));
	list[3] = new xyRect(3, 5, 1, 3, -2, new diffuseLight(new constantTexture(vec3(4, 4, 4))));
	return new hitableList(list, 4);
}

int main()
{
	int res = 2;
	int nx = 2000 / res;
	int ny = 1000 / res;
	int ns = 1000 / res;
//	hitable *world = randomScene();
//	hitable *world = twoSphere();
	hitable *world = simpleLight();

	ofstream img;
	img.open("C:\\ray tracing image\\simple_light.ppm");
//	img.open("C:\\ray tracing image\\texture2.ppm");

	img << "P3" << endl << nx << " " << ny << endl << "255" << endl;
	vec3 lookfrom(5, 2, 6);
//	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float diskToFocus = (lookfrom - lookat).length();
	float aperture = 0.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 60, float(nx) / float(ny), 0.1, 0.9*diskToFocus, 0.0, 1.0);
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				//				float randNum = rand() % (100) / float(100);  
				float u = float(i + rand() / float(RAND_MAX)) / float(nx);
				float v = float(j + rand() / float(RAND_MAX)) / float(ny);
				//col = vec3(u, v, 0) * ns;break;
				ray r = cam.getRay(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			img << ir << " " << ig << " " << ib << endl;
		}
	}
	img.close();
	return 0;
}