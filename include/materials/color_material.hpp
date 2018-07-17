#ifndef COLOR_MATERIAL_H
#define COLOR_MATERIAL_H

#include <material.hpp>

class ColorMaterial: public Material {
public:
    ColorMaterial(const glm::vec3 &c, const int &n, const float &ka, const float &ks, const float &k_reflect=0.0f, const float &k_refract=0.0f, const float &i_refract=1.003):Material(n, ka, ks, k_reflect, k_refract, i_refract) {
        color = c;
        id = 0;
    }

    ColorMaterial(Material* mat): Material(mat) {
        ColorMaterial* c_mat = dynamic_cast<ColorMaterial*>(mat);
        color = c_mat->color;
        id = c_mat->id;
    }

    virtual ~ColorMaterial(){}

private:
    glm::vec3 color;
    glm::vec3 getObjectColor(const glm::vec2 &uv) {
        return color;
    }
};

#endif
