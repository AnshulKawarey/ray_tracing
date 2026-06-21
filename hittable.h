#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "ray.h"

// abstraction to include all objects that are hittable
// defines a virtual function for hit which takes in a ray
// for efficiency, a lot other ray tracers include a min to max range such that if the ray is between them only then the hit counts

class material;

class hit_record{
    public:
        point3 p;
        vec3 normal;
        shared_ptr<material> mat;
        float t;
        bool front_face;

        void set_face_normal(const ray& r, const vec3& outward_normal){
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : - outward_normal; 
        }
};

class hittable{
    public:
        virtual ~hittable() = default;
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
} ;

#endif