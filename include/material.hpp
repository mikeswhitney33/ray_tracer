#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <vector>
#include <math.h>
#include <iostream>

enum MatType {LIGHT, NORMAL};

class Material {
public:
    Material(const int &n, const float &ka, const float &ks, const float &k_reflect=0.0f, const float &k_refract=0.0f, const float &i_refract=1.003) {
        phong = n;
        ambientStrength = ka;
        specularStrength = ks;
        reflectiveStrength = k_reflect;
        refractiveStrength = k_refract;
        indexOfRefraction = i_refract;
    }

    Material(Material* mat) {
        phong = mat->phong;
        ambientStrength = mat->ambientStrength;
        specularStrength = mat->specularStrength;
        reflectiveStrength = mat->reflectiveStrength;
        refractiveStrength = mat->refractiveStrength;
        indexOfRefraction = mat->indexOfRefraction;
    }
    virtual ~Material(){}

    float ambientStrength;
    float specularStrength;
    float reflectiveStrength;
    float refractiveStrength;
    float indexOfRefraction;

    int phong;

    virtual Material* copy() = 0;
    virtual glm::vec3 getObjectColor(const glm::vec2 &uv) const = 0;
};

#endif
