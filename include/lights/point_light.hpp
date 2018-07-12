#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <light.hpp>

class PointLight : public Light {
public:
    PointLight(glm::vec3 c, glm::vec3 p): Light(c) {
        point = p;
    }

    glm::vec3 getDirection(glm::vec3 pos) {
        return point - pos;
    }

    float getDistance(glm::vec3 pos) {
        return glm::distance(point, pos);
    }

private:
    glm::vec3 point;
};

#endif
