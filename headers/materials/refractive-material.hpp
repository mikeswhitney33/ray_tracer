#ifndef REFRACTIVE_MATERIAL_HPP
#define REFRACTIVE_MATERIAL_HPP

#include "material.hpp"

class RefractiveMaterial : public Material {
public:
    RefractiveMaterial(const vec3 &color, const vec3 &emission);
    virtual vec3 incoming(const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, vec3 (*radiance)(const vec3&, const vec3&, int)) const;
};

#endif