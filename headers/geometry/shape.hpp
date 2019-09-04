#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "vec3.hpp"
#include "../materials/material.hpp"

#define EPS 1e-4


class Shape {
public:
    Shape(Material* mat);
    virtual ~Shape();
    virtual double intersect(const vec3 &ro, const vec3 &rd) const = 0;
    virtual vec3 getNormal(const vec3 &rd, const vec3 &pt) const  = 0;
    Material* getMat() const;
private:
    Material* mat;
};


#endif