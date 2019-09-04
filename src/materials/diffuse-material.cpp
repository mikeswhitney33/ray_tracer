#include "../../headers/materials/diffuse-material.hpp"

DiffuseMaterial::DiffuseMaterial(const vec3 &color, const vec3 &emission):
    Material(color, emission){}

vec3 DiffuseMaterial::incoming(const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, vec3 (*radiance)(const vec3&, const vec3&, int)) const {
    double r1 = 2 * M_PI * drand48();
    double r2 = drand48();
    double r2s = sqrt(r2);
    vec3 w = nl;
    vec3 u = cross((fabs(w.x) > .1 ? vec3(0,1) : vec3(1)) , w).norm();
    vec3 v = cross(w , u);
    vec3 d = (
        u * cos(r1) * r2s +
        v * sin(r1) * r2s +
        w * sqrt(1-r2)
    ).norm();
    return radiance(x, d, depth);
}
