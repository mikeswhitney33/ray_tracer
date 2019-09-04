#include "../../headers/geometry/sphere.hpp"

Sphere::Sphere(Material* mat, const vec3 &position, double radius)
    :Shape(mat), position(position), radius(radius), radius2(radius*radius){}

double Sphere::intersect(const vec3 &ro, const vec3 &rd) const {
    vec3 op = position-ro;
    double t;
    double b = dot(op, rd);
    double det = b * b - dot(op, op) + radius2;
    if (det<0) {
        return 0;
    }
    else {
        det=sqrt(det);
    }
    t = b - det;
    if(t > EPS) {
        return t;
    }
    t = b + det;
    if(t > EPS) {
        return t;
    }
        return 0;
}

vec3 Sphere::getNormal(const vec3 &rd, const vec3 &pt) const  {
    return (pt - position) / radius;
}

vec3 Sphere::getPos() const {
    return position;
}

double Sphere::getRadius() const {
    return radius;
}
