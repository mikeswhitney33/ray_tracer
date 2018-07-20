#ifndef AREA_LIGHT_H
#define AREA_LIGHT_H

#include <light.hpp>

class AreaLight: public Light {
public:
    AreaLight(glm::vec3 c, glm::vec3 vmin, glm::vec3 vmax):Light(c), bounding_box(vmin, vmax){}
    virtual glm::vec3 getDirection(glm::vec3 pos) {
        float x = random_float(bounding_box.bounds[0].x, bounding_box.bounds[1].x);
        float y = random_float(bounding_box.bounds[0].y, bounding_box.bounds[1].y);
        float z = random_float(bounding_box.bounds[0].z, bounding_box.bounds[1].z);
        glm::vec3 pt(x, y, z);
        return pt - pos;
    }
    virtual float getDistance(glm::vec3 pos) {
        float x = random_float(bounding_box.bounds[0].x, bounding_box.bounds[1].x);
        float y = random_float(bounding_box.bounds[0].y, bounding_box.bounds[1].y);
        float z = random_float(bounding_box.bounds[0].z, bounding_box.bounds[1].z);
        glm::vec3 pt(x, y, z);
        return glm::distance(pt, pos);
    }

private:
    BoundingBox bounding_box;
};


#endif
