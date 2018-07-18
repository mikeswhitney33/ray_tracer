#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

typedef enum  {PRIMARY, DIFFUSE, REFLECT, REFRACT} RayType;

class Ray {
public:
    Ray(const RayType &ray_type, const glm::vec3 &orig, const glm::vec3 &dir): orig(orig), dir(dir) {
        invdir = 1.0f / dir;
        for(int i = 0;i < 3;i++) {
            sign[i] = (invdir[i] < 0);
        }
        type = ray_type;
    }
    Ray(const glm::vec3 &orig, const glm::vec3 &dir): orig(orig), dir(dir) {
        invdir = 1.0f / dir;
        for(int i = 0;i < 3;i++) {
            sign[i] = (invdir[i] < 0);
        }
        type = PRIMARY;
    }

    glm::vec3 eval (const float &t) const {
        return orig + t * dir;
    }

    RayType type;
    glm::vec3 orig, dir, invdir;
    int sign[3];
};

#endif
