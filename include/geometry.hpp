#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include <material.hpp>


class Geometry {
public:
    Geometry(Material* mat) {
        material = mat;
        material->shape = this;
    }
    virtual ~Geometry(){
        std::cout << "Destructing Geometry!" << std::endl;
        delete material;
    }
    virtual bool intersect(glm::vec3 r0, glm::vec3 rd, glm::vec3 &normal, float &t) = 0;
    Material* getMaterial() {
        return material;
    }

    virtual glm::vec2 getUV(glm::vec3 pt) = 0;
private:
    Material* material;
};

#endif