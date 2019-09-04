#ifndef LARGEPT_HPP
#define LARGEPT_HPP

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <limits>

#include "../headers/geometry/vec3.hpp"
#include "../headers/factories/material-factory.hpp"
#include "../headers/factories/shape-factory.hpp"

#define clamp(x) (x < 0 ? 0 : x > 1 ? 1 : x)
#define toInt(x) (int(pow(clamp(x), 1 / 2.2) * 255 + 0.5))
#define NUM_SHAPES 9

extern const Shape* shapes[NUM_SHAPES];

bool intersect(const vec3 &ro, const vec3 &rd, double &t, int &id);

#endif