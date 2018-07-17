#ifndef OCTREE_H
#define OCTREE_H

#include <vector>

#include <geometry.hpp>
#include <bounding_box.hpp>

class Octree {
public:
    Octree(const BoundingBox &box):bounding_box(box) {}
    Octree(const glm::vec3 &vmin, const glm::vec3 &vmax):bounding_box(vmin, vmax) {}

    virtual ~Octree() {
        for(int i = 0;i < shapes.size();i++) {
            delete shapes[i];
        }
    }

    bool intersect(Geometry* shape) {
        return shape->intersect(bounding_box);
    }

    void add_shape(Geometry* shape) {
        int num_inters = 0;
        int idx = 0;
        for(int i = 0;i < children.size();i++) {
            if(children[i].intersect(shape)) {
                idx = i;
                num_inters++;
            }
        }
        if(num_inters == 1) {
            children[idx].add_shape(shape);
        }
        else {
            shapes.push_back(shape);
            if(num_inters == 0 && children.size() == 0 && shapes.size() > 5) {
                split();
            }
        }
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
        for(int i = 0;i < children.size();i++) {
            if(children[i].intersect(ray, n, _t, _uv, _shape)) {
                if(_t < t) {
                    t = _t;
                    normal = n;
                    uv = _uv;
                    chosen = true;
                    shape = _shape;
                }
            }
        }
        for(int i = 0;i < shapes.size();i++) {
            if(shapes[i]->intersect(ray, n, _t, _uv)) {
                if(_t < t) {
                    t = _t;
                    normal = n;
                    uv = _uv;
                    shape = shapes[i];
                    chosen = true;
                }
            }
        }

        return chosen;
    }

    std::vector<Octree> children;
    std::vector<Geometry*> shapes;
    BoundingBox bounding_box;

private:
    void split() {
        glm::vec3 mid_point = (bounding_box.bounds[0] + bounding_box.bounds[1]) / 2.0f;

        // Upper
        //  Upper North West
        glm::vec3 unw_low(bounding_box.bounds[0].x, mid_point.y, mid_point.z);
        glm::vec3 unw_high(mid_point.x, bounding_box.bounds[1].y, bounding_box.bounds[1].z);
        children.push_back(Octree(unw_low, unw_high));
        //  Upper North East
        glm::vec3 une_low = mid_point;
        glm::vec3 une_high = bounding_box.bounds[1];
        children.push_back(Octree(une_low, une_high));
        // Upper South West
        glm::vec3 usw_low(bounding_box.bounds[0].x, bounding_box.bounds[0].y, mid_point.z);
        glm::vec3 usw_high(mid_point.x, mid_point.y, bounding_box.bounds[1].z);
        children.push_back(Octree(usw_low, usw_high));
        // Upper South East
        glm::vec3 use_low(mid_point.x, bounding_box.bounds[0].y, mid_point.z);
        glm::vec3 use_high(bounding_box.bounds[1].x, mid_point.y, bounding_box.bounds[1].z);
        children.push_back(Octree(use_low, use_high));

        // Lower North West
        glm::vec3 lnw_low(bounding_box.bounds[0].x, mid_point.y, bounding_box.bounds[0].z);
        glm::vec3 lnw_high(mid_point.x, bounding_box.bounds[1].y, mid_point.z);
        children.push_back(Octree(lnw_low, lnw_high));
        // Lower North East
        glm::vec3 lne_low(mid_point.x, mid_point.y, bounding_box.bounds[0].z);
        glm::vec3 lne_high(bounding_box.bounds[1].x, bounding_box.bounds[1].y, mid_point.z);
        children.push_back(Octree(lne_low, lne_high));
        // Lower South West
        glm::vec3 lsw_low = bounding_box.bounds[0];
        glm::vec3 lsw_high = mid_point;
        children.push_back(Octree(lsw_low, lsw_high));
        // Lower South East
        glm::vec3 lse_low(mid_point.x, bounding_box.bounds[0].y, bounding_box.bounds[0].z);
        glm::vec3 lse_high(bounding_box.bounds[1].x, mid_point.y, mid_point.z);
        children.push_back(Octree(lse_low, lse_high));

        std::vector<Geometry*> tmp;
        for(int i = 0;i < shapes.size();i++) {
            tmp.push_back(shapes[i]);
        }
        shapes.clear();
        for(int i = 0;i < tmp.size();i++) {
            add_shape(tmp[i]);
        }
    }
};

#endif
