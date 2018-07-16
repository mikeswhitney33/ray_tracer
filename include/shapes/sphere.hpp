#ifndef SPHERE_H
#define SPHERE_H

#include <geometry.hpp>

class Sphere : public Geometry {
public:
    Sphere(Material* mat, glm::vec3 c, float r):Geometry(mat)  {
        center = c;
        radius = r;
        setExtents();
    }

    virtual ~Sphere(){}

    bool intersect(glm::vec3 r0, glm::vec3 rd, glm::vec3 &normal, float &t, glm::vec2 &uv) {
        if(!inBounds(r0, rd)) {
            return false;
        }
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

        float u = atan2(normal.x, normal.z) / (2 * M_PI) + 0.5;
        float v = normal.y * 0.5 + 0.5;
        uv = glm::vec2(u, v);
        return true;
    }


private:
    glm::vec3 center;
    float radius;

    void setExtents() {
        lowExtents = center - radius;
        highExtents = center + radius;
    }
};

#endif
