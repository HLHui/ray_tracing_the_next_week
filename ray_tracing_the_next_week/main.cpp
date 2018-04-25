#include<iostream>
#include<fstream>
#include<conio.h>
#include"camera.h"
#include"hitable.h"
#include"sphere.h"
#include"hitableList.h"
#include"float.h"
#include"moving_sphere.h"
#include"texture.h"
#include"BVH.h"
#include"material.h"
#include"rectangle.h"
#include"box.h"
#include"translate.h"
#include"triangle.h"
#include"polygon.h"
#include"tiny_obj_loader.h"

using namespace std;

vec3 color(const ray &r, hitable *world, int depth)
{
	hitRecord rec;
	if (world->hit(r, 0.001, (numeric_limits<float>::max)(), rec))
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
	}

//	make the background black

//	if (world->hit(r, 0.001, (numeric_limits<float>::max)(), rec))
//	{
//		ray scattered;
//		vec3 attenuation;
//		vec3 emitted = rec.matPtr->emitted(rec.u, rec.v, rec.p);
////		cout << emitted[0] << " " << emitted[1] << " " << emitted[2] << endl;
//		/*if (emitted[0] != 0)
//		{
//			cout << emitted[0] << " " << emitted[1] << " " << emitted[2] << endl;
//			_getch();
//		}*/
//		float pdf;
//		vec3 albedo;
//		if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered))
//		{
//			return emitted + attenuation *color(scattered, world, depth + 1);
////			return emitted+
//		}
//		else
//		{
//			
//			return emitted;
//		}
//	}
//	else
//	{
//		return vec3(0, 0, 0);
//	}

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
	/*texture *checker = new checkerTexture(new constantTexture(vec3(0.1, 0.1, 0.1)), new constantTexture(vec3(0.9, 0.9, 0.9)));
	int n = 50;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
	list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));
	return new hitableList(list, 2);*/

	texture *pertext = new noiseTexture();
	hitable **list = new hitable*[3];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	list[2] = new triangle(vec3(-8, 0, -2), vec3(-1, -2, -4), vec3(0, 5, -3), new metal(vec3(0.8, 0.6, 0.5), 0.0));

	return new hitableList(list, 3);
}

hitable *simpleLight()
{
	texture *pertext = new noiseTexture(10);
	hitable **list = new hitable*[10];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	list[2] = new sphere(vec3(0, 8, 0), 2, new diffuseLight(new constantTexture(vec3(1, 1, 1))));
	//set up a rectangle as a light
	list[3] = new xyRect(-1, 3, 1, 5, -4, new diffuseLight(new constantTexture(vec3(1, 1, 1))));
	return new hitableList(list, 4);
}

hitable *cornellBox()
{
	hitable **list = new hitable*[6];
	int i = 0;
	material *red = new lambertian(new constantTexture(vec3(0.5, 0, 0)));
	material *white = new lambertian(new constantTexture(vec3(1, 1, 1)));
	material *green = new lambertian(new constantTexture(vec3(0, 0.5, 0)));
	material *light = new diffuseLight(new constantTexture(vec3(1, 1, 1)));
	list[i++] = new flipNormals(new yzRect(0, 555, 0, 555, 555, green));
	list[i++] = new yzRect(0, 555, 0, 555, 0, red);
	list[i++] = new xzRect(113, 443, 177, 432, 554, light);
	list[i++] = new flipNormals(new xzRect(0, 555, 0, 555, 555, white));
	list[i++] = new xzRect(0, 555, 0, 555, 0, white);
	list[i++] = new flipNormals(new xyRect(0, 555, 0, 555, 555, white));	
//	list[i++] = new box(vec3(130, 0, 65), vec3(295, 165, 230), white);
//	list[i++] = new box(vec3(265, 0, 295), vec3(430, 330, 460), white);
	list[i++] = new translate(new rotateY(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -15), vec3(130, 0, 65));
	list[i++] = new translate(new rotateY(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(256, 0, 295));

//	hitable *b = new translate(new rotateY(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(256, 0, 295));
//	list[i++] = new constantMedium(b, 0.01, new constantTexture(vec3(0.5, 0.5, 0.5)));
	return new hitableList(list, i);
}

hitable *final()
{
	int nb = 20;
	hitable **list = new hitable*[30];
	hitable **boxlist = new hitable*[10000];
	hitable **boxlist2 = new hitable*[10000];
	material *white = new lambertian(new constantTexture(vec3(1, 1, 1)));
	material *ground = new lambertian(new constantTexture(vec3(0.48, 0.83, 0.53)));
	int b = 0;
	for (int i = 0; i < nb; i++)
	{
		for (int j = 0; j < nb; j++)
		{
			float w = 100;
			float x0 = -1000 + i*w;
			float z0 = -1000 + j*w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (rand() / float(RAND_MAX) + 0.01);
			float z1 = z0 + w;
			boxlist[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
		}
	}
	int i = 0;
	list[i++] = new bvhNode(boxlist, b, 0, 1);
	material *light = new diffuseLight(new constantTexture(vec3(1, 1, 1)));
	list[i++] = new xzRect(123, 423, 147, 412, 554, light);
	vec3 center(400, 400, 200);
	list[i++] = new movingSphere(center, center + vec3(30, 0, 0), 0, 1, 50, new lambertian(new constantTexture(vec3(0.7, 0.3, 0.1))));
	list[i++] = new sphere(vec3(255, 150, 45), 50, new metal(vec3(0.8, 0.8, 0.9), 10.0));
	hitable *boundary = new sphere(vec3(350, 160, 150), 70, new dielectric(1.5));
	list[i++] = boundary;
	hitable *tra = new translate(new rotateY(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(256, 0, 295));
	list[i++] = new constantMedium(tra, 0.01, new constantTexture(vec3(0.5, 0.5, 0.5)));
	list[i++] = new triangle(vec3(-8, 0, -2), vec3(-1, -2, -4), vec3(0, 5, -3), new metal(vec3(0.8, 0.6, 0.5), 0.0));

	return new hitableList(list, i);
}

//hitable *triangle()
//{
//	texture *pertext = new noiseTexture();
//	hitable **list = new hitable*[1];
//	list[0] = new Triangle(vec3(-1, -1,-5), vec3(1, -1, -5), vec3(0, 1, -5), new lambertian(pertext));
//	return new hitableList(list, 1);
//}

hitable *triangles()
{
	texture *pertext = new noiseTexture();
	hitable **list = new hitable*[3];

	int i = 0;
	list[i++] = new triangle(vec3(-18, 2, -4), vec3(-10, -2, -4), vec3(-8, 5, -3), new metal(vec3(0.8, 0.6, 0.5), 0.0));
	list[i++] = new triangle(vec3(-4, 0, -2), vec3(3, -2, -4), vec3(4, 5, -3), new dielectric(1.5));
	list[i++] = new triangle(vec3(5, 0, -2), vec3(8, 0, -4), vec3(9, 5, -3), new lambertian(pertext));
//	list[i++] = new triangle(vec3(-1, 1.5, -1), vec3(-3, 1.5, -1), vec3(-2, 3.5, -1), new lambertian(new constantTexture(vec3(rand() / float(RAND_MAX)*rand() / float(RAND_MAX), rand() / float(RAND_MAX)*rand() / float(RAND_MAX), rand() / float(RAND_MAX)*rand() / float(RAND_MAX)))));
	list[i++] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	list[i++] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
//	list[i++] = new sphere(vec3(2, 1.0, 0), 1.0, new dielectric(1.5));
//	list[i++] = new sphere(vec3(2, 1.0, 0), -0.9, new dielectric(1.5));

	return new hitableList(list, i);

}
int main()
{
	int res = 2;
	int nx = 2000 / res;
	int ny = 1000 / res;
	int ns = 1000 / res;
//	hitable *world = randomScene();
//	hitable *world = twoSphere();
//	hitable *world = simpleLight();
//	hitable *world = cornellBox();
//	hitable *world = final();

	hitable *world = triangles();

	ofstream img;
	img.open("C:\\ray tracing image\\triangle4.ppm");
//	img.open("C:\\ray tracing image\\texture2.ppm");
//	img.open("C:\\ray tracing image\\translate.ppm");

	img << "P3" << endl << nx << " " << ny << endl << "255" << endl;
//	vec3 lookfrom(5, 2, 6);

	vec3 lookfrom(10, 3, 10);
	vec3 lookat(0, 0, 0);


//	vec3 lookfrom(278, 278, -600);
//	vec3 lookat(278, 278, 0);

//	vec3 lookfrom(0, 0, 10);
//	vec3 lookat(0, 1, -1);

	float diskToFocus = (lookfrom - lookat).length();
//	float diskToFocus = 10;
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

//			cout << ir << " , " << ig << " , " << ib << endl;
			img << ir << " " << ig << " " << ib << endl;
		}
	}
	img.close();
	return 0;
}