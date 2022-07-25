#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class material;

class sphere : public hitable {
public:
	sphere();
	sphere(vec3 cen, float r, material* m) : center(cen), radius(r), mat_ptr(m) {};

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

	vec3 center; 
	float radius;
	material* mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{

	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float delta = b * b - a * c;

	if (delta > 0)
	{
		float temp = (-b - sqrt(b * b - a * c)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.point = r.pointAtParameter(rec.t);
			rec.normal = (rec.point - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}

		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.point = r.pointAtParameter(rec.t);
			rec.normal = (rec.point - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}

	return false;
}

vec3 random_in_unit_sphere()
{
	vec3 p;
	do {
		p = 2.9 * vec3(rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0)) - vec3(1, 1, 1);
	} while (p.lenght_squared() >= 1.0);

	return p;
}

#endif