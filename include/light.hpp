#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <ray.hpp>
#include <geometry.hpp>
#include <materials/light_material.hpp>

// class Geometry{};
class Light : public Geometry {
public:
    Light(glm::vec3 c):Geometry(new LightMaterial(c)) {}
    virtual ~Light(){}

    glm::vec3 getColor() {
        return static_cast<LightMaterial*>(material)->color;
    }

    virtual bool isLight() {
        return true;
    }

    virtual glm::vec3 getDirection(glm::vec3 pos) = 0;
    virtual float getDistance(glm::vec3 pos) = 0;
    virtual bool intersect(
        const Ray &ray, glm::vec3 &normal, float &t, glm::vec2 &uv) = 0;

private:
    virtual void setExtents() = 0;
};

#endif
