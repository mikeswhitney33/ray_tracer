#ifndef MATERIAL_FACTORY_HPP
#define MATERIAL_FACTORY_HPP

#include "../materials/material.hpp"


Material* makeDiffuseMaterial(const vec3 &color, const vec3 &emission);
Material* makeSpecularMaterial(const vec3 &color, const vec3 &emission);
Material* makeRefractiveMaterial(const vec3 &color, const vec3 &emission);


#endif