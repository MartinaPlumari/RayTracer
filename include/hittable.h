#pragma once

#include "ray.h"

struct hit_record {
	point3 p;		//hit point
	vec3 normal;	//normal from the hit point
	double t;		//value of the t parameter
};

class hittable {
public:
	virtual ~hittable() = default;
	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};