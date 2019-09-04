#include "../../headers/geometry/shape.hpp"

Shape::Shape(Material* mat)
    :mat(mat){}

Shape::~Shape() {
    delete mat;
}

Material* Shape::getMat() const {
    return mat;
}
