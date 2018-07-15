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
        delete material;
    }
    virtual bool intersect(glm::vec3 r0, glm::vec3 rd, glm::vec3 &normal, float &t) = 0;
    Material* getMaterial() {
        return material;
    }

    virtual glm::vec2 getUV(glm::vec3 pt) = 0;
protected:
    glm::vec3 lowExtents;
    glm::vec3 highExtents;

    bool checkShapeBoundingVolumePlane(float xl, float xh, float x0, float xd, float& t_near, float& t_far){
        if(xd == 0) {
          if(x0 < xl || x0 > xh) {
            return false;
          }
        }
        else {
          float t1, t2;
          t1 = (xl - x0)/xd;
          t2 = (xh - x0)/xd;
          if(t1 > t2) {
            float tmp = t1;
            t1 = t2;
            t2 = tmp;
          }
          if(t1 > t_near) {
            t_near = t1;
          }
          if(t2 < t_far){
            t_far = t2;
          }
          if(t_near > t_far) {
            return false;
          }
          if(t_far < 0) {
            return false;
          }
        }
        return true;
    }

    bool inBounds(glm::vec3 r0, glm::vec3 rd){
        float t_near = -9999999.0f, t_far = 9999999.0f;
        if(!checkShapeBoundingVolumePlane(lowExtents.x, highExtents.x, r0.x, rd.x, t_near, t_far)) {
            return false;
        }
        if(!checkShapeBoundingVolumePlane(lowExtents.y, highExtents.y, r0.y, rd.y, t_near, t_far)) {
            return false;
        }
        if(!checkShapeBoundingVolumePlane(lowExtents.z, highExtents.z, r0.z, rd.z, t_near, t_far)) {
            return false;
        }
        return true;
    }
private:
    Material* material;
    virtual void setExtents() = 0;
};

#endif
