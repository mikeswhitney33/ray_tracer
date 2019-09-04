#include "../../headers/materials/specular-material.hpp"

SpecularMaterial::SpecularMaterial(const vec3 &color, const vec3 &emission)
    :Material(color, emission){}


vec3 SpecularMaterial::incoming(const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, vec3 (*radiance)(const vec3&, const vec3&, int)) const {
    return radiance(x, rd - n * 2 * dot(n, rd), depth);
}
