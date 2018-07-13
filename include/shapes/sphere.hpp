#ifndef SPHERE_H
#define SPHERE_H

#include <geometry.hpp>

class Sphere : public Geometry {
public:
    Sphere(Material* mat, glm::vec3 c, float r):Geometry(mat)  {
        center = c;
        radius = r;
    }

    virtual ~Sphere(){

    }

    bool intersect(glm::vec3 r0, glm::vec3 rd, glm::vec3 &normal, float &t) {
        glm::vec3 oc = center - r0;
        float oc_mag = glm::length(oc);
        bool inside_sphere = oc_mag < radius;
        float tca = glm::dot(rd, oc);
        if(tca < 0 && !inside_sphere) {
            return false;
        }
        float thcsq = radius * radius - oc_mag * oc_mag + tca * tca;
        if(thcsq < 0) {
            return false;
        }
        float thc = (float)sqrt(thcsq);
        t = (!inside_sphere)? tca - thc : tca + thc;
        glm::vec3 inter = r0 + rd * t;
        normal = (inter - center) / radius;
        return true;
    }

    glm::vec2 getUV(glm::vec3 pt) {
        glm::vec3 n = glm::normalize(pt - center);
        float u = atan2(n.x, n.z) / (2 * M_PI) + 0.5;
        float v = n.y * 0.5 + 0.5;
        return glm::vec2(u, v);
    }

private:
    glm::vec3 center;
    float radius;
};

#endif
