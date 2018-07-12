#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <light.hpp>

class DirectionalLight : public Light {
public:
    DirectionalLight(glm::vec3 c, glm::vec3 d): Light(c) {
        direction = d;
    }

    glm::vec3 getDirection(glm::vec3 pos) {
        return direction;
    }

    float getDistance(glm::vec3 pos) {
        return 999999.0f;
    }

private:
    glm::vec3 direction;
};

#endif
