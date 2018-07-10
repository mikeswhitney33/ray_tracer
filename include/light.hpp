#ifndef LIGHT_H
#define LIGHT_H


class Light {
public:
    Light(glm::vec3 c) {
        color = c;
    }
    glm::vec3 getColor() {
        return color;
    }

    virtual glm::vec3 getDirection(glm::vec3 pos) = 0;
private:
    glm::vec3 color;
};

#endif
