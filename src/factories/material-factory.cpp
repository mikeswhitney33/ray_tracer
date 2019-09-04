#include "../../headers/factories/material-factory.hpp"
#include "../../headers/materials/diffuse-material.hpp"
#include "../../headers/materials/specular-material.hpp"
#include "../../headers/materials/refractive-material.hpp"

Material* makeDiffuseMaterial(const vec3 &color, const vec3 &emission) {
    return new DiffuseMaterial(color, emission);
}

Material* makeSpecularMaterial(const vec3 &color, const vec3 &emission) {
    return new SpecularMaterial(color, emission);
}

Material* makeRefractiveMaterial(const vec3 &color, const vec3 &emission) {
    return new RefractiveMaterial(color, emission);
}
