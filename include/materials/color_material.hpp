#ifndef COLOR_MATERIAL_H
#define COLOR_MATERIAL_H

#include <material.hpp>

class ColorMaterial: public Material {
public:
    ColorMaterial(int n, float ka, float ks, glm::vec3 c):Material(n, ka, ks) {
        color = c;
    }

private:
    glm::vec3 color;
    glm::vec3 getObjectColor() {
        return color;
    }

};


#endif
