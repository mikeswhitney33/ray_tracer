#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "vec3.hpp"

#define EPS 1e-8

enum Material {DIFF, SPEC, REFR};

class Shape {
public:
    Shape(vec3 emission, vec3 color, Material mat):emission(emission), color(color), mat(mat){}
    vec3 getColor() {
        return color;
    }
    vec3 getEmission() {
        return emission;
    }
    Material getMat() {
        return mat;
    }
    virtual bool intersect(const vec3 &ray_orig, const vec3 &ray_dir, float &min_t) = 0;
    virtual vec3 getNormal(const vec3 &ray_dir, const vec3 &pt) = 0;
private:
    vec3 color, emission;
    Material mat;
};

class Sphere : public Shape {
public:
    Sphere(vec3 emission, vec3 color, Material mat, vec3 center, float radius):Shape(emission, color, mat),center(center), radius(radius) {
        radius2 = radius*radius;
    }
    virtual bool intersect(const vec3 &ray_orig, const vec3 &ray_dir, float &min_t) {
        vec3 OC = center - ray_orig;
        float t_ca = dot(OC, ray_dir);
        if(t_ca < 0) return false;
        float d2 = dot(OC, OC) - t_ca * t_ca;
        if(d2 > radius2) return false;
        float t_hc = sqrtf(radius2 - d2);
        float t0 = t_ca - t_hc;
        float t1 = t_ca + t_hc;
        if(t1 < t0) std::swap(t0, t1);
        if(t0 < 0) {
            t0 = t1;
            if (t0 < 0) return false;
        }
        if (t0 > min_t) return false;
        min_t = t0;
        return true;
    }

    virtual vec3 getNormal(const vec3 &ray_dir, const vec3 &pt) {
        return (pt - center) / radius;
    }
private:
    vec3 center;
    float radius, radius2;
};

class Triangle : public Shape {
public:
    Triangle(const vec3 &emission, const vec3 &color, Material mat, const vec3 &A, const vec3 &B, const vec3 &C):
        Shape(emission, color, mat), A(A), B(B), C(C){}

    virtual bool intersect(const vec3 &ray_orig, const vec3 &ray_dir, float &min_t) {
        vec3 AB = B - A;
        vec3 AC = C - A;
        vec3 pvec = cross(ray_dir, AC);
        float det = dot(AB, pvec);
        if(fabs(det) < EPS) return false;
        float inv_det = 1.0f / det;
        vec3 tvec = ray_orig - A;
        float u = dot(tvec, pvec) * inv_det;
        if(u < 0 || u > 1) return false;
        vec3 qvec = cross(tvec, AB);
        float v = dot(ray_dir, qvec) * inv_det;
        if(v < 0 || u + v > 1) return false;
        float t = dot(AC, qvec) * inv_det;
        if(t < 0) return false;
        if(t > min_t) return false;
        min_t = t;
        return true;
    }

    virtual vec3 getNormal(const vec3 &ray_dir, const vec3 &pt) {
        vec3 n = norm(cross(B-A, C-A));
        if(dot( n, ray_dir) < 0) n *= -1;
        return n;
    }
private:
    vec3 A, B, C;
};

#endif