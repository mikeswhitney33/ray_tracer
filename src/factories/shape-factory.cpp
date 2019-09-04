#include "../../headers/factories/shape-factory.hpp"
#include "../../headers/geometry/sphere.hpp"

Shape* makeSphere(Material* mat, const vec3 &position, double radius) {
    return new Sphere(mat, position, radius);
}