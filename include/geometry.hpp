#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include <material.hpp>
#include <bounding_box.hpp>
#include <ray.hpp>


class Geometry {
public:
    Geometry(Material* mat) {
        material = mat;
    }
    virtual ~Geometry(){
        delete material;
    }
    virtual bool intersect(const Ray &ray, glm::vec3 &normal, float &t, glm::vec2 &uv) = 0;

    Material* material;
    BoundingBox bounding_box;
protected:
    bool inBounds(const Ray &ray) {
        return bounding_box.intersect(ray);
    }
private:
    virtual void setExtents() = 0;
};

#endif
