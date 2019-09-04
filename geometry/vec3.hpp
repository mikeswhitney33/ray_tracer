#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <ostream>

/**
 * vec3: the basic structure representing a 3d vector
 * 
 * members:
 *  float x: the x component of the vector
 *  float y: the y component of the vector
 *  float z: the z component of the vector
 */
struct vec3 {
    float x, y, z;
    vec3(float x=0, float y=0, float z=0):x(x),y(y),z(z){}
};

/**
 * add: a basic addition between two floats. This is to be used
 *  in to pass the operation around as a function pointer.
 * 
 * args:
 *  float a: the first float 
 *  float b: the second float
 * 
 * return: the sum of a and b.
 */
float add(float a, float b) {
    return a + b;
}

/**
 * sub: a basic subtraction between two floats. This is to be used
 *  in to pass the operation around as a function pointer.
 * 
 * args:
 *  float a: the first float 
 *  float b: the second float
 * 
 * return: the difference between a and b.
 */
float sub(float a, float b) {
    return a - b;
}

/**
 * mul: a basic multiplication between two floats. This is to be used
 *  in to pass the operation around as a function pointer.
 * 
 * args:
 *  float a: the first float 
 *  float b: the second float
 * 
 * return: the product of a and b.
 */
float mul(float a, float b) {
    return a * b;
}

float div(float a, float b) {
    return a / b;
}

vec3 binop(const vec3 &a, const vec3 &b, float (*f)(float, float)) {
    return vec3(f(a.x, b.x), f(a.y, b.y), f(a.z, b.z));
}

vec3 binop(const vec3 &a, float b, float (*f)(float, float)) {
    return vec3(f(a.x, b), f(a.y, b), f(a.z, b));
}

vec3 binop(float a, const vec3 &b, float (*f)(float, float)) {
    return vec3(f(a, b.x), f(a, b.y), f(a, b.z));
}

vec3 operator+(const vec3 &a, const vec3 &b) {
    return binop(a, b, add);
}

vec3 operator+(const vec3 &a, float b) {
    return binop(a, b, add);
}

vec3 operator+(float a, const vec3 &b) {
    return binop(a, b, add);
}

vec3 operator-(const vec3 &a, const vec3 &b) {
    return binop(a, b, sub);
}

vec3 operator-(const vec3 &a, float b) {
    return binop(a, b, sub);
}

vec3 operator-(float a, const vec3 &b) {
    return binop(a, b, sub);
}

vec3 operator*(const vec3 &a, const vec3 &b) {
    return binop(a, b, mul);
}

vec3 operator*(const vec3 &a, float b) {
    return binop(a, b, mul);
}

vec3 operator*(float a, const vec3 &b) {
    return binop(a, b, mul);
}

vec3 operator/(const vec3 &a, const vec3 &b) {
    return binop(a, b, div);
}

vec3 operator/(const vec3 &a, float b) {
    return binop(a, b, div);
}

vec3 operator/(float a, const vec3 &b) {
    return binop(a, b, div);
}

vec3 &operator+=(vec3 &a, const vec3 &b) {
    a = a + b;
    return a;
}

vec3 &operator+=(vec3 &a, float b) {
    a = a + b;
    return a;
}

vec3 &operator-=(vec3 &a, const vec3 &b) {
    a = a - b;
    return a;
}

vec3 &operator-=(vec3 &a, float b) {
    a = a - b;
    return a;
}

vec3 &operator*=(vec3 &a, const vec3 &b) {
    a = a * b;
    return a;
}

vec3 &operator*=(vec3 &a, float b) {
    a = a * b;
    return a;
}

vec3 &operator/=(vec3 &a, const vec3 &b) {
    a = a / b;
    return a;
}

vec3 &operator/=(vec3 &a, float b) {
    a = a / b;
    return a;
}

float sum(const vec3 &a) {
    return a.x + a.y + a.z;
}

float max(const vec3 &a) {
    return a.x > a.y && a.x > a.z ? a.x : a.y > a.z ? a.y : a.z;
}

float min(const vec3 &a) {
    return a.x < a.y && a.x < a.z ? a.x : a.y < a.z ? a.y : a.z;
}

float dot(const vec3 &a, const vec3 &b) {
    return sum(a * b);
}

float mag(const vec3 &a) {
    return dot(a, a);
}

float len(const vec3 &a) {
    return sqrtf(mag(a));
}

vec3 norm(const vec3 &a) {
    return a / len(a);
}

vec3 cross(const vec3 &a, const vec3 &b) {
    return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

vec3 reflect(const vec3 &I, const vec3 &N) {
    return I - 2 * dot(I, N) * N;
}

vec3 refract(const vec3 &I, const vec3 &N, float eta) {
    float k = 1.0 - eta * eta * (1.0f - dot(N, I) * dot(N, I));
    if(k < 0.0f) {
        return vec3();
    }
    else {
        return eta * I - (eta * dot(N, I) + sqrtf(k)) * N;
    }
}

std::ostream &operator<<(std::ostream &os, const vec3 &a) {
    os << a.x << ' ' << a.y << ' ' << a.z << ' ';
    return os;
}

vec3 clamp(const vec3 &a) {
    return vec3(
        a.x > 1 ? 1 : a.x < 0 ? 0 : a.x,
        a.y > 1 ? 1 : a.y < 0 ? 0 : a.y,
        a.z > 1 ? 1 : a.z < 0 ? 0 : a.z
    );
}

vec3 toPixel(const vec3 &a) {
    vec3 c = 255 * clamp(a);
    return vec3((int) c.x, (int) c.y, (int) c.z);
}

#endif