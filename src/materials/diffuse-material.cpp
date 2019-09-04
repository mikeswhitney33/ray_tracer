#include "../../headers/materials/diffuse-material.hpp"
#include "../../headers/largept.hpp"
#include "../../headers/geometry/sphere.hpp"

DiffuseMaterial::DiffuseMaterial(const vec3 &color, const vec3 &emission):
    Material(color, emission){}

vec3 DiffuseMaterial::incoming(const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, vec3 (*radiance)(const vec3&, const vec3&, int, int)) const {
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

    return radiance(x, d, depth, 1);
}

vec3 DiffuseMaterial::radiance(const vec3 &f, const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, int E, vec3 (*rad)(const vec3&, const vec3&, int, int)) const {
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

    vec3 e;
    for(int i = 0;i < NUM_SHAPES;i++) {
        const Shape* shape = shapes[i];
        const Material* mat = shape->getMat();
        vec3 matE = mat->getEmission();
        if(matE.x <= 0 && matE.y <= 0 && matE.z <= 0) {
            continue;
        }
        vec3 sp = shape->getPos();
        double sr = shape->getRadius();

        vec3 sw = sp - x;
        vec3 su = cross((abs(sw.x) > 0.1 ? vec3(0, 1) : vec3(1)), sw).norm();
        vec3 sv = cross(sw, su);
        double cos_a_max = sqrt(1 - sr*sr / dot(x - sp, x - sp));
        double eps1 = drand48();
        double eps2 = drand48();
        double cos_a = 1 - eps1 + eps1 * cos_a_max;
        double sin_a = sqrt(1 - cos_a * cos_a);
        double phi = 2 * M_PI * eps2;
        vec3 l = su * cos(phi) * sin_a + sv * sin(phi) * sin_a + sw * cos_a;
        l.norm();
        double t;
        int id;
        if(intersect(x, l, t, id) && id == i) {
            double omega = 2 * M_PI * (1 - cos_a_max);
            e = e + (f * getEmission() * dot(l, nl) * omega) * M_1_PI;
        }
    }

    return getEmission() * E + e + f * rad(x, d, depth, 0);
}
