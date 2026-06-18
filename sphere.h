#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "interval.h"

class sphere : public hittable {
public:
  sphere(const point3 &_center, double _radius, shared_ptr<material> mat) : center(_center), radius(std::fmax(0, _radius)), mat(mat) {}
  bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
    // The math behond it is the sphere equation of x^2 + y^2 + z^2 = r^2,
    // Which with 2 points just becomes (x1-x2)^2 and resp.for y and z,
    // so when we put it in Vector terms with 3 Dimensions it is simply
    // (A-B).(A-B) = r^2 which when put in our terms is C (center) and P (point)
    // so (C-P).(C-P) = r^2 so every P which satisfies this lies on the sphere
    // if our ray P(t)=Q+td
    // (C−P(t))⋅(C−P(t))=r^2 which can be found by replacing P(t)
    // with its expanded form: (C−(Q+td))⋅(C−(Q+td))=r^2
    // instead of putting this equation and finding 6 resultant vectors
    // we can solve for t since that is the only unknown in this equation
    // t^2d⋅d−2td⋅(C−Q)+(C−Q)⋅(C−Q)−r^2=0
    // here to see if it hits or not we just check the determinant of b^2 - 4ac
    // to be 0 or greater and to return the point, we calc -b-discriminant / 2 a
    // a = d * d
    // b = -2 * d * (C-Q)
    // but we can simplify by considering b = -2h
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = h * h - a * c;

    if (discriminant < 0) {
      return false;
    }

    auto sqrtd = std::sqrt(discriminant);

    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      root = (h + sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        return false;
      }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return true;
  }

private:
  point3 center;
  double radius;
  shared_ptr<material> mat;
};

#endif