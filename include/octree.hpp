#ifndef OCTREE_H
#define OCTREE_H

#include <vector>

#include <geometry.hpp>
#include <bounding_box.hpp>

class Octree {
public:
    Octree(const BoundingBox &box) {
        bounding_box = box;
    }

    void add_shape(Geometry* shape) {
        shapes.push_back(shape);
    }

    bool intersect(const Ray &ray, glm::vec3 &normal, float &t, glm::vec2 &uv, Geometry* &shape) {
        if(!bounding_box.intersect(ray)) {
            return false;
        }
        t = 999.0f;
        float _t;
        glm::vec3 n;
        glm::vec2 _uv;
        Geometry* _shape;
        bool chosen = false;
        // for(int i = 0;i < children.size();i++) {
        //     if(children[i].intersect(ray, n, _t, _uv, _shape)) {
        //         if(_t < t) {
        //             t = _t;
        //             normal = n;
        //             uv = _uv;
        //             chosen = true;
        //         }
        //     }
        // }
        for(int i = 0;i < shapes.size();i++) {
            if(shapes[i]->intersect(ray, n, _t, _uv)) {
                if(_t < t) {
                    t = _t;
                    normal = n;
                    uv = _uv;
                    shape = _shape;
                    chosen = true;
                }
            }
        }

        return chosen;
    }

    std::vector<Octree> children;
    std::vector<Geometry*> shapes;
    BoundingBox bounding_box;
};

#endif
