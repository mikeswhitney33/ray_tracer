#include "../../headers/geometry/vec3.hpp"

vec3::vec3(double x, double y, double z)
    :x(x), y(y), z(z){}

vec3 &vec3::norm() {
    return *this = *this * (1 / sqrt(x*x + y*y + z*z));
}

double add(double a, double b){
    return a + b;
}
double sub(double a, double b){
    return a - b;
}
double mul(double a, double b){
    return a * b;
}
double div(double a, double b){
    return a / b;
}
vec3 binop(const vec3 &a, const vec3 &b, double(*f)(double, double)) {
    return vec3(f(a.x, b.x), f(a.y, b.y), f(a.z, b.z));
}
vec3 binop(const vec3 &a, double b, double(*f)(double, double)) {
    return vec3(f(a.x, b), f(a.y, b), f(a.z, b));
}
vec3 binop(double a, const vec3 &b, double(*f)(double, double)) {
    return vec3(f(a, b.x), f(a, b.y), f(a, b.z));
}
vec3 operator+(const vec3 &a, const vec3 &b) {
    return binop(a, b, add);
}
vec3 operator+(const vec3 &a, double b) {
    return binop(a, b, add);
}
vec3 operator+(double a, const vec3 &b) {
    return binop(a, b, add);
}
vec3 operator-(const vec3 &a, const vec3 &b) {
    return binop(a, b, sub);
}
vec3 operator-(const vec3 &a, double b) {
    return binop(a, b, sub);
}
vec3 operator-(double a, const vec3 &b) {
    return binop(a, b, sub);
}
vec3 operator*(const vec3 &a, const vec3 &b) {
    return binop(a, b, mul);
}
vec3 operator*(const vec3 &a, double b) {
    return binop(a, b, mul);
}
vec3 operator*(double a, const vec3 &b) {
    return binop(a, b, mul);
}

vec3 operator/(const vec3 &a, const vec3 &b) {
    return binop(a, b, div);
}
vec3 operator/(const vec3 &a, double b) {
    return binop(a, b, div);
}
vec3 operator/(double a, const vec3 &b) {
    return binop(a, b, div);
}

double dot(const vec3 &a, const vec3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double max(const vec3 &a) {
    return a.x > a.y && a.x > a.z ? a.x : a.y > a.z ? a.y : a.z;
}

vec3 cross(const vec3 &a, const vec3 &b) {
    return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
