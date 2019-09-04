#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <ostream>


struct vec3 {
    double x, y, z;
    vec3(double x=0, double y=0, double z=0);
    vec3 &norm();

    // vec3 operator%(vec3 &b) {
    //     return vec3(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y-y*b.x);
    // }
};

vec3 operator+(const vec3 &a, const vec3 &b);
vec3 operator+(const vec3 &a, double b);
vec3 operator+(double a, const vec3 &b);
vec3 operator-(const vec3 &a, const vec3 &b);
vec3 operator-(const vec3 &a, double b);
vec3 operator-(double a, const vec3 &b);
vec3 operator*(const vec3 &a, const vec3 &b);
vec3 operator*(const vec3 &a, double b);
vec3 operator*(double a, const vec3 &b);
vec3 operator/(const vec3 &a, const vec3 &b);
vec3 operator/(const vec3 &a, double b);
vec3 operator/(double a, const vec3 &b);
double dot(const vec3 &a, const vec3 &b);
double max(const vec3 &a);
vec3 cross(const vec3 &a, const vec3 &b);



#endif