#include "../../headers/materials/material.hpp"

Material::Material(const vec3 &color, const vec3 &emission)
    :color(color), emission(emission){}

Material::~Material(){}

vec3 Material::getColor() const {
    return color;
}

vec3 Material::getEmission() const {
    return emission;
}