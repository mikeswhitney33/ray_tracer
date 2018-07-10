#ifndef SPHERE_H
#define SPHERE_H

#include <geometry.hpp>

class Sphere : public Geometry {
public:
    Sphere(Material* mat, glm::vec3 c, float r):Geometry(mat)  {
        center = c;
        radius = r;
    }

    bool intersect(glm::vec3 r0, glm::vec3 rd, glm::vec3 &normal, float &t) {
        float t0, t1;
        glm::vec3 L = center - r0;
        float tca = glm::dot(L, rd);
        if(tca < 0) {
            return false;
        }
        float d2 = glm::dot(L, L) - tca * tca;
        if(d2 > radius) {
            return false;
        }
        float thc = sqrt(radius - d2);
        t0 = tca - thc;
        t1 = tca + thc;
        if(t0 > t1) {
            std::swap(t0, t1);
        }
        if(t0 < 0) {
            t0 = t1;
            if(t0 < 0) {
                return false;
            }
        }
        t = t0;
        glm::vec3 inter = r0 + rd * t;
        normal = (inter - center) / radius;
        return true;
    }

private:
    glm::vec3 center;
    float radius;
};

#endif
