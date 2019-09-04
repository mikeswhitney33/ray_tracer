#ifndef MATERIALS_HPP
#define MATERIALS_HPP

#include "../geometry/vec3.hpp"

class Material {
public:
    Material(const vec3 &color, const vec3 &emission);
    virtual ~Material();
    virtual vec3 radiance(const vec3 &f, const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, int E, vec3 (*radiance)(const vec3&, const vec3&, int, int)) const = 0;
    virtual vec3 incoming(const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, vec3 (*radiance)(const vec3&, const vec3&, int, int)) const = 0;
    vec3 getColor() const;
    vec3 getEmission() const;
private:
    vec3 color, emission;
};

#endif