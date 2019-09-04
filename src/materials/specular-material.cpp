#include "../../headers/materials/specular-material.hpp"

SpecularMaterial::SpecularMaterial(const vec3 &color, const vec3 &emission)
    :Material(color, emission){}


vec3 SpecularMaterial::incoming(const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, vec3 (*radiance)(const vec3&, const vec3&, int, int)) const {
    return radiance(x, rd - n * 2 * dot(n, rd), depth, 1);
}

vec3 SpecularMaterial::radiance(const vec3 &f, const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, int E, vec3 (*rad)(const vec3&, const vec3&, int, int)) const {
    return getEmission() + f * rad(x, rd - n * 2 * dot(n, rd), depth, 1);
}
