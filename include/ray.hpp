#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray {
public:
    Ray(const glm::vec3 &orig, const glm::vec3 &dir): orig(orig), dir(dir) {
        invdir = 1.0f / dir;
        for(int i = 0;i < 3;i++) {
            sign[i] = (invdir[i] < 0);
        }
    }
    glm::vec3 orig, dir, invdir;
    int sign[3];

    glm::vec3 eval (const float &t) const {
        return orig + t * dir;
    }
};

#endif
