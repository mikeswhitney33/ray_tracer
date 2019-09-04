#ifndef SHAPE_FACTORY_HPP
#define SHAPE_FACTORY_HPP

#include "../geometry/shape.hpp"

Shape* makeSphere(Material* mat, const vec3 &position, double radius);

#endif