#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "shape.hpp"

class Sphere: public Shape {
public:
    Sphere(Material* mat, const vec3 &position, double radius);
    virtual double intersect(const vec3 &ro, const vec3 &rd) const;
    virtual vec3 getNormal(const vec3 &rd, const vec3 &pt) const;
    virtual vec3 getPos() const;
    virtual double getRadius() const;
private:
    vec3 position;
    double radius;
    double radius2;
};

#endif