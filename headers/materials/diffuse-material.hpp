#ifndef DIFFUSE_MATERIAL_HPP
#define DIFFUSE_MATERIAL_HPP

#include "material.hpp"

class DiffuseMaterial : public Material {
public:
    DiffuseMaterial(const vec3 &color, const vec3 &emission);
    virtual vec3 radiance(const vec3 &f, const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, int E, vec3 (*radiance)(const vec3&, const vec3&, int, int)) const;
    virtual vec3 incoming(const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, vec3 (*radiance)(const vec3&, const vec3&, int, int)) const;
};

#endif