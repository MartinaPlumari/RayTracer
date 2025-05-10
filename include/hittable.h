#pragma once

struct hit_record {
	point3 p;		//hit point
	vec3 normal;	//normal from the hit point
	double t;		//value of the t parameter
	bool front_face;

	/* This function sets the hit record normal vector 
	/* we assume that the outward normal vector is of unit length -> normalization left to the geometry 
	/ (it is easier and maybe computationally better if we do this operation with knowledge of the geometry) */
	void set_face_normal(const ray& r, const vec3& outward_normal) 
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual ~hittable() = default;
	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};