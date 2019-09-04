#include "../../headers/materials/refractive-material.hpp"

RefractiveMaterial::RefractiveMaterial(const vec3 &color, const vec3 &emission)
    :Material(color, emission){}

vec3 RefractiveMaterial::incoming(const vec3 &x, const vec3 &rd, const vec3 &n, const vec3 &nl, int depth, vec3 (*radiance)(const vec3&, const vec3&, int)) const {
    vec3 refld = rd - n * 2 * dot(n, rd);     // Ideal dielectric REFRACTION
    bool into = dot(n, nl) > 0;                // Ray from outside going in?
    double nc = 1;
    double nt = 1.5;
    double nnt = into ? nc / nt : nt / nc;
    double ddn = dot(rd, nl);
    double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
    if (cos2t < 0) {   // Total internal reflection
        return radiance(x, refld, depth);
    }
    else {
        vec3 tdir = (rd * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();
        double a = nt - nc;
        double b = nt + nc;
        double R0 = a * a / (b * b);
        double c = 1 - (into ? -ddn : dot(tdir, n));
        double Re = R0 + (1 - R0) * c * c * c * c * c;
        double Tr = 1 - Re;
        double P = 0.25 + 0.5 * Re;
        double RP = Re / P;
        double TP = Tr / (1 - P);
        if(depth > 2) {
            if(drand48() < P) {
                return radiance(x, refld, depth) * RP;
            }
            else {
                return radiance(x, tdir, depth) * TP;
            }
        }
        else {
            return radiance(x, refld, depth) * Re +
                radiance(x, tdir, depth) * Tr;
        }
    }
}